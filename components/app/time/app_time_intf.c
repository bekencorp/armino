#include <common/bk_include.h>
#include "bk_arm_arch.h"
#include <os/os.h>
#include <components/app_time_intf.h>
#include "time/time.h"
#include <time/ntp.h>


#define TAG "app_time_intf"

#define LOGI(...) BK_LOGI(TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(TAG, ##__VA_ARGS__)

#define NTP_SYNC_CYCLE				   (1*60*60*1000)//3600000ms

#define DEFAULT_YEAR 	2021
#define DEFAULT_MONTH 	10
#define DEFAULT_DAY 	12
#define DEFAULT_HOUR 	10
#define DEFAULT_MIN		10
#define DEFAULT_SEC 	10

static void num2str(char *c, int i)
{
	c[0] = i / 10 + '0';
	c[1] = i % 10 + '0';
}

static void ntp_sync_thread_enrty(void *param)
{
	rtos_delay_milliseconds(10000);

    while (1)
    {
        time_t cur_time = ntp_sync_to_rtc();
		if (cur_time)
		{
			LOGI("\r\nGet local time from NTP server: %s", ctime(&cur_time));
			rtos_delay_milliseconds(NTP_SYNC_CYCLE);
		}
		else
		{
			rtos_delay_milliseconds(10000);
		}
    }
}

static int soft_rtc_init(void)
{
	struct tm time_default = {0};

	time_default.tm_year = DEFAULT_YEAR - 1900;
	time_default.tm_mon = DEFAULT_MONTH - 1;
	time_default.tm_mday = DEFAULT_DAY;
	time_default.tm_hour = DEFAULT_HOUR;
	time_default.tm_min = DEFAULT_MIN;
	time_default.tm_sec = DEFAULT_SEC;
	time_t seconds = mktime(&time_default);

	return datetime_set(seconds);
}

int app_time_rtc_ntp_sync_init(void)
{
    static bool init_ok = 0;

    if (init_ok)
    {
        return 0;
    }

	int ret = rtos_create_thread(NULL,
	                         5,
	                         "ntp_sync_task",
	                         (beken_thread_function_t)ntp_sync_thread_enrty,
	                         4 * 1024,
	                         NULL);
	if (ret)
	{
		LOGI("ntp_sync_thread_enrty create fail!\r\n");
		return -1;
	}

	//soft_rtc_init();

	LOGI("rtc_ntp_sync_init complete\r\n");

    init_ok = 1;

	return 0;
}

int get_curtime_str(char *buf,unsigned char opera)
{
	struct tm s_tm_default = {0};
	s_tm_default.tm_year = DEFAULT_YEAR - 1900;
	s_tm_default.tm_mon = DEFAULT_MONTH - 1;
	s_tm_default.tm_mday = DEFAULT_DAY;
	s_tm_default.tm_hour = DEFAULT_HOUR;
	s_tm_default.tm_min = DEFAULT_MIN;
	s_tm_default.tm_sec = DEFAULT_SEC;

	struct tm s_tm = {0};
	datetime_get(&s_tm);
	if( (s_tm_default.tm_year == s_tm.tm_year) && \
		(s_tm_default.tm_mon == s_tm.tm_mon) && \
		(s_tm_default.tm_mday == s_tm.tm_mday) )
	{
		LOGI("ntp unconnected! use default timer\r\n");
	}

	num2str(buf, (s_tm.tm_year+1900) / 100);
	num2str(buf + 2, (s_tm.tm_year+1900) % 100);
	num2str(buf + 4, s_tm.tm_mon+1);
	num2str(buf + 6, s_tm.tm_mday);
	num2str(buf + 8, s_tm.tm_hour);
	num2str(buf + 10, s_tm.tm_min);
	num2str(buf + 12, s_tm.tm_sec);
	buf[14] = 0;

	LOGI("get_curtime_str:%s\r\n",buf);

	return 0;
}

/*@buf:output year mon mday hour min sec*/
int app_time_timestr_get(char *buf,unsigned char len)
{
	if(NULL == buf)
	{
		LOGI("buf NULL error!\r\n");
		return -1;
	}

	if(len < 15)
	{
		LOGI("len too small!\r\n");
		return -1;
	}

	return get_curtime_str(buf,1);
}

int app_time_datetime_get(user_datetime_t *pdt)
{
	struct tm s_tm_default = {0};
	s_tm_default.tm_year = DEFAULT_YEAR - 1900;
	s_tm_default.tm_mon = DEFAULT_MONTH - 1;
	s_tm_default.tm_mday = DEFAULT_DAY;
	s_tm_default.tm_hour = DEFAULT_HOUR;
	s_tm_default.tm_min = DEFAULT_MIN;
	s_tm_default.tm_sec = DEFAULT_SEC;

	struct tm s_tm = {0};
	datetime_get(&s_tm);
	if( (s_tm_default.tm_year == s_tm.tm_year) && \
		(s_tm_default.tm_mon == s_tm.tm_mon) && \
		(s_tm_default.tm_mday == s_tm.tm_mday) )
	{
		LOGI("ntp unconnected! use default timer\r\n");
	}

	pdt->year = s_tm.tm_year+1900;
	pdt->month = s_tm.tm_mon+1;
	pdt->mday = s_tm.tm_mday;
	pdt->hour = s_tm.tm_hour;
	pdt->minute = s_tm.tm_min;
	pdt->second = s_tm.tm_sec;
	pdt->wday = s_tm.tm_wday;

	LOGI("%d-%d-%d  %d:%d:%d  %d\r\n",pdt->year,pdt->month,pdt->mday,pdt->hour,pdt->minute,pdt->second,pdt->wday);

	return 0;
}

