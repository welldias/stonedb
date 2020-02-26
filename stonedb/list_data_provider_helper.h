#ifndef __PROJECT_STONE_DATA_LISTDATAPROVIDERHELPER_H__
#define __PROJECT_STONE_DATA_LISTDATAPROVIDERHELPER_H__

#include <string>
#include <ctime>

namespace Data {

	class ListDataProviderHelper {
    private:
        mutable char m_buffer[128];

    protected:
        const void ToString(const std::string& str, std::string& result) const { result = str; }
        const void ToString(__int64 i, std::string& result) const { result = std::to_string(i); }
        const void ToString(unsigned __int64 i, std::string& result) const { result = std::to_string(i); }
        const void ToString(int i, std::string& result) const { result = std::to_string(i); }
        const void ToString(tm time, std::string& result) const {
            
            static tm null = { 0 };
            if (memcmp(&time, &null, sizeof(tm))) {
                strftime(m_buffer, sizeof(m_buffer), "%Y-%m-%d %H:%M:%S", &time);
                m_buffer[sizeof(m_buffer) - 1] = 0;
            }
            else
                m_buffer[0] = 0;

            result = m_buffer;
        }
        const void GetStrTimeT(time_t t, std::string& result) const {
            tm time = { 0 };

            if (localtime_s(&time, &t)) {
                m_buffer[0] = 0;
            }
            else {
                strftime(m_buffer, sizeof(m_buffer), "%Y-%m-%d %H:%M:%S", &time);
                m_buffer[sizeof(m_buffer) - 1] = 0;
            }

            result = m_buffer;
        }

        static int Comp(const std::string& s1, const std::string& s2) { 
            return _stricmp(s1.c_str(), s2.c_str()); 
        }
        static int Comp(int val1, int val2) { return val1 == val2 ? 0 : (val1 < val2 ? -1 : 1); }
        static int Comp(__int64 val1, __int64 val2) { return val1 == val2 ? 0 : (val1 < val2 ? -1 : 1); }
        static int Comp(unsigned __int64 val1, unsigned __int64 val2) { return val1 == val2 ? 0 : (val1 < val2 ? -1 : 1); }
        static int CompTimeT(time_t val1, time_t val2) { return val1 == val2 ? 0 : (val1 < val2 ? -1 : 1); }
        static int Comp(tm val1, tm val2) {
            if (val1.tm_year != val2.tm_year)
                return val1.tm_year < val2.tm_year ? -1 : 1;
            if (val1.tm_mon != val2.tm_mon)
                return val1.tm_mon < val2.tm_mon ? -1 : 1;
            if (val1.tm_mday != val2.tm_mday)
                return val1.tm_mday < val2.tm_mday ? -1 : 1;
            if (val1.tm_hour != val2.tm_hour)
                return val1.tm_hour < val2.tm_hour ? -1 : 1;
            if (val1.tm_min != val2.tm_min)
                return val1.tm_min < val2.tm_min ? -1 : 1;
            if (val1.tm_sec != val2.tm_sec)
                return val1.tm_sec < val2.tm_sec ? -1 : 1;

            return 0;
        }
	};
}

#endif /*__PROJECT_STONE_DATA_LISTDATAPROVIDERHELPER_H__*/