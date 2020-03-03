#ifndef __PROJECT_STONE_SYSTEMCLOCK_H__
#define __PROJECT_STONE_SYSTEMCLOCK_H__

#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <time.h>
#endif

#include <cstring>
#include <cstdint>

namespace Utils {

	typedef uint64_t Clock64;

	class  SystemClock  {

    public:
		static Clock64 StartCount() {

#ifdef _WINDOWS
            FILETIME filetime;
            GetSystemTimeAsFileTime(&filetime);

            /* calculate the elapsed number of 100 nanosecond units */
            Clock64 current_tics = (Clock64)filetime.dwLowDateTime + (((Clock64)filetime.dwHighDateTime) << 32);

            /* return number of elapsed milliseconds */
            return (current_tics / 10000);
#else
            return 0;
#endif
		}

        static double StopCount(Clock64 startTime) {
#ifdef _WINDOWS
            return double(GetCurrentTime() - startTime) / CLOCKS_PER_SEC;
#else
            return 0.0;
#endif
        }

        static bool LocalTime(time_t& t, tm& time) {
#ifdef _WINDOWS
            return (localtime_s(&time, &t) == 0);
#else
            tm* ptrTime = localtime(&t);
            if (ptrTime != NULL) {
                memcpy(&time, &ptrTime, sizeof(time));
                return true;
            }
            return false;
#endif
        }

        static bool GmTime(time_t& t, tm& time) {

#ifdef _WINDOWS
            return (gmtime_s(&time, &t) == 0);
#else
            tm* ptrTime = gmtime(&t);
            if (ptrTime != NULL) {
                memcpy(&time, &ptrTime, sizeof(time));
                return true;
            }
            return false;
#endif
        }

	};
}

#endif /* __PROJECT_STONE_SYSTEMCLOCK_H__ */

