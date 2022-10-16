#include "time/time.h"
#include <common/bk_include.h>
#include "bk_arm_arch.h"
#include <os/os.h>


/* days per month -- nonleap! */
const short __spm[13] =
  { 0,
    (31),
    (31+28),
    (31+28+31),
    (31+28+31+30),
    (31+28+31+30+31),
    (31+28+31+30+31+30),
    (31+28+31+30+31+30+31),
    (31+28+31+30+31+30+31+31),
    (31+28+31+30+31+30+31+31+30),
    (31+28+31+30+31+30+31+31+30+31),
    (31+28+31+30+31+30+31+31+30+31+30),
    (31+28+31+30+31+30+31+31+30+31+30+31),
  };
static long  timezone = 0;//(8*60*60);//east 8 zone
static const char days[] = "Sun Mon Tue Wed Thu Fri Sat ";
static const char months[] = "Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec ";

/* seconds per day */
#define SPD 24*60*60

int __isleap(int year)
{
	/* every fourth year is a leap year except for century years that are
	 * not divisible by 400. */
	/*  return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)); */
	return (!(year % 4) && ((year % 100) || !(year % 400)));
}

struct tm *gmtime_r(const time_t *timep, struct tm *r)
{
	time_t i;
	register time_t work = *timep % (SPD);
	r->tm_sec = work % 60;
	work /= 60;
	r->tm_min = work % 60;
	r->tm_hour = work / 60;
	work = *timep / (SPD);
	r->tm_wday = (4 + work) % 7;
	for (i = 1970;; ++i)
	{
		register time_t k = __isleap(i) ? 366 : 365;
		if (work >= k)
			work -= k;
		else
			break;
	}
	r->tm_year = i - 1900;
	r->tm_yday = work;

	r->tm_mday = 1;
	if (__isleap(i) && (work > 58))
	{
		if (work == 59)
			r->tm_mday = 2; /* 29.2. */
		work -= 1;
	}

	for (i = 11; i && (__spm[i] > work); --i)
		;
	r->tm_mon = i;
	r->tm_mday += work - __spm[i];
	return r;
}

struct tm* localtime_r(const time_t* t, struct tm* r)
{
	struct s_timeval time = {0};
	gettimeofday(&time, NULL);

	time_t tmp = time.tv_sec + *t + timezone;
	return gmtime_r(&tmp, r);
}

struct tm* localtime(const time_t* t)
{
	static struct tm tmp;
	return localtime_r(t, &tmp);
}

time_t mktime(struct tm * const t)
{
	register time_t day;
	register time_t i;
	register time_t years = t->tm_year - 70;

	if (t->tm_sec > 60)
	{
		t->tm_min += t->tm_sec / 60;
		t->tm_sec %= 60;
	}
	if (t->tm_min > 60)
	{
		t->tm_hour += t->tm_min / 60;
		t->tm_min %= 60;
	}
	if (t->tm_hour > 24)
	{
		t->tm_mday += t->tm_hour / 24;
		t->tm_hour %= 24;
	}
	if (t->tm_mon > 12)
	{
		t->tm_year += t->tm_mon / 12;
		t->tm_mon %= 12;
	}
	while (t->tm_mday > __spm[1 + t->tm_mon])
	{
		if (t->tm_mon == 1 && __isleap(t->tm_year + 1900))
		{
			--t->tm_mday;
		}
		t->tm_mday -= __spm[t->tm_mon];
		++t->tm_mon;
		if (t->tm_mon > 11)
		{
			t->tm_mon = 0;
			++t->tm_year;
		}
	}

	if (t->tm_year < 70)
		return (time_t) -1;

	/* Days since 1970 is 365 * number of years + number of leap years since 1970 */
	day = years * 365 + (years + 1) / 4;

	/* After 2100 we have to substract 3 leap years for every 400 years
	 This is not intuitive. Most mktime implementations do not support
	 dates after 2059, anyway, so we might leave this out for it's
	 bloat. */
	if (years >= 131)
	{
		years -= 131;
		years /= 100;
		day -= (years >> 2) * 3 + 1;
		if ((years &= 3) == 3)
			years--;
		day -= years;
	}

	day += t->tm_yday = __spm[t->tm_mon] + t->tm_mday - 1 +
			(__isleap(t->tm_year + 1900) & (t->tm_mon > 1));

	/* day is now the number of days since 'Jan 1 1970' */
	i = 7;
	t->tm_wday = (day + 4) % i; /* Sunday=0, Monday=1, ..., Saturday=6 */

	i = 24;
	day *= i;
	i = 60;
	return ((day + t->tm_hour) * i + t->tm_min) * i + t->tm_sec;
}

static void num2str(char *c, int i)
{
	c[0] = i / 10 + '0';
	c[1] = i % 10 + '0';
}

char *asctime_r(const struct tm *t, char *buf)
{
	/* "Wed Jun 30 21:49:08 1993\n" */
	*(int*) buf = *(int*) (days + (t->tm_wday << 2));
	*(int*) (buf + 4) = *(int*) (months + (t->tm_mon << 2));
	num2str(buf + 8, t->tm_mday);
	if (buf[8] == '0')
		buf[8] = ' ';
	buf[10] = ' ';
	num2str(buf + 11, t->tm_hour);
	buf[13] = ':';
	num2str(buf + 14, t->tm_min);
	buf[16] = ':';
	num2str(buf + 17, t->tm_sec);
	buf[19] = ' ';
	num2str(buf + 20, (t->tm_year + 1900) / 100);
	num2str(buf + 22, (t->tm_year + 1900) % 100);
	buf[24] = '\n';
	return buf;
}

char *asctime(const struct tm *timeptr)
{
	static char buf[25];
	return asctime_r(timeptr, buf);
}

char *ctime(const time_t *timep)
{
	return asctime(localtime(timep));
}


int gettimeofday(struct s_timeval *tp, void *ignore)
{
	#include <driver/hal/hal_aon_rtc_types.h>
	extern uint64_t bk_aon_rtc_get_current_tick(aon_rtc_id_t id);
	aon_rtc_unit_t aon_rtc_id = AON_RTC_ID_1;
	long time = 0;

	(void)ignore;

	uint64_t tick = bk_aon_rtc_get_current_tick(aon_rtc_id);
	//os_printf("bk_aon_rtc_get_current_tick:%lld\r\n",tick);
	if (tp != NULL)
	{
		tp->tv_sec = tick/32/1000;
		tp->tv_usec = 0;
		//os_printf("sys run time second:%ld\r\n",tp->tv_sec);

		time = tp->tv_sec;

		return time;
	}

	return 0;
}


#define DEFAULT_YEAR 	2022
#define DEFAULT_MONTH 	10
#define DEFAULT_DAY 	12
#define DEFAULT_HOUR 	10
#define DEFAULT_MIN		10
#define DEFAULT_SEC 	10

int get_curtime_str(char *buf,uint8_t opera)
{
	struct tm time_default = {0};
	time_default.tm_year = DEFAULT_YEAR - 1900;
	time_default.tm_mon = DEFAULT_MONTH - 1;
	time_default.tm_mday = DEFAULT_DAY;
	time_default.tm_hour = DEFAULT_HOUR;
	time_default.tm_min = DEFAULT_MIN;
	time_default.tm_sec = DEFAULT_SEC;

	time_t seconds = mktime(&time_default);
	//os_printf("seconds:%lld\r\n",seconds);

	struct tm *ptm = localtime(&seconds);
	//os_printf("year:%d,mon:%d,mday:%d,hour:%d,minute:%d,sec:%d\r\n",
	//	ptm->tm_year,ptm->tm_mon,ptm->tm_mday,ptm->tm_hour,ptm->tm_min,ptm->tm_sec);

	num2str(buf, (ptm->tm_year+1900) / 100);
	num2str(buf + 2, (ptm->tm_year+1900) % 100);
	num2str(buf + 4, ptm->tm_mon+1);
	num2str(buf + 6, ptm->tm_mday);
	num2str(buf + 8, ptm->tm_hour);
	num2str(buf + 10, ptm->tm_min);
	num2str(buf + 12, ptm->tm_sec);
	buf[14] = 0;

	return 0;
}

