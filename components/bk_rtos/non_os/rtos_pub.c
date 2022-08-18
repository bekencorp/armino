#include <string.h>
#include <stdlib.h>
#include <common/bk_generic.h>
#include <common/bk_kernel_err.h>
#include <os/os.h>
#include <os/mem.h>
#include <components/system.h>
#include "bk_fake_clock.h"
#include "bk_arch.h"
#include "portmacro.h"
#include "heap_4.h"

#ifndef TIMER_THREAD_STACK_SIZE
#define TIMER_THREAD_STACK_SIZE      1024 + 4*1024
#endif
#define TIMER_QUEUE_LENGTH           5

typedef struct
{
    event_handler_t function;
    void*           arg;
} timer_queue_message_t;

typedef struct
{
    event_handler_t function;
    void*           arg;
} beken_event_message_t;

extern uint32_t platform_is_in_interrupt_context( void );
extern uint32_t platform_cpsr_content( void );

static bool s_is_started_scheduler = false;
static volatile uint32_t s_sema_cnt = 0;
static volatile uint32_t s_sema_bit = 0;

bk_err_t rtos_create_thread(beken_thread_t* thread, uint8_t priority, const char* name,
							beken_thread_function_t function, uint32_t stack_size, beken_thread_arg_t arg)
{
	if (function) {
		function(arg);
	}
	return kNoErr;
}

bk_err_t rtos_delete_thread( beken_thread_t* thread )
{
	return kNoErr;
}

bk_err_t rtos_thread_join(beken_thread_t* thread)
{
	return kNoErr;
}

bool rtos_is_current_thread(beken_thread_t* thread)
{
	return true;
}

beken_thread_t* rtos_get_current_thread(void)
{
	return NULL;
}

bk_err_t rtos_check_stack(void)
{
	return kNoErr;
}

bk_err_t rtos_thread_force_awake(beken_thread_t *thread)
{
	return kNoErr;
}

void rtos_thread_sleep(uint32_t seconds)
{

}

bk_err_t beken_time_get_time(beken_time_t *time_ptr)
{
	return kNoErr;
}

bk_err_t beken_time_set_time(beken_time_t* time_ptr)
{
	return kNoErr;
}

bk_err_t rtos_init_semaphore(beken_semaphore_t* semaphore, int maxCount)
{
	*(uint32_t *)semaphore = s_sema_bit++;

	return kNoErr;
}

bk_err_t rtos_init_semaphore_ex(beken_semaphore_t* semaphore, int maxCount, int init_count)
{
	return kNoErr;
}

bk_err_t rtos_get_semaphore(beken_semaphore_t* semaphore, uint32_t timeout_ms)
{
	uint32_t sema_bit = *(uint32_t *)semaphore;

	while (1) {
		if (s_sema_cnt & BIT(sema_bit)) {
			s_sema_cnt &= (~BIT(sema_bit));
			return kNoErr;
		}
	}

	return kNoErr;
}

int rtos_get_semaphore_count(beken_semaphore_t* semaphore)
{
	return 0;
}

int rtos_set_semaphore(beken_semaphore_t* semaphore)
{
	uint32_t sema_bit = *(uint32_t *)semaphore;

	s_sema_cnt |= BIT(sema_bit);
	return kNoErr;
}

bk_err_t rtos_deinit_semaphore(beken_semaphore_t* semaphore)
{
	return kNoErr;
}

void rtos_enter_critical(void)
{

}

void rtos_exit_critical(void)
{

}

bk_err_t rtos_init_mutex(beken_mutex_t *mutex)
{
	return kNoErr;
}

bk_err_t rtos_trylock_mutex(beken_mutex_t *mutex)
{
	return kNoErr;
}

bk_err_t rtos_lock_mutex(beken_mutex_t* mutex)
{
	return kNoErr;
}

bk_err_t rtos_unlock_mutex(beken_mutex_t* mutex)
{
	return kNoErr;
}

bk_err_t rtos_deinit_mutex(beken_mutex_t* mutex)
{
	return kNoErr;
}

bk_err_t rtos_init_queue(beken_queue_t* queue, const char* name, uint32_t message_size, uint32_t number_of_messages)
{
	return kNoErr;
}

bk_err_t rtos_push_to_queue(beken_queue_t* queue, void* message, uint32_t timeout_ms)
{
	return kNoErr;
}

bk_err_t rtos_push_to_queue_front(beken_queue_t* queue, void* message, uint32_t timeout_ms)
{
	return kNoErr;
}

bk_err_t rtos_pop_from_queue(beken_queue_t *queue, void *message, uint32_t timeout_ms)
{
	return kNoErr;
}

bk_err_t rtos_deinit_queue(beken_queue_t *queue)
{
	return kNoErr;
}

bool rtos_is_queue_empty(beken_queue_t *queue)
{
	return true;
}

bool rtos_is_queue_full(beken_queue_t *queue)
{
	return true;
}

bk_err_t rtos_start_oneshot_timer(beken2_timer_t *timer)
{
	return kNoErr;
}

bk_err_t rtos_deinit_oneshot_timer(beken2_timer_t *timer)
{
	return kNoErr;
}

bk_err_t rtos_stop_oneshot_timer(beken2_timer_t *timer)
{
	return kNoErr;
}

bool rtos_is_oneshot_timer_init(beken2_timer_t *timer)
{
	return true;
}

bool rtos_is_oneshot_timer_running(beken2_timer_t *timer)
{
	return true;
}

bk_err_t rtos_oneshot_reload_timer(beken2_timer_t *timer)
{
	return kNoErr;
}

bk_err_t rtos_oneshot_reload_timer_ex(beken2_timer_t *timer,
										uint32_t time_ms,
										timer_2handler_t function,
										void *larg,
										void *rarg)
{
	return kNoErr;
}

bk_err_t rtos_init_oneshot_timer(beken2_timer_t *timer,
									uint32_t time_ms,
									timer_2handler_t function,
									void *larg,
									void *rarg)
{
	return kNoErr;
}

bk_err_t rtos_init_timer(beken_timer_t *timer,
							uint32_t time_ms,
							timer_handler_t function,
							void *arg)
{
	return kNoErr;
}

bk_err_t rtos_start_timer(beken_timer_t *timer)
{
	return kNoErr;
}

bk_err_t rtos_stop_timer(beken_timer_t *timer)
{
	return kNoErr;
}

bk_err_t rtos_reload_timer(beken_timer_t *timer)
{
	return kNoErr;
}

bk_err_t rtos_change_period(beken_timer_t *timer, uint32_t time_ms)
{
	return kNoErr;
}

bk_err_t rtos_deinit_timer(beken_timer_t *timer)
{
	return kNoErr;
}

uint32_t rtos_get_timer_expiry_time(beken_timer_t *timer)
{
	return 0;
}

uint32_t rtos_get_next_expire_time()
{
	return 0;
}

uint32_t rtos_get_current_timer_count(void)
{
	return 0;
}

bool rtos_is_timer_init(beken_timer_t *timer)
{
	return true;
}

bool rtos_is_timer_running(beken_timer_t *timer)
{
	return true;
}

void rtos_suspend_thread(beken_thread_t *thread)
{

}

void rtos_resume_thread(beken_thread_t *thread)
{

}

uint32_t beken_ms_per_tick(void)
{
	return bk_get_ms_per_tick();
}

/**
 * Gets time in milliseconds since RTOS start
 *
 * @Note: since this is only 32 bits, it will roll over every 49 days, 17 hours.
 *
 * @returns Time in milliseconds since RTOS started.
 */
beken_time_t rtos_get_time(void)
{
	return (beken_time_t)0;
}

/**
 * Delay for a number of milliseconds
 *
 * Processing of this function depends on the minimum sleep
 * time resolution of the RTOS.
 * The current thread sleeps for the longest period possible which
 * is less than the delay required, then makes up the difference
 * with a tight loop
 *
 * @return bk_err_t : kNoErr if delay was successful
 *
 */
bk_err_t rtos_delay_milliseconds(uint32_t num_ms)
{
	extern void delay(int num);
	delay(100 * num_ms);

	return kNoErr;
}

void *beken_malloc(size_t xWantedSize)
{
	return (void *)os_malloc(xWantedSize);
}

void beken_free( void *pv )
{
	os_free(pv);
}

void *beken_realloc( void *pv, size_t xWantedSize )
{
	return os_realloc(pv, xWantedSize);
}

void rtos_start_scheduler(void)
{
	s_is_started_scheduler = true;
}

bool rtos_is_scheduler_started(void)
{
	return s_is_started_scheduler;
}

uint32_t rtos_get_cpsr(void)
{
	return platform_cpsr_content();
}

char* rtos_get_name(void)
{
#define RTOS_NAME                     "NON_OS"
	return RTOS_NAME;
}

char* rtos_get_version(void)
{
#define RTOS_VERSION                  "NA"
	return RTOS_VERSION;
}

size_t rtos_get_free_heap_size(void)
{
	return xPortGetFreeHeapSize();
}

size_t rtos_get_min_ever_free_heap_size(void)
{
	return xPortGetMinimumEverFreeHeapSize();
}

uint32_t rtos_disable_int(void)
{
	return port_disable_interrupts_flag();
}

void rtos_enable_int(uint32_t int_level)
{
	port_enable_interrupts_flag(int_level);
}

void rtos_stop_int(void)
{
	//portDISABLE_INTERRUPTS();
}

void rtos_start_int(void)
{
	//portENABLE_INTERRUPTS();
}

bool rtos_is_in_interrupt_context(void)
{
	return platform_is_in_interrupt_context();
}

void rtos_wait_for_interrupt(void)
{
	arch_wait_for_interrupt();
}

void rtos_dump_task_list(void)
{

}

void rtos_dump_backtrace(void)
{

}

