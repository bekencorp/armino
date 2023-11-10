#ifndef __TIME_H__
#define __TIME_H__

#include "sys/time.h"


time_t mktime(struct tm * const t);

char *asctime(const struct tm *timeptr);
char *ctime(const time_t *timep);
struct tm* localtime(const time_t* t);

char *asctime_r(const struct tm *t, char *buf);
struct tm *gmtime_r(const time_t *timep, struct tm *r);
struct tm* localtime_r(const time_t* t, struct tm* r);

int datetime_set(time_t       sec);
int datetime_get(struct tm *t);
int datetime_get_with_zone(struct tm *t, int zone);

time_t timestamp_get(void);


#endif
