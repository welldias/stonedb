#ifndef __PROJECT_STONE_SYSTEMCLOCK_H__
#define __PROJECT_STONE_SYSTEMCLOCK_H__

#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <time.h>
#endif

#include <chrono>
#include <cstring>
#include <cstdint>

namespace Utils {

    using namespace std::chrono;

    typedef system_clock::time_point Clock64;

	class  SystemClock  {

    public:
		static Clock64 StartCount() {
            return system_clock::now();
		}

        static long long StopCount(Clock64 startTime) {
            return (duration_cast<milliseconds>(system_clock::now() - startTime)).count();
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

