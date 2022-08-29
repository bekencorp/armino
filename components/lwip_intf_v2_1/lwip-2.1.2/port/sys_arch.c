/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/* lwIP includes. */
#include "lwip/debug.h"
#include "lwip/def.h"
#include "lwip/sys.h"
#include "lwip/mem.h"
#include "lwip/stats.h"
#include "sys_rtos.h"
#include "lwip/timeouts.h"
#include <os/os.h>
#include <components/log.h>

#define CFG_ENABLE_LWIP_MUTEX      1

#if CFG_ENABLE_LWIP_MUTEX
static sys_mutex_t sys_arch_mutex;
#endif

/*-----------------------------------------------------------------------------------*/
err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
	bk_err_t ret;

	if (size > 0)
		ret = rtos_init_queue(mbox, NULL, sizeof( void * ), size);
	else
		ret = rtos_init_queue(mbox, NULL, sizeof( void * ), archMESG_QUEUE_LENGTH);

#if SYS_STATS
      ++lwip_stats.sys.mbox.used;
      if (lwip_stats.sys.mbox.max < lwip_stats.sys.mbox.used) {
         lwip_stats.sys.mbox.max = lwip_stats.sys.mbox.used;
	  }
#endif /* SYS_STATS */

    if (kNoErr != ret)
        return ERR_MEM;

    return ERR_OK;
}

/*-----------------------------------------------------------------------------------*/
/*
  Deallocates a mailbox. If there are messages still present in the
  mailbox when the mailbox is deallocated, it is an indication of a
  programming error in lwIP and the developer should be notified.
*/
void sys_mbox_free(sys_mbox_t *mbox)
{
	rtos_deinit_queue(mbox);

#if SYS_STATS
     --lwip_stats.sys.mbox.used;
#endif /* SYS_STATS */
}

/*-----------------------------------------------------------------------------------*/
//   Posts the "msg" to the mailbox.
void sys_mbox_post(sys_mbox_t *mbox, void *data)
{
	bk_err_t ret;

	ret = rtos_push_to_queue(mbox, &data, BEKEN_WAIT_FOREVER);
    if (ret != kNoErr) {
        LWIP_DEBUGF(SYS_DEBUG, ("%s: sys_mbox_post error %u\n", __FUNCTION__, ret));
    }
}


/*-----------------------------------------------------------------------------------*/
//   Try to post the "msg" to the mailbox.
err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
	err_t result;
	bk_err_t ret;

	ret = rtos_push_to_queue(mbox, &msg, 0);
   if (kNoErr == ret)
   {
      result = ERR_OK;
   }
   else
   	{
      // could not post, queue must be full
      result = ERR_MEM;

#if SYS_STATS
      lwip_stats.sys.mbox.err++;
#endif /* SYS_STATS */

   }

   return result;
}

/*-----------------------------------------------------------------------------------*/
/*
  Blocks the thread until a message arrives in the mailbox, but does
  not block the thread longer than "timeout" milliseconds (similar to
  the sys_arch_sem_wait() function). The "msg" argument is a result
  parameter that is set by the function (i.e., by doing "*msg =
  ptr"). The "msg" parameter maybe NULL to indicate that the message
  should be dropped.

  The return values are the same as for the sys_arch_sem_wait() function:
  Number of milliseconds spent waiting or SYS_ARCH_TIMEOUT if there was a
  timeout.

  Note that a function with a similar name, sys_mbox_fetch(), is
  implemented by lwIP.
*/
u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
	void *dummyptr;
	beken_time_t StartTime, EndTime, Elapsed;

	beken_time_get_time(&StartTime);

	if ( NULL == msg )
	{
		msg = &dummyptr;
	}

	if ( timeout != 0 )
	{
		if ( kNoErr == rtos_pop_from_queue(mbox, &(*msg), timeout) )
		{
			beken_time_get_time(&EndTime);
			Elapsed = (EndTime - StartTime) * 1;

			return ( Elapsed );
		}
		else // timed out blocking for message
		{
			*msg = NULL;

			return SYS_ARCH_TIMEOUT;
		}
	}
	else // block forever for a message.
	{
		rtos_pop_from_queue(mbox, &(*msg), BEKEN_WAIT_FOREVER);
		beken_time_get_time(&EndTime);
		Elapsed = (EndTime - StartTime) * 1;

		return ( Elapsed ); // return time blocked TODO test
	}
}

/*-----------------------------------------------------------------------------------*/
/*
  Similar to sys_arch_mbox_fetch, but if message is not ready immediately, we'll
  return with SYS_MBOX_EMPTY.  On success, 0 is returned.
*/
u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
void *dummyptr;

	if ( msg == NULL )
	{
		msg = &dummyptr;
	}

   if ( kNoErr == rtos_pop_from_queue(mbox, &(*msg), 0) )
   {
      return ERR_OK;
   }
   else
   {
      return SYS_MBOX_EMPTY;
   }
}
/*----------------------------------------------------------------------------------*/
int sys_mbox_valid(sys_mbox_t *mbox)
{
  if (*mbox == SYS_MBOX_NULL)
    return 0;
  else
    return 1;
}
/*-----------------------------------------------------------------------------------*/
void sys_mbox_set_invalid(sys_mbox_t *mbox)
{
  *mbox = SYS_MBOX_NULL;
}

/*-----------------------------------------------------------------------------------*/
//  Creates a new semaphore. The "count" argument specifies
//  the initial state of the semaphore.
err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
	bk_err_t ret;

	ret = rtos_init_semaphore_adv(sem, 0xff, count);
	if(kNoErr != ret)
		return ERR_ARG;

#if SYS_STATS
	++lwip_stats.sys.sem.used;
 	if (lwip_stats.sys.sem.max < lwip_stats.sys.sem.used) {
		lwip_stats.sys.sem.max = lwip_stats.sys.sem.used;
	}
#endif /* SYS_STATS */

	return ERR_OK;
}

/*-----------------------------------------------------------------------------------*/
/*
  Blocks the thread while waiting for the semaphore to be
  signaled. If the "timeout" argument is non-zero, the thread should
  only be blocked for the specified time (measured in
  milliseconds).

  If the timeout argument is non-zero, the return value is the number of
  milliseconds spent waiting for the semaphore to be signaled. If the
  semaphore wasn't signaled within the specified time, the return value is
  SYS_ARCH_TIMEOUT. If the thread didn't have to wait for the semaphore
  (i.e., it was already signaled), the function may return zero.

  Notice that lwIP implements a function with a similar name,
  sys_sem_wait(), that uses the sys_arch_sem_wait() function.
*/
u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
	bk_err_t ret;
	beken_time_t StartTime, EndTime, Elapsed;

	beken_time_get_time(&StartTime);

	if(	timeout != 0)
	{
		ret = rtos_get_semaphore(sem, timeout);
		if( kNoErr == ret )
		{
			beken_time_get_time(&EndTime);
			Elapsed = (EndTime - StartTime) * 1;

			return (Elapsed); // return time blocked TODO test
		}
		else
		{
			return SYS_ARCH_TIMEOUT;
		}
	}
	else // must block without a timeout
	{
		ret = rtos_get_semaphore(sem, BEKEN_WAIT_FOREVER);
		if( kNoErr != ret )
			return SYS_ARCH_TIMEOUT;

		beken_time_get_time(&EndTime);
		Elapsed = (EndTime - StartTime) * 1;

		return ( Elapsed ); // return time blocked
	}
}

/*-----------------------------------------------------------------------------------*/
// Signals a semaphore
void sys_sem_signal(sys_sem_t *sem)
{
	rtos_set_semaphore(sem);
}

/*-----------------------------------------------------------------------------------*/
// Deallocates a semaphore
void sys_sem_free(sys_sem_t *sem)
{
#if SYS_STATS
      --lwip_stats.sys.sem.used;
#endif /* SYS_STATS */

	rtos_deinit_semaphore(sem);
}
/*-----------------------------------------------------------------------------------*/
int sys_sem_valid(sys_sem_t *sem)
{
  if (*sem == SYS_SEM_NULL)
    return 0;
  else
    return 1;
}

/*-----------------------------------------------------------------------------------*/
void sys_sem_set_invalid(sys_sem_t *sem)
{
  *sem = SYS_SEM_NULL;
}

/*-----------------------------------------------------------------------------------*/
err_t sys_mutex_trylock(sys_mutex_t *pxMutex)
{
	bk_err_t ret;

	ret = rtos_trylock_mutex(pxMutex);
	if (kNoErr == ret)
		return 0;
	else
		return -1;
}

/*-----------------------------------------------------------------------------------*/
// Initialize sys arch
void sys_init(void)
{
#if CFG_ENABLE_LWIP_MUTEX
	sys_mutex_new(&sys_arch_mutex);
#endif
}

/*-----------------------------------------------------------------------------------*/
                                      /* Mutexes*/
/*-----------------------------------------------------------------------------------*/
err_t sys_mutex_new(sys_mutex_t *mutex)
{
	bk_err_t ret;

	ret = rtos_init_mutex(mutex);
	if(kNoErr != ret)
		return ERR_VAL;

#if SYS_STATS
	++lwip_stats.sys.mutex.used;
 	if (lwip_stats.sys.mutex.max < lwip_stats.sys.mutex.used) {
		lwip_stats.sys.mutex.max = lwip_stats.sys.mutex.used;
	}
#endif /* SYS_STATS */

    return ERR_OK;
}
/*-----------------------------------------------------------------------------------*/
/* Deallocate a mutex*/
void sys_mutex_free(sys_mutex_t *mutex)
{
#if SYS_STATS
      --lwip_stats.sys.mutex.used;
#endif /* SYS_STATS */

	rtos_deinit_mutex(mutex);
}

void sys_mutex_lock(sys_mutex_t *mutex)
{
	rtos_lock_mutex(mutex);
}

void sys_mutex_unlock(sys_mutex_t *mutex)
{
	rtos_unlock_mutex(mutex);
}

/*-----------------------------------------------------------------------------------*/
// TODO
/*-----------------------------------------------------------------------------------*/
/*
  Starts a new thread with priority "prio" that will begin its execution in the
  function "thread()". The "arg" argument will be passed as an argument to the
  thread() function. The id of the new thread is returned. Both the id and
  the priority are system dependent.
*/
sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread , void *arg, int stacksize, int prio)
{
	sys_thread_t CreatedTask;
	bk_err_t result;

	result = rtos_create_thread(&CreatedTask, prio, name, thread, stacksize * sizeof(uint32_t), arg);
	if(result == kNoErr)
	{
	   return CreatedTask;
	}
	else
	{
	   return NULL;
	}
}

int sys_thread_delete(sys_thread_t pid)
{
	bk_err_t ret;

	ret = rtos_delete_thread(&pid);
	if(kNoErr == ret)
	{
		return ERR_OK;
	}
	else
	{
		return ERR_ARG;
	}
}

/*
  This optional function does a "fast" critical region protection and returns
  the previous protection level. This function is only called during very short
  critical regions. An embedded system which supports ISR-based drivers might
  want to implement this function by disabling interrupts. Task-based systems
  might want to implement this by using a mutex or disabling tasking. This
  function should support recursive calls from the same task or interrupt. In
  other words, sys_arch_protect() could be called while already protected. In
  that case the return value indicates that it is already protected.

  sys_arch_protect() is only required if your port is supporting an operating
  system.
*/
sys_prot_t sys_arch_protect(void)
{
#if CFG_ENABLE_LWIP_MUTEX
	sys_mutex_lock(&sys_arch_mutex);

	return 0;
#else
	return port_disable_interrupts_flag();
#endif
}

/*
  This optional function does a "fast" set of critical region protection to the
  value specified by pval. See the documentation for sys_arch_protect() for
  more information. This function is only required if your port is supporting
  an operating system.
*/
void sys_arch_unprotect(sys_prot_t pval)
{
#if CFG_ENABLE_LWIP_MUTEX
	(void)pval;
	sys_mutex_unlock(&sys_arch_mutex);
#else
	port_enable_interrupts_flag(pval);
#endif
}

/*
 * Prints an assertion messages and aborts execution.
 */
void sys_assert( const char *msg )
{
	(void) msg;

	/*FSL:only needed for debugging*/
	os_printf(msg);
	os_printf("\n\r");

	rtos_enter_critical();

	for(;;)
	;
}

u32_t sys_now(void)
{
	beken_time_t now_tick = 0;

	beken_time_get_time(&now_tick);
	return now_tick * beken_ms_per_tick();
}

u32_t sys_jiffies(void)
{
	beken_time_t now_tick = 0;

	beken_time_get_time(&now_tick);
	return now_tick;
}

void sys_arch_msleep(int ms)
{
	rtos_delay_milliseconds(ms);
}

// eof
