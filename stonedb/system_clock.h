#ifndef __PROJECT_STONE_SYSTEMCLOCK_H__
#define __PROJECT_STONE_SYSTEMCLOCK_H__

#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <time.h>

namespace Utils {

	typedef unsigned __int64 Clock64;

	class  SystemClock  {

    public:
		static Clock64 StartCount() {

            FILETIME filetime;
            GetSystemTimeAsFileTime(&filetime);

            /* calculate the elapsed number of 100 nanosecond units */
            Clock64 current_tics = (Clock64)filetime.dwLowDateTime + (((Clock64)filetime.dwHighDateTime) << 32);

            /* return number of elapsed milliseconds */
            return (current_tics / 10000);
		}

        static double StopCount(Clock64 startTime) {
            return double(GetCurrentTime() - startTime) / CLOCKS_PER_SEC;
        }

	};
}

#endif /* __PROJECT_STONE_SYSTEMCLOCK_H__ */

