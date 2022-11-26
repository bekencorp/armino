#ifndef _NTP_H_
#define _NTP_H_

#include <time/sys_time.h>

/**
 * Get the UTC time from NTP server
 *
 * @note this function is not reentrant
 *
 * @return >0: success, current UTC time
 *         =0: get failed
 */
time_t ntp_get_time(void);

/**
 * Get the local time from NTP server
 *
 * @return >0: success, current local time, offset timezone by NTP_TIMEZONE
 *         =0: get failed
 */
time_t ntp_get_local_time(void);

/**
 * Sync current local time to RTC by NTP
 *
 * @return >0: success, current local time, offset timezone by NTP_TIMEZONE
 *         =0: sync failed
 */
time_t ntp_sync_to_rtc(void);

#endif /* _NTP_H_ */
