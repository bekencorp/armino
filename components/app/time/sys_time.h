#ifndef _SYS_TIME_H_
#define _SYS_TIME_H_

#ifndef time_t
typedef long time_t;
#endif

/*
 * Structure returned by gettimeofday(2) system call,
 * and used in other calls.
 */
struct s_timeval {
	long	tv_sec;		/* seconds */
	long	tv_usec;	/* and microseconds */
};

/*
 * Structure defined by POSIX.1b to be like a timeval.
 */
struct s_timespec {
	time_t	tv_sec;		/* seconds */
	long	tv_nsec;	/* and nanoseconds */
};

struct timezone {
  int tz_minuteswest;	/* minutes west of Greenwich */
  int tz_dsttime;	/* type of dst correction */
};

struct tm {
  int tm_sec;			/* Seconds.	[0-60] (1 leap second) */
  int tm_min;			/* Minutes.	[0-59] */
  int tm_hour;			/* Hours.	[0-23] */
  int tm_mday;			/* Day.		[1-31] */
  int tm_mon;			/* Month.	[0-11] */
  int tm_year;			/* Year - 1900. */
  int tm_wday;			/* Day of week.	[0-6] */
  int tm_yday;			/* Days in year.[0-365]	*/
  int tm_isdst;			/* DST.		[-1/0/1]*/

  long int tm_gmtoff;		/* Seconds east of UTC.  */
  const char *tm_zone;		/* Timezone abbreviation.  */
};

int gettimeofday(struct s_timeval *tp, void *ignore);

#endif
