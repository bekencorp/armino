/*

  This file is a part of JRTPLIB
  Copyright (c) 1999-2017 Jori Liesenborgs

  Contact: jori.liesenborgs@gmail.com

  This library was developed at the Expertise Centre for Digital Media
  (http://www.edm.uhasselt.be), a research center of the Hasselt University
  (http://www.uhasselt.be). The library is based upon work done for 
  my thesis at the School for Knowledge Technology (Belgium/The Netherlands).

  Permission is hereby granted, free of charge, to any person obtaining a
  copy of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  IN THE SOFTWARE.

*/

#include "rtpconfig.h"
#include "rtptimeutilities.h"
#ifdef RTPDEBUG
	#include <iostream>
#endif // RTPDEBUG
#include "os/os.h"

namespace jrtplib
{

RTPTimeInitializerObject::RTPTimeInitializerObject()
{
#ifdef RTPDEBUG
	std::cout << "RTPTimeInitializer: Initializing RTPTime::CurrentTime()" << std::endl;
#endif // RTPDEBUG
	RTPTime curtime = RTPTime::CurrentTime();
	JRTPLIB_UNUSED(curtime);
	dummy = -1;
}

RTPTimeInitializerObject timeinit;

//#ifdef RTP_HAVE_QUERYPERFORMANCECOUNTER
#if 0
inline uint64_t RTPTime::CalculateMicroseconds(uint64_t performancecount,uint64_t performancefrequency)
{
	uint64_t f = performancefrequency;
	uint64_t a = performancecount;
	uint64_t b = a/f;
	uint64_t c = a%f; // a = b*f+c => (a*1000000)/f = b*1000000+(c*1000000)/f

	return b*C1000000+(c*C1000000)/f;
}

inline RTPTime RTPTime::CurrentTime()
{
	rtos_get_time(void)
	static int inited = 0;
	static uint64_t microseconds, initmicroseconds;
	static LARGE_INTEGER performancefrequency;

	uint64_t emulate_microseconds, microdiff;
	SYSTEMTIME systemtime;
	FILETIME filetime;

	LARGE_INTEGER performancecount;

	QueryPerformanceCounter(&performancecount);
    
	if(!inited){
		inited = 1;
		QueryPerformanceFrequency(&performancefrequency);
		GetSystemTime(&systemtime);
		SystemTimeToFileTime(&systemtime,&filetime);
		microseconds = ( ((uint64_t)(filetime.dwHighDateTime) << 32) + (uint64_t)(filetime.dwLowDateTime) ) / (uint64_t)10;
		microseconds-= CEPOCH; // EPOCH
		initmicroseconds = CalculateMicroseconds(performancecount.QuadPart, performancefrequency.QuadPart);
	}
    
	emulate_microseconds = CalculateMicroseconds(performancecount.QuadPart, performancefrequency.QuadPart);

	microdiff = emulate_microseconds - initmicroseconds;

	double t = 1e-6*(double)(microseconds + microdiff);
	return RTPTime(t);
}

inline void RTPTime::Wait(const RTPTime &delay)
{
	if (delay.m_t <= 0)
		return;

	uint64_t sec = (uint64_t)delay.m_t;
	uint32_t microsec = (uint32_t)(1e6*(delay.m_t-(double)sec));
	DWORD t = ((DWORD)sec)*1000+(((DWORD)microsec)/1000);
	Sleep(t);
}

#else // unix style

//#ifdef RTP_HAVE_CLOCK_GETTIME
#if 0
inline double RTPTime_timespecToDouble(struct timespec &ts)
{
	return (double)ts.tv_sec + 1e-9*(double)ts.tv_nsec;
}

inline RTPTime RTPTime::CurrentTime()
{
	static bool s_initialized = false;
	static double s_startOffet = 0;

	if (!s_initialized)
	{
		s_initialized = true;

		// Get the corresponding times in system time and monotonic time
		struct timespec tpSys, tpMono;

		clock_gettime(CLOCK_REALTIME, &tpSys);
		clock_gettime(CLOCK_MONOTONIC, &tpMono);

		double tSys = RTPTime_timespecToDouble(tpSys);
		double tMono = RTPTime_timespecToDouble(tpMono);

		s_startOffet = tSys - tMono;
		return tSys;
	}

	struct timespec tpMono;
	clock_gettime(CLOCK_MONOTONIC, &tpMono);

	double tMono0 = RTPTime_timespecToDouble(tpMono);
	return tMono0 + s_startOffet;
}

#else // gettimeofday fallback

RTPTime RTPTime::CurrentTime()
{
	struct timeval tv;
	//need to change to bk_rtc_gettimeofday
	//gettimeofday(&tv,0);
	tv.tv_sec = rtos_get_time()/1000;
	tv.tv_usec = 0;
	return RTPTime((uint64_t)tv.tv_sec,(uint32_t)tv.tv_usec);
}
#endif // RTP_HAVE_CLOCK_GETTIME

void RTPTime::Wait(const RTPTime &delay)
{
	if (delay.m_t <= 0)
		return;

	rtos_delay_milliseconds(delay.GetSeconds()*1000 + delay.GetMicroSeconds()/1000);
#if 0
	uint64_t sec = (uint64_t)delay.m_t;
	uint64_t nanosec = (uint32_t)(1e9*(delay.m_t-(double)sec));

	struct timespec req,rem;
	int ret;

	req.tv_sec = (time_t)sec;
	req.tv_nsec = ((long)nanosec);
	do
	{
		ret = nanosleep(&req,&rem);
		req = rem;
	} while (ret == -1 && errno == EINTR);
#endif
}

#endif // RTP_HAVE_QUERYPERFORMANCECOUNTER

} // end namespace

