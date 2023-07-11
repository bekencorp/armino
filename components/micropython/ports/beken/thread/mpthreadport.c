#include "stdio.h"

#include "py/runtime.h"
#include "py/gc.h"
#include "py/mpthread.h"
#include "py/mphal.h"

#if MICROPY_PY_THREAD
#include "os/os.h"

#define MP_THREAD_PRIORITY                              (5)
#define MP_THREAD_MIN_STACK_SIZE                        (4 * 1024)
#define MP_THREAD_DEFAULT_STACK_SIZE                    (MP_THREAD_MIN_STACK_SIZE + 1024)

// this structure forms a linked list, one node per active thread
typedef struct _mp_thread_t {
    beken_thread_t id;        // system id of thread
    void *(*entry)(void *);   //orignal task entry
    int ready;              // whether the thread is ready and running
    void *arg;              // thread Python args, a GC root pointer
    void *stack;            // pointer to the stack
    size_t stack_len;       // number of words in the stack
    mp_state_thread_t *state;
    struct _mp_thread_t *next;
} mp_thread_t;

// the mutex controls access to the linked list
STATIC mp_thread_mutex_t thread_mutex;
STATIC mp_thread_t thread_entry0;

void mp_thread_init(void *stack, uint32_t stack_len) {
    if (NULL == stack) {
        //TODO: get stack address with rtos_xxx API
    }
    // create the first entry in the linked list of all threads
    thread_entry0.id = rtos_get_current_thread();
    thread_entry0.ready = 1;
    thread_entry0.entry = NULL;
    thread_entry0.arg = NULL;
    thread_entry0.stack = stack;
    thread_entry0.stack_len = stack_len;
    thread_entry0.next = NULL;
    mp_thread_mutex_init(&thread_mutex);

    mp_thread_set_state(&mp_state_ctx.thread);

    // memory barrier to ensure above data is committed
}

void mp_thread_gc_others(void) {
    mp_thread_mutex_lock(&thread_mutex, 1);
    for (mp_thread_t *th = &thread_entry0; th != NULL; th = th->next) {
        gc_collect_root((void **)&th, 1);
        gc_collect_root(&th->arg, 1); // probably not needed
        if (rtos_is_current_thread(&th->id)) {
            continue;
        }
        if (!th->ready || !th->stack) {
            continue;
        }
        gc_collect_root(th->stack, th->stack_len);
    }
    mp_thread_mutex_unlock(&thread_mutex);
}

mp_state_thread_t *mp_thread_get_state() {
    mp_state_thread_t *state = NULL;
    mp_thread_mutex_lock(&thread_mutex, 1);
    for (mp_thread_t *th = &thread_entry0; th != NULL; th = th->next) {
        if (rtos_is_current_thread(&th->id)) {
            state = th->state;
            break;
        }
    }
    mp_thread_mutex_unlock(&thread_mutex);

    if (NULL == state) {
        mp_raise_msg_varg(&mp_type_OSError, MP_ERROR_TEXT("can't find state lr=%p"), __builtin_return_address(0));
    }

    return state;
}

void mp_thread_set_state(mp_state_thread_t *state) {
    mp_thread_mutex_lock(&thread_mutex, 1);
    for (mp_thread_t *th = &thread_entry0; th != NULL; th = th->next) {
        if (rtos_is_current_thread(&th->id)) {
            th->state = state;
            break;
        }
    }
    mp_thread_mutex_unlock(&thread_mutex);
}

void mp_thread_start(void) {
    mp_thread_mutex_lock(&thread_mutex, 1);
    for (mp_thread_t *th = &thread_entry0; th != NULL; th = th->next) {
        if (rtos_is_current_thread(&th->id)) {
            th->ready = 1;
            break;
        }
    }
    mp_thread_mutex_unlock(&thread_mutex);
}

STATIC void mp_thread_entry(void *arg) {
    mp_thread_t *th = (void *)arg;

    if (NULL == th) {
    } else if (NULL != th->entry) {
        th->entry(th->arg);
    }
    rtos_delete_thread(NULL);
}

void mp_thread_create_ex(void *(*entry)(void *), void *arg, size_t *stack_size, int priority, char *name) {
    // store thread entry function into a global variable so we can access it
	int result;

    if (*stack_size == 0) {
        *stack_size = MP_THREAD_DEFAULT_STACK_SIZE; // default stack size
    } else if (*stack_size < MP_THREAD_MIN_STACK_SIZE) {
        *stack_size = MP_THREAD_MIN_STACK_SIZE; // minimum stack size
    }

    // Allocate linked-list node (must be outside thread_mutex lock)
    mp_thread_t *th = m_new_obj(mp_thread_t);

    if (NULL == th) {
    	return;
    }

    th->entry = entry;
    th->ready = 0;
    th->arg = arg;
    th->stack = NULL; //TODO: get stack address with rtos_xxx API
    th->stack_len = *stack_size;

    mp_thread_mutex_lock(&thread_mutex, 1);

    // create thread
	result = rtos_create_thread(&th->id,
		priority,
		name,
		(beken_thread_function_t)mp_thread_entry,
		th->stack_len,
		(beken_thread_arg_t)th);

    if (result != BK_OK) {
        mp_thread_mutex_unlock(&thread_mutex);
        mp_raise_msg(&mp_type_OSError, MP_ERROR_TEXT("can't create thread"));
    }

    // add thread to linked list of all threads
    for (mp_thread_t *prev = &thread_entry0; prev != NULL; prev = prev->next) {
        // unlink the node from the list
        if (prev->next == NULL) {
		    prev->next = th;
            break;
        }
    }

    // adjust the stack_size to provide room to recover from hitting the limit
    *stack_size -= 1024;

    mp_thread_mutex_unlock(&thread_mutex);
}

void mp_thread_create(void *(*entry)(void *), void *arg, size_t *stack_size) {
    mp_thread_create_ex(entry, arg, stack_size, MP_THREAD_PRIORITY, "mp_thread");
}

void mp_thread_finish(void) {
    mp_thread_mutex_lock(&thread_mutex, 1);
    for (mp_thread_t *th = &thread_entry0; th != NULL; th = th->next) {
        if (rtos_is_current_thread(&th->id)) {
            th->ready = 0;
            break;
        }
    }
    mp_thread_mutex_unlock(&thread_mutex);
}

// This is called from the FreeRTOS idle task and is not within Python context,
// so MP_STATE_THREAD is not valid and it does not have the GIL.
void vPortCleanUpTCB(void *tcb) {
    mp_thread_t *prev = NULL;
    mp_thread_mutex_lock(&thread_mutex, 1);
    for (mp_thread_t *th = &thread_entry0; th != NULL; prev = th, th = th->next) {
        // unlink the node from the list
        if ((void *)th->id == tcb) {
            if (prev != NULL) {
                prev->next = th->next;
            } else {
                // move the start pointer
                //thread = th->next;
            }
            // The "th" memory will eventually be reclaimed by the GC.
            break;
        }
    }
    mp_thread_mutex_unlock(&thread_mutex);
}

void mp_thread_mutex_init(mp_thread_mutex_t *mutex) {
    // Need a binary semaphore so a lock can be acquired on one Python thread
    // and then released on another.
    if (NULL == mutex->handle) {
        rtos_init_mutex(&mutex->handle);
    }
}

int mp_thread_mutex_lock(mp_thread_mutex_t *mutex, int wait) {
	if (wait) {
	    return rtos_lock_mutex(&mutex->handle);
	} else {
	    return rtos_trylock_mutex(&mutex->handle);
	}
}

void mp_thread_mutex_unlock(mp_thread_mutex_t *mutex) {
    rtos_unlock_mutex(&mutex->handle);
}

void mp_thread_deinit(void) {
    for (;;) {
        // Find a task to delete
        beken_thread_t id = NULL;
        mp_thread_mutex_lock(&thread_mutex, 1);
        for (mp_thread_t *th = &thread_entry0; th != NULL; th = th->next) {
            // Don't delete the current task
            if (!rtos_is_current_thread(&th->id)) {
                id = th->id;
                break;
            }
        }
        mp_thread_mutex_unlock(&thread_mutex);

        if (id == NULL) {
            // No tasks left to delete
            break;
        } else {
            // Call FreeRTOS to delete the task (it will call vPortCleanUpTCB)
            rtos_delete_thread(&id);
        }
    }
}

#else

void vPortCleanUpTCB(void *tcb) {
}

#endif // MICROPY_PY_THREAD
