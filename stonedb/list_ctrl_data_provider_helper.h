#ifndef __PROJECT_STONE_DATA_LISTCTRLDATAPROVIDERHELPER_H__
#define __PROJECT_STONE_DATA_LISTCTRLDATAPROVIDERHELPER_H__

#include <string>
#include <ctime>

namespace Data {
	class ListCtrlDataProviderHelper
	{
    private:
        mutable char m_buffer[128];

    protected:
        const char* getStr(const std::string& str) const { return str.c_str(); }
        const char* getStr(__int64 i) const { return _i64toa(i, m_buffer, 10); }
        const char* getStr(unsigned __int64 i) const { return _ui64toa(i, m_buffer, 10); }
        const char* getStr(int i) const { return itoa(i, m_buffer, 10); }
        const char* getStr(tm time) const {
            static tm null = { 0 };
            if (memcmp(&time, &null, sizeof(tm))) {
                strftime(m_buffer, sizeof m_buffer, "%Y-%m-%d %H:%M:%S", &time);
                m_buffer[sizeof(m_buffer) - 1] = 0;
            }
            else
                m_buffer[0] = 0;
            return m_buffer;
        }
        const char* getStrTimeT(time_t t) const {
            if (const tm* ptm = localtime(&t)) {
                strftime(m_buffer, sizeof m_buffer, "%Y-%m-%d %H:%M:%S", ptm);
                m_buffer[sizeof(m_buffer) - 1] = 0;
            }
            else
                m_buffer[0] = 0;
            return m_buffer;
        }

        static int comp(const std::string& s1, const std::string& s2) { return stricmp(s1.c_str(), s2.c_str()); }
        static int comp(int val1, int val2) { return val1 == val2 ? 0 : (val1 < val2 ? -1 : 1); }
        static int comp(__int64 val1, __int64 val2) { return val1 == val2 ? 0 : (val1 < val2 ? -1 : 1); }
        static int comp(unsigned __int64 val1, unsigned __int64 val2) { return val1 == val2 ? 0 : (val1 < val2 ? -1 : 1); }
        static int compTimeT(time_t val1, time_t val2) { return val1 == val2 ? 0 : (val1 < val2 ? -1 : 1); }
        static int comp(tm val1, tm val2) {
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

#endif /*__PROJECT_STONE_DATA_LISTCTRLDATAPROVIDERHELPER_H__*/