/**
 * RTOS测试用例
 */

#include "rtthread.h"
#include "sys_rtos.h"
#include <common/bk_kernel_err.h>
#include <os/os.h>

#include "finsh.h"

#define THREAD_TIMESLICE 5

#define RTOS_DEBUG   0
#if RTOS_DEBUG
#define RTOS_DBG(...)     rt_kprintf("[RTOS]"),rt_kprintf(__VA_ARGS__)
#else
#define RTOS_DBG(...)
#endif



/**
 * 消息队列测试
 */
static beken_queue_t mq;
static beken_thread_t queue_thread_revive;
static beken_thread_t queue_thread_send;
#define MSG_SIZE 12 /* 消息长度 */
#define MSG_NUM  10 /* 消息数量 */

static void thread_queue_recive_entry(void *parameter)
{
	rt_uint8_t buf[MSG_SIZE];

	while (1) {
		rt_memset(&buf[0], 0, sizeof(buf));
		if (rtos_pop_from_queue(&mq, &buf[0], 100) == kNoErr)
			rt_kprintf("recive thread:recive msg = %s\n", &buf);
		else
			rt_kprintf("recive thread:failed\n");
		rtos_delay_milliseconds(100);
	}
}

static void thread_queue_send_entry(void *parameter)
{
	rt_uint8_t buf[MSG_SIZE];

	while (1) {
		rt_memset(&buf[0], 0, sizeof(buf));
		for (int i = 0; i < MSG_SIZE - 1; i++)
			buf[i] = 'a' + i;
		if (!rtos_is_queue_full(&mq)) {
			rt_kprintf("send thread:%s\n", &buf[0]);
			rtos_push_to_queue(&mq, &buf[0], 0);
		} else {
			rt_kprintf("send thread: queue full\n");
			rtos_delay_milliseconds(10000);
			return;
		}

		// rt_memset(&buf[0], 0 , sizeof(buf));
	}
}

int msg_queue_simple_init(void)
{
	bk_err_t ret;

	/* 初始化消息队列 */
	ret = rtos_init_queue(&mq, "msg_queue", MSG_SIZE, MSG_NUM);
	BK_ASSERT(ret == kNoErr);

	/* 创建recive线程 */
	ret = rtos_create_thread(&queue_thread_revive, 24, "recv_thread", thread_queue_recive_entry, 1024, RT_NULL);
	BK_ASSERT(ret == kNoErr);

	/* 创建send线程 */
	rtos_create_thread(&queue_thread_send, 25, "send_thread", thread_queue_send_entry, 1024, RT_NULL);
	BK_ASSERT(ret == kNoErr);

	return kNoErr;
}

// MSH_CMD_EXPORT(msg_queue_simple_init, msq queue simple);


/**
 * 定时器测试
 */
// static beken_thread_t one_shot_tim;
static beken2_timer_t one_shot;
static void one_shot_timeout(void *Larg, void *Rarg)
{
	rt_kprintf("enter one shot time out !!! \n");
}

int one_shot_time_simple_init(void)
{
	bk_err_t ret;
	ret = rtos_init_oneshot_timer(&one_shot, 10000, one_shot_timeout, RT_NULL, RT_NULL);
	BK_ASSERT(ret == kNoErr);

	ret = rtos_start_oneshot_timer(&one_shot);
	BK_ASSERT(ret == kNoErr);

	return kNoErr;
}

// MSH_CMD_EXPORT(one_shot_time_simple_init, one shot time simple);


