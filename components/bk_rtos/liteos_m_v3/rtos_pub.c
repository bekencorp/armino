#include <common/bk_include.h>
#include <os/os.h>
#include "los_config.h"
#include "los_context.h"
#include "los_task.h"
#include "los_queue.h"
#include "los_sem.h"
#include "los_mux.h"
#include "los_memory.h"
#include "los_interrupt.h"
#include "los_swtmr.h"
#include "bk_los_timer.h"
#include "portmacro.h"

/* OS_TASK_PRIORITY_HIGHEST and OS_TASK_PRIORITY_LOWEST is reserved for internal TIMER and IDLE task use only. */
#define ISVALID_LOS_PRIORITY(losPrio) ((losPrio) > OS_TASK_PRIORITY_HIGHEST && (losPrio) < OS_TASK_PRIORITY_LOWEST)
/**
* @brief Enumerates thread states.
*
*/
typedef enum {
  /** The thread is inactive. */
  osThreadInactive        =  0,
  /** The thread is ready. */
  osThreadReady           =  1,
  /** The thread is running. */
  osThreadRunning         =  2,
  /** The thread is blocked. */
  osThreadBlocked         =  3,
  /** The thread is terminated. */
  osThreadTerminated      =  4,
  /** The thread is abnormal. */
  osThreadError           = -1,
  /** Reserved */
  osThreadReserved        = 0x7FFFFFFF
} osThreadState_t;
  
#if (LOSCFG_BASE_CORE_SWTMR_ALIGN == 1)
  /**
  * @brief Enumerates timer permissions.
  *
  * @since 1.0
  * @version 1.0
  */
  typedef enum	{
	/** The timer is not allowed to wake up the RTOS. */
	osTimerRousesIgnore 	  = 	0,
	/** The timer is allowed to wake up the RTOS. */
	osTimerRousesAllow		  = 	1
  } osTimerRouses_t;
  
  /**
  * @brief Enumerates timer alignment modes.
  *
  */
  typedef enum	{
	/** The timer ignores alignment. */
	osTimerAlignIgnore		  = 	0,
	/** The timer allows alignment. */
	osTimerAlignAllow		  = 	1
  } osTimerAlign_t;
#endif //(LOSCFG_BASE_CORE_SWTMR_ALIGN == 1)

/******************************************************
 *               Variable Define
 ******************************************************/
uint32_t ms_to_tick_ratio = LOS_TICK_MS;
beken_time_t beken_time_offset = 0;
static bool s_is_started_scheduler = false;

/******************************************************
 *               Function Declarations
 ******************************************************/
extern void arch_wait_for_interrupt(void);
extern VOID OsMemInfoPrint(VOID *pool);
extern uint32_t platform_cpsr_content(void);

/******************************************************
 *               Function Definitions
 ******************************************************/
bk_err_t rtos_create_thread( beken_thread_t *thread, uint8_t priority, const char *name, 
						beken_thread_function_t function, uint32_t stack_size, beken_thread_arg_t arg )
{
    uint32_t uwTid;
    UINT32 uwRet = LOS_OK;
	bk_err_t ret;
    LosTaskCB *pstTaskCB = NULL;
    TSK_INIT_PARAM_S stTskInitParam = {NULL};
    UINT16 usPriority;

	ret = kNoErr;
    if (OS_INT_ACTIVE) {
        ret = kGeneralErr;
		goto tinit_exit;
    }

    usPriority = priority;
    if (!ISVALID_LOS_PRIORITY(usPriority)) {
        ret = kUnsupportedErr;/* unsupported priority */
		goto tinit_exit;
    }

    stTskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)function;
    stTskInitParam.uwArg = (uint32_t)arg;
    //NOTES:Special code:Lite OS uses stack size more then freertos about 300 Bytes base on RISC-V.
    stTskInitParam.uwStackSize = stack_size + 0x180;
    stTskInitParam.pcName = (CHAR *)name;
    stTskInitParam.usTaskPrio = usPriority;

    uwRet = LOS_TaskCreate((UINT32 *)&uwTid, &stTskInitParam);
	RTOS_LOGI("[debug]LOS_TaskCreate:%s:%d\r\n", name, uwRet);
	if(LOS_OK != uwRet)
	{
		ret = kGeneralErr;
		goto tinit_exit;
	}
    pstTaskCB = OS_TCB_FROM_TID(uwTid);

	if(thread)
	{
		*thread = (beken_thread_t *)pstTaskCB;
	}

tinit_exit:
	BK_ASSERT(LOS_OK == uwRet);
	return ret;
}

bk_err_t rtos_delete_thread(beken_thread_t *thread)
{
    UINT32 uwRet;
	uint32_t selfTid;
    LosTaskCB *pstTaskCB = NULL;

    if (OS_INT_ACTIVE) {
        return kUnknownErr;
    }

    if (thread && (*thread == NULL)) {
        return kParamErr;
    }

	if(NULL == thread)
	{
	    selfTid = LOS_CurTaskIDGet();
	    pstTaskCB = OS_TCB_FROM_TID(selfTid);
	}
	else
	{
	    pstTaskCB = (LosTaskCB *)*thread;
	}

    uwRet = LOS_TaskDelete(pstTaskCB->taskID);

    switch (uwRet) {
        case LOS_ERRNO_TSK_OPERATE_IDLE:
        case LOS_ERRNO_TSK_SUSPEND_SWTMR_NOT_ALLOWED:
        case LOS_ERRNO_TSK_ID_INVALID:
            return kParamErr;

        case LOS_ERRNO_TSK_NOT_CREATED:
            return kNoResourcesErr;

        default:
            return kNoErr;
    }
}

uint32_t _thread_get_status(beken_thread_t *thread)
{
    UINT16 taskStatus;
    osThreadState_t stState;
    LosTaskCB *pstTaskCB = NULL;

	BK_ASSERT(thread);
    if (OS_INT_ACTIVE || *thread == NULL) {
        return osThreadError;
    }

    pstTaskCB = (LosTaskCB *)*thread;
    taskStatus = pstTaskCB->taskStatus;

    if (taskStatus & OS_TASK_STATUS_RUNNING) {
        stState = osThreadRunning;
    } else if (taskStatus & OS_TASK_STATUS_READY) {
        stState = osThreadReady;
    } else if (taskStatus &
        (OS_TASK_STATUS_DELAY | OS_TASK_STATUS_PEND | OS_TASK_STATUS_SUSPEND)) {
        stState = osThreadBlocked;
    } else if (taskStatus & OS_TASK_STATUS_UNUSED) {
        stState = osThreadInactive;
    } else {
        stState = osThreadError;
    }

    return stState;
}

bk_err_t rtos_thread_join(beken_thread_t *thread)
{
	BK_ASSERT(thread);
    while ( _thread_get_status( *thread ) != osThreadInactive )
    {
        rtos_delay_milliseconds(10);
    }
    
    return kNoErr;
}

bool rtos_is_current_thread( beken_thread_t *thread )
{
    uint32_t selfTid = LOS_CurTaskIDGet();
    LosTaskCB *tcb = OS_TCB_FROM_TID(selfTid);
	
    if ( tcb == (LosTaskCB *)*thread )
    {
        return true;
    }
    else
    {
        return false;
    }
}

beken_thread_t *rtos_get_current_thread(void)
{
    uint32_t selfTid = LOS_CurTaskIDGet();
    LosTaskCB *tcb = OS_TCB_FROM_TID(selfTid);
	
    return (beken_thread_t *)tcb;
}

/* Re-write vTaskList to add a buffer size parameter */
bk_err_t rtos_print_thread_status( char* pcWriteBuffer, int xWriteBufferLen )
{
    //  TODO
    return kNoErr;
}

bk_err_t rtos_check_stack(void)
{
    //  TODO: Add stack checking here.
    return kNoErr;
}

bk_err_t rtos_thread_force_awake( beken_thread_t *thread )
{
    return kNoErr;
}

void rtos_thread_sleep(uint32_t seconds)
{
    UINT32 uwRet = LOS_OK;
	
	uwRet = LOS_TaskDelay(seconds * LOS_TICKS_PER_SECOND);
	(void)uwRet;
}

void rtos_suspend_thread(beken_thread_t *thread)
{
    UINT32 uwRet;
    LosTaskCB *pstTaskCB = NULL;
	beken_thread_t bk_thread;

    if (OS_INT_ACTIVE) {
        return;
    }

	bk_thread = *thread;
    if (bk_thread == NULL) {
        bk_thread = rtos_get_current_thread();
    }

    pstTaskCB = (LosTaskCB *)bk_thread;
    uwRet = LOS_TaskSuspend(pstTaskCB->taskID);
    switch (uwRet) {
        case LOS_ERRNO_TSK_OPERATE_IDLE:
        case LOS_ERRNO_TSK_SUSPEND_SWTMR_NOT_ALLOWED:
        case LOS_ERRNO_TSK_ID_INVALID:
            return;

        case LOS_ERRNO_TSK_NOT_CREATED:
        case LOS_ERRNO_TSK_ALREADY_SUSPENDED:
        case LOS_ERRNO_TSK_SUSPEND_LOCKED:
            return;

        default:
            return;
    }
}

void rtos_resume_thread(beken_thread_t *thread)
{
    UINT32 uwRet;
    LosTaskCB *pstTaskCB = NULL;
	beken_thread_t bk_thread;

    if (OS_INT_ACTIVE) {
        return;
    }

	bk_thread = *thread;
    if (bk_thread == NULL) {
        bk_thread = rtos_get_current_thread();
    }


    pstTaskCB = (LosTaskCB *)bk_thread;
    uwRet = LOS_TaskResume(pstTaskCB->taskID);

    switch (uwRet) {
        case LOS_ERRNO_TSK_ID_INVALID:
            return;

        case LOS_ERRNO_TSK_NOT_CREATED:
        case LOS_ERRNO_TSK_NOT_SUSPENDED:
            return;

        default:
            return;
    }
}

uint32_t rtos_get_tick_count(void)
{
	return (uint32_t)LOS_TickCountGet();
}

uint32_t beken_ms_per_tick(void)
{
	return ms_to_tick_ratio;
}

bk_err_t beken_time_get_time(beken_time_t* time_ptr)
{
    *time_ptr = (beken_time_t) ( LOS_TickCountGet() * ms_to_tick_ratio ) + beken_time_offset;
    return kNoErr;
}

bk_err_t beken_time_set_time(beken_time_t* time_ptr)
{
    beken_time_offset = *time_ptr - (beken_time_t) ( LOS_TickCountGet() * ms_to_tick_ratio );
    return kNoErr;
}

bk_err_t rtos_init_semaphore(beken_semaphore_t *semaphore, int max_count )
{
    return rtos_init_semaphore_ex(semaphore, max_count, 0);
}

bk_err_t rtos_init_semaphore_ex(beken_semaphore_t *semaphore, int max_count, int init_count)
{
    UINT32 uwRet;
    UINT32 uwSemId;

    if (OS_INT_ACTIVE) {
        *semaphore = (beken_semaphore_t)NULL;
		goto init_aexit;
    }

    if (1 == max_count) {
        uwRet = LOS_BinarySemCreate((UINT16)init_count, &uwSemId);
    } else {
        uwRet = LOS_SemCreate((UINT16)init_count, &uwSemId);
    }
	BK_ASSERT(LOS_OK == uwRet);

    if (uwRet == LOS_OK) {
        *semaphore = (beken_semaphore_t)(GET_SEM(uwSemId));
    } else {
        *semaphore = (beken_semaphore_t)NULL;
    }

init_aexit:
    return ( *semaphore != NULL ) ? kNoErr : kGeneralErr;
}

bk_err_t rtos_get_semaphore(beken_semaphore_t *semaphore, uint32_t timeout_ms )
{
    UINT32 uwRet;
    uint32_t timeout;

	BK_ASSERT(semaphore);
    if(timeout_ms == BEKEN_WAIT_FOREVER)
        timeout = LOS_WAIT_FOREVER;
    else
        timeout = timeout_ms / ms_to_tick_ratio;     

    if (*semaphore == NULL) {
        return kParamErr;
    }

    if (OS_INT_ACTIVE && (timeout != LOS_NO_WAIT)) {
        return kUnknownErr;
    }

    uwRet = LOS_SemPend(((LosSemCB *)*semaphore)->semID, timeout);
    if (uwRet == LOS_OK) {
        return kNoErr;
    } else if (uwRet == LOS_ERRNO_SEM_TIMEOUT) {
        return kTimeoutErr;
    } else if (uwRet == LOS_ERRNO_SEM_INVALID) {
        return kParamErr;
    } else if (uwRet == LOS_ERRNO_SEM_PEND_INTERR) {
        return kInProgressErr;
    } else {
        return kGeneralErr;
    } 
}

int rtos_get_semaphore_count(beken_semaphore_t *semaphore )
{
    uint32_t uwIntSave;
    uint32_t uwCount;

    if (OS_INT_ACTIVE) {
        return 0;
    }

	BK_ASSERT(semaphore);

    if (*semaphore == NULL) {
        return 0;
    }

    uwIntSave = LOS_IntLock();
    uwCount = ((LosSemCB *)*semaphore)->semCount;
    LOS_IntRestore(uwIntSave);

    return uwCount;
}

int rtos_set_semaphore(beken_semaphore_t *semaphore )
{
    UINT32 uwRet;

	BK_ASSERT(semaphore);

    if (*semaphore == NULL) {
        return kParamErr;
    }

    uwRet = LOS_SemPost(((LosSemCB *)*semaphore)->semID);
    if (uwRet == LOS_OK) {
        return kNoErr;
    } else if (uwRet == LOS_ERRNO_SEM_INVALID) {
        return kParamErr;
    } else {
        return kGeneralErr;
    }
}

bk_err_t rtos_deinit_semaphore(beken_semaphore_t *semaphore )
{
    UINT32 uwRet;

    if (OS_INT_ACTIVE) {
        return kGeneralErr;
    }

	BK_ASSERT(semaphore);

    if (*semaphore == NULL) {
        return kParamErr;
    }

    uwRet = LOS_SemDelete(((LosSemCB *)*semaphore)->semID);
    *semaphore = (beken_semaphore_t) NULL;
    if (uwRet == LOS_OK) {
        return kNoErr;
    } else if (uwRet == LOS_ERRNO_SEM_INVALID) {
        return kParamErr;
    } else {
        return kGeneralErr;
    }
}

void rtos_enter_critical(void)
{
}

void rtos_exit_critical(void)
{
}

bk_err_t rtos_init_mutex(beken_mutex_t *mutex)
{
    UINT32 uwRet;
    UINT32 uwMuxId;

    if (OS_INT_ACTIVE) {
        *mutex = NULL;
		goto init_exit;
    }

	BK_ASSERT(mutex);

    uwRet = LOS_MuxCreate(&uwMuxId);
    if (uwRet == LOS_OK) {
        *mutex = (beken_mutex_t)(GET_MUX(uwMuxId));
    } else {
        *mutex = (beken_mutex_t)NULL;
    }
	BK_ASSERT(LOS_OK == uwRet);

init_exit:
    if ( *mutex == NULL )
    {
        RTOS_LOGI("rtos_init_mutex:failed,mutex = %p\r\n", *mutex);
        return kGeneralErr;
    }

    RTOS_LOGD("rtos_init_mutex:success,mutex = %p\r\n", *mutex);
    return kNoErr;
}

bk_err_t rtos_trylock_mutex(beken_mutex_t *mutex)
{
    UINT32 uwRet;

    if (*mutex == NULL) {
        return kParamErr;
    }

	BK_ASSERT(mutex);

    uwRet = LOS_MuxPend(((LosMuxCB *)*mutex)->muxID, 0);
    if (uwRet == LOS_OK) {
        return kNoErr;
    } else if (uwRet == LOS_ERRNO_MUX_TIMEOUT) {
        return kTimeoutErr;
    } else if (uwRet == LOS_ERRNO_MUX_INVALID) {
        return kParamErr;
    } else {
        return kGeneralErr;
    }
}

bk_err_t rtos_lock_mutex(beken_mutex_t *mutex)
{
    UINT32 uwRet;

	BK_ASSERT(mutex);

    if (*mutex == NULL) {
        return kParamErr;
    }

    uwRet = LOS_MuxPend(((LosMuxCB *)*mutex)->muxID, LOS_WAIT_FOREVER);
    if (uwRet == LOS_OK) {
        return kNoErr;
    } else if (uwRet == LOS_ERRNO_MUX_TIMEOUT) {
        return kTimeoutErr;
    } else if (uwRet == LOS_ERRNO_MUX_INVALID) {
        return kParamErr;
    } else {
        return kGeneralErr;
    }
}

bk_err_t rtos_unlock_mutex(beken_mutex_t *mutex)
{
    UINT32 uwRet;

    if (*mutex == NULL) {
        return kParamErr;
    }

	BK_ASSERT(mutex);

    uwRet = LOS_MuxPost(((LosMuxCB *)*mutex)->muxID);
    if (uwRet == LOS_OK) {
        return kNoErr;
    } else {
        return kGeneralErr;
    }
}

bk_err_t rtos_deinit_mutex(beken_mutex_t *mutex)
{
    UINT32 uwRet;

    if (OS_INT_ACTIVE) {
        RTOS_LOGI("rtos_deinit_mutex:failed(int active), mutex = %p\r\n", *mutex);
        return kStateErr;
    }

	BK_ASSERT(mutex);

    if (*mutex == NULL) {
        RTOS_LOGI("rtos_deinit_mutex:failed(mutex is null).\r\n");
        return kParamErr;
    }

    rtos_unlock_mutex(mutex);

    uwRet = LOS_MuxDelete(((LosMuxCB *)*mutex)->muxID);
    if (uwRet == LOS_OK) {
        RTOS_LOGD("rtos_deinit_mutex:success, mutex = %p\r\n", *mutex);
        return kNoErr;
    } else if (uwRet == LOS_ERRNO_MUX_INVALID) {
        RTOS_LOGI("rtos_deinit_mutex:failed(invalid mux), mutex = %p\r\n", *mutex);
        return kParamErr;
    } else {
        RTOS_LOGI("rtos_deinit_mutex:failed(other), mutex = %p\r\n", *mutex);
        return kGeneralErr;
    }
}

bk_err_t rtos_init_queue( beken_queue_t *queue, const char* name, uint32_t msg_size, uint32_t msg_count )
{
    UINT32 uwRet;
    UINT32 uwQueueID;
	bk_err_t ret = kNoErr;

	BK_ASSERT(queue);

    if (0 == msg_count || 0 == msg_size || OS_INT_ACTIVE) {
        *queue = (beken_queue_t)NULL;
		ret = kParamErr;
		goto qinit_exit;
    }

    uwRet = LOS_QueueCreate((char *)name, (UINT16)msg_count, &uwQueueID, 0, (UINT16)msg_size);
	RTOS_LOGI("[debug]LOS_QueueCreate:%s:%d\r\n", name, uwRet);
    if (uwRet == LOS_OK) {
        *queue = (beken_queue_t)(GET_QUEUE_HANDLE(uwQueueID));
    } else {
        *queue = (beken_queue_t)NULL;
		ret = kNoResourcesErr;
    }
	BK_ASSERT(LOS_OK == uwRet);

qinit_exit:
    return ret;
}

bk_err_t rtos_push_to_queue( beken_queue_t *queue, void* msg_ptr, uint32_t timeout_ms )
{
    UINT32 uwRet;
    uint32_t timeout;
    uint32_t uwBufferSize;
    LosQueueCB *pstQueue = (LosQueueCB *)*queue;

	BK_ASSERT(queue);

    if(timeout_ms == BEKEN_WAIT_FOREVER)
        timeout = LOS_WAIT_FOREVER;
    else
        timeout = timeout_ms / ms_to_tick_ratio;    

    if (pstQueue == NULL || msg_ptr == NULL || ((OS_INT_ACTIVE) && (0 != timeout))) {
        return kParamErr;
    }
    if (pstQueue->queueSize < sizeof(uint32_t)) {
        return kParamErr;
    }
    uwBufferSize = (uint32_t)(pstQueue->queueSize - sizeof(uint32_t));
    uwRet = LOS_QueueWriteCopy((uint32_t)pstQueue->queueID, (void *)msg_ptr, uwBufferSize, timeout);
    if (uwRet == LOS_OK) {
        return kNoErr;
    } else if (uwRet == LOS_ERRNO_QUEUE_INVALID || uwRet == LOS_ERRNO_QUEUE_NOT_CREATE) {
        return kParamErr;
    } else if (uwRet == LOS_ERRNO_QUEUE_TIMEOUT) {
        return kTimeoutErr;
    } else {
        return kGeneralErr;
    }
}

bk_err_t rtos_push_to_queue_front( beken_queue_t *queue, void *msg_ptr, uint32_t timeout_ms )
{
    UINT32 uwRet;
    uint32_t timeout;
    uint32_t uwBufferSize;
    LosQueueCB *pstQueue = (LosQueueCB *)*queue;

	BK_ASSERT(queue);

    if(timeout_ms == BEKEN_WAIT_FOREVER)
        timeout = LOS_WAIT_FOREVER;
    else
        timeout = timeout_ms / ms_to_tick_ratio;    

    if (pstQueue == NULL || msg_ptr == NULL || ((OS_INT_ACTIVE) && (0 != timeout))) {
        return kParamErr;
    }
    if (pstQueue->queueSize < sizeof(uint32_t)) {
        return kParamErr;
    }
    uwBufferSize = (uint32_t)(pstQueue->queueSize - sizeof(uint32_t));
    uwRet = LOS_QueueWriteHeadCopy((uint32_t)pstQueue->queueID, (void *)msg_ptr, uwBufferSize, timeout);
    if (uwRet == LOS_OK) {
        return kNoErr;
    } else if (uwRet == LOS_ERRNO_QUEUE_INVALID || uwRet == LOS_ERRNO_QUEUE_NOT_CREATE) {
        return kParamErr;
    } else if (uwRet == LOS_ERRNO_QUEUE_TIMEOUT) {
        return kTimeoutErr;
    } else {
        return kGeneralErr;
    }
}

bk_err_t rtos_pop_from_queue( beken_queue_t *queue, void *msg_ptr, uint32_t timeout_ms )
{
    UINT32 uwRet;
    uint32_t timeout;
    UINT32 uwBufferSize;
    LosQueueCB *pstQueue = (LosQueueCB *)*queue;

	BK_ASSERT(queue);

    if(timeout_ms == BEKEN_WAIT_FOREVER)
        timeout = LOS_WAIT_FOREVER;
    else
        timeout = timeout_ms / ms_to_tick_ratio;    

    if (pstQueue == NULL || msg_ptr == NULL || ((OS_INT_ACTIVE) && (0 != timeout))) {
        return kParamErr;
    }

    uwBufferSize = (uint32_t)(pstQueue->queueSize - sizeof(uint32_t));
    uwRet = LOS_QueueReadCopy((uint32_t)pstQueue->queueID, msg_ptr, &uwBufferSize, timeout);
    if (uwRet == LOS_OK) {
        return kNoErr;
    } else if (uwRet == LOS_ERRNO_QUEUE_INVALID || uwRet == LOS_ERRNO_QUEUE_NOT_CREATE) {
        return kParamErr;
    } else if (uwRet == LOS_ERRNO_QUEUE_TIMEOUT) {
        return kTimeoutErr;
    } else {
        return kGeneralErr;
    }
}

bk_err_t rtos_deinit_queue(beken_queue_t *queue)
{
    UINT32 uwRet;
    LosQueueCB *pstQueue = (LosQueueCB *)*queue;

	BK_ASSERT(queue);

    if (pstQueue == NULL) {
        return kParamErr;
    }

    if (OS_INT_ACTIVE) {
        return kGeneralErr;
    }

    uwRet = LOS_QueueDelete((uint32_t)pstQueue->queueID);
    if (uwRet == LOS_OK) {
        return kNoErr;
    } else if (uwRet == LOS_ERRNO_QUEUE_NOT_FOUND || uwRet == LOS_ERRNO_QUEUE_NOT_CREATE) {
        return kParamErr;
    } else {
        return kUnknownErr;
    }
}

uint32_t _queue_get_capacity(beken_queue_t *queue)
{
    uint32_t capacity;
    LosQueueCB *pstQueue = (LosQueueCB *)*queue;

    if (pstQueue == NULL) {
        capacity = 0U;
    } else {
        capacity = pstQueue->queueLen;
    }

    return (capacity);
}

uint32_t _queue_get_count(beken_queue_t *queue)
{
    uint32_t count;
    UINTPTR uwIntSave;
    LosQueueCB *pstQueue = (LosQueueCB *)*queue;

	BK_ASSERT(queue);

    if (pstQueue == NULL) {
        count = 0U;
    } else {
        uwIntSave = LOS_IntLock();
        count = (uint32_t)(pstQueue->readWriteableCnt[OS_QUEUE_READ]);
        LOS_IntRestore(uwIntSave);
    }
    return count;
}

bool rtos_is_queue_full(beken_queue_t *queue)
{
	BK_ASSERT(queue);
    return ( _queue_get_capacity(queue) == _queue_get_count(queue) ) ? true : false;
}

bool rtos_is_queue_empty(beken_queue_t *queue)
{
	BK_ASSERT(queue);
    return ( 0 == _queue_get_count(queue) ) ? true : false;
}

static void timer_callback2( void *handle )
{
    beken2_timer_t *timer = (beken2_timer_t *) handle;

	if(BEKEN_MAGIC_WORD != timer->beken_magic)
	{
		return;
	}
    if ( timer->function )
    {
        timer->function( timer->left_arg, timer->right_arg );
    }
}

static void timer_callback1( void *handle )
{
    beken_timer_t *timer = (beken_timer_t*) handle;

    if ( timer->function )
    {
        timer->function( timer->arg);
    }
}

bk_err_t rtos_init_oneshot_timer( beken2_timer_t *timer, 
									uint32_t time_ms, 
									timer_2handler_t func,
									void* larg, 
									void* rarg )
{
    SWTMR_CTRL_S *pstSwtmr;
	bk_err_t ret = kNoErr;
    UINT32 usSwTmrID;
    UINT8 mode;

	timer->handle = NULL;
    if (NULL == func) {
		ret = kParamErr;
		goto tinit_exit;
    }

    mode = LOS_SWTMR_MODE_NO_SELFDELETE;
	timer->beken_magic = BEKEN_MAGIC_WORD;
	timer->function = func;
	timer->left_arg = larg;
	timer->right_arg = rarg;
	
#if (LOSCFG_BASE_CORE_SWTMR_ALIGN == 1)
    if (LOS_OK != LOS_SwtmrCreate(1, mode, (SWTMR_PROC_FUNC)timer_callback2, &usSwTmrID, (uint32_t)(UINTPTR)timer,
        osTimerRousesAllow, osTimerAlignIgnore)) {
        ret = kGeneralErr;
		goto tinit_exit;
    }
#else
    if (LOS_OK != LOS_SwtmrCreate(1, mode, (SWTMR_PROC_FUNC)func, &usSwTmrID, (uint32_t)(UINTPTR)larg)) {
        ret = kGeneralErr;
		goto tinit_exit;
    }
#endif

	pstSwtmr = (SWTMR_CTRL_S *)OS_SWT_FROM_SID(usSwTmrID);
    timer->handle = (void *)pstSwtmr;

	if(pstSwtmr){
		pstSwtmr->uwInterval = time_ms / ms_to_tick_ratio;
	}

tinit_exit:
    return ret;
}

bk_err_t rtos_start_oneshot_timer(beken2_timer_t *timer)
{
	return rtos_start_timer((beken_timer_t *)timer);
}

bk_err_t rtos_deinit_oneshot_timer(beken2_timer_t *timer)
{
	timer->beken_magic = 0;
	timer->left_arg = 0;
	timer->right_arg = 0;
	timer->function = NULL;
	
	return rtos_deinit_timer((beken_timer_t *)timer);
}

bk_err_t rtos_stop_oneshot_timer(beken2_timer_t *timer)
{
    return rtos_stop_timer((beken_timer_t *)timer);
}

bool rtos_is_oneshot_timer_init(beken2_timer_t *timer)
{
    return (NULL != timer->handle) ? true : false;
}

bool rtos_is_oneshot_timer_running(beken2_timer_t *timer)
{
    return rtos_is_timer_running((beken_timer_t *)timer);
}

bk_err_t rtos_oneshot_reload_timer(beken2_timer_t *timer)
{
    return rtos_start_timer((beken_timer_t *)timer);
}

bk_err_t rtos_oneshot_reload_timer_ex(beken2_timer_t *timer,
										uint32_t time_ms,
										timer_2handler_t function,
										void *larg,
										void *rarg)
{
	return kUnsupportedErr;
}

bk_err_t rtos_init_timer(beken_timer_t *timer, 
									uint32_t time_ms, 
									timer_handler_t func, 
									void* argument )
{
    SWTMR_CTRL_S *pstSwtmr;
	bk_err_t ret = kNoErr;
    UINT32 usSwTmrID;
    UINT8 mode;

	timer->handle = NULL;

    if (NULL == func) {
		ret = kParamErr;
		goto tinit_exit;
    }

    mode = LOS_SWTMR_MODE_PERIOD;
	timer->function = func;
	timer->arg = argument;
	
#if (LOSCFG_BASE_CORE_SWTMR_ALIGN == 1)
    if (LOS_OK != LOS_SwtmrCreate(1, mode, (SWTMR_PROC_FUNC)timer_callback1, &usSwTmrID, (uint32_t)(UINTPTR)timer,
        osTimerRousesAllow, osTimerAlignIgnore)) {
		ret = kGeneralErr;
        RTOS_LOGI("rtos_init_timer: Err LOSCFG_BASE_CORE_SWTMR_ALIGN == 1.\n");
		goto tinit_exit;
    }
#else
    if (LOS_OK != LOS_SwtmrCreate(1, mode, (SWTMR_PROC_FUNC)timer_callback1, &usSwTmrID, (uint32_t)(UINTPTR)timer)) {
		ret = kGeneralErr;
        RTOS_LOGI("rtos_init_timer: Err.\n");
		goto tinit_exit;
    }
#endif

    timer->handle = (void *)OS_SWT_FROM_SID(usSwTmrID);
	pstSwtmr = (SWTMR_CTRL_S *)timer->handle;


	if(pstSwtmr){
		pstSwtmr->uwInterval = time_ms / ms_to_tick_ratio;
	}

tinit_exit:
    return ret;
}

bk_err_t rtos_start_timer(beken_timer_t *timer)
{
    UINT32 uwRet;
    SWTMR_CTRL_S *pstSwtmr;

    if (NULL == timer) {
        return kParamErr;
    }

    UINTPTR intSave = LOS_IntLock();
    pstSwtmr = (SWTMR_CTRL_S *)timer->handle;
    uwRet = LOS_SwtmrStart(pstSwtmr->usTimerID);
    LOS_IntRestore(intSave);
    if (LOS_OK == uwRet) {
        return kNoErr;
    } else if (LOS_ERRNO_SWTMR_ID_INVALID == uwRet) {
        return kParamErr;
    } else {
        return kTimeoutErr;
    }
}

bk_err_t rtos_stop_timer(beken_timer_t *timer)
{
    UINT32 uwRet;
    SWTMR_CTRL_S *pstSwtmr;

    if (NULL == timer) {
        return kParamErr;
    }

	pstSwtmr = (SWTMR_CTRL_S *)timer->handle;
    uwRet = LOS_SwtmrStop(pstSwtmr->usTimerID);
    if (LOS_OK == uwRet) {
        return kNoErr;
    } else if (LOS_ERRNO_SWTMR_ID_INVALID == uwRet) {
        return kParamErr;
    } else {
        return kGeneralErr;
    }
}

bk_err_t rtos_reload_timer(beken_timer_t *timer)
{
    return rtos_start_timer(timer);
}

bk_err_t rtos_change_period(beken_timer_t *timer, uint32_t time_ms)
{
    UINT32 uwRet;
	UINTPTR intSave;
    SWTMR_CTRL_S *pstSwtmr;
	
    if (NULL == timer) {
        return kParamErr;
    }
	rtos_stop_timer(timer);

    intSave = LOS_IntLock();
    pstSwtmr = (SWTMR_CTRL_S *)timer->handle;
    pstSwtmr->uwInterval = time_ms / ms_to_tick_ratio;
    uwRet = LOS_SwtmrStart(pstSwtmr->usTimerID);
    LOS_IntRestore(intSave);
    if (LOS_OK == uwRet) {
        return kNoErr;
    } else if (LOS_ERRNO_SWTMR_ID_INVALID == uwRet) {
        return kParamErr;
    } else {
        return kGeneralErr;
    }
}

bk_err_t rtos_deinit_timer(beken_timer_t *timer)
{
    UINT32 uwRet;
    SWTMR_CTRL_S *pstSwtmr;

	pstSwtmr = (SWTMR_CTRL_S *)timer->handle;
    if (NULL == pstSwtmr) {
        return kParamErr;
    }

    uwRet = LOS_SwtmrDelete(pstSwtmr->usTimerID);
    if (LOS_OK == uwRet) {
		timer->handle = NULL;
		timer->arg = 0;
		timer->function = NULL;
        return kNoErr;
    } else if (LOS_ERRNO_SWTMR_ID_INVALID == uwRet) {
        return kParamErr;
    } else {
        return kGeneralErr;
    }
}

uint32_t rtos_get_timer_expiry_time(beken_timer_t *timer)
{
	uint32_t val;
    UINTPTR intSave;
    SWTMR_CTRL_S *pstSwtmr;

	intSave = LOS_IntLock();
    pstSwtmr = (SWTMR_CTRL_S *)timer->handle;
    val = pstSwtmr->uwInterval;
    LOS_IntRestore(intSave);
	
    return val;
}

uint32_t rtos_get_next_expire_time(void)
{
    return kGeneralErr;
}

uint32_t rtos_get_current_timer_count(void)
{
    return kGeneralErr;
}

bool rtos_is_timer_init(beken_timer_t *timer)
{
    return (NULL != timer->handle) ? true : false;
}

bool rtos_is_timer_running(beken_timer_t *timer)
{
    if (NULL == timer->handle) {
        return 0;
    }

    return (OS_SWTMR_STATUS_TICKING == ((SWTMR_CTRL_S *)timer->handle)->ucState);
}

bk_err_t rtos_init_event_flags( beken_event_flags_t* event_flags )
{
    __maybe_unused_var( event_flags );
	
    return kUnsupportedErr;
}

bk_err_t rtos_wait_for_event_flags( beken_event_flags_t* event_flags, uint32_t flags_to_wait_for, uint32_t* flags_set, beken_bool_t clear_set_flags, beken_event_flags_wait_option_t wait_option, uint32_t timeout_ms )
{
    __maybe_unused_var( event_flags );
    __maybe_unused_var( flags_to_wait_for );
    __maybe_unused_var( flags_set );
    __maybe_unused_var( clear_set_flags );
    __maybe_unused_var( wait_option );
    __maybe_unused_var( timeout_ms );

    return kUnsupportedErr;
}

bk_err_t rtos_set_event_flags( beken_event_flags_t* event_flags, uint32_t flags_to_set )
{
    __maybe_unused_var( event_flags );
    __maybe_unused_var( flags_to_set );
	
    return kUnsupportedErr;
}

bk_err_t rtos_deinit_event_flags( beken_event_flags_t* event_flags )
{
    __maybe_unused_var( event_flags );
	
    return kUnsupportedErr;
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
    return (beken_time_t) ( LOS_TickCountGet() * ms_to_tick_ratio );
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
bk_err_t rtos_delay_milliseconds( uint32_t num_ms )
{
    uint32_t ticks;

    ticks = num_ms / ms_to_tick_ratio;
    if (ticks == 0)
        ticks = 1;

    LOS_TaskDelay( ticks );

    return kNoErr;
}

/*-----------------------------------------------------------*/
void *beken_malloc( size_t xWantedSize )
{
	return (void *)LOS_MemAlloc(m_aucSysMem0, xWantedSize);;
}

void beken_free( void *pv )
{
	LOS_MemFree(m_aucSysMem0, pv);
}

void *beken_realloc( void *pv, size_t xWantedSize )
{
	return LOS_MemRealloc(m_aucSysMem0, pv, xWantedSize);
}
/*-----------------------------------------------------------*/

void rtos_start_scheduler(void)
{
	s_is_started_scheduler = true;
	LOS_Start();
}

bool rtos_is_scheduler_started(void)
{
	return s_is_started_scheduler;
}

#if (!CONFIG_ARCH_RISCV)
uint32_t rtos_get_cpsr(void)
{

	return platform_cpsr_content();

}
#endif

uint32_t rtos_disable_int(void)
{
	return port_disable_interrupts_flag();
}

void rtos_enable_int(uint32_t int_level)
{
	port_enable_interrupts_flag(int_level);
}
uint32_t rtos_disable_mie_int(void)
{
    return port_disable_mie_flag();
}

void rtos_enable_mie_int(uint32_t int_level)
{
	port_enable_mie_flag(int_level);
}

void rtos_stop_int(void)
{
	portDISABLE_INTERRUPTS();
}

void rtos_start_int(void)
{
	portENABLE_INTERRUPTS();
}

bool rtos_is_in_interrupt_context(void)
{
	return platform_is_in_interrupt_context();
}

void rtos_wait_for_interrupt(void)
{
	arch_wait_for_interrupt();
}

void rtos_dump_task_backtrace(beken_thread_t *task)
{
	// TODO
}

void rtos_dump_backtrace(void)
{
	rtos_dump_task_backtrace(NULL);
}

void rtos_dump_task_list(void)
{
	OsGetAllTskInfo();
}

size_t rtos_get_free_heap_size(void)
{	
	UINT32 OsMemGetFreeSize(VOID *pool);

	OsMemInfoPrint(m_aucSysMem0);
	LOS_MemFreeNodeShow(m_aucSysMem0);
	return OsMemGetFreeSize(m_aucSysMem0);
}
// eof

