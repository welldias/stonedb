#include "host_array.h"
#include "error_util.h"

namespace Oci20 {

    const std::string HostArray::m_null;

    HostArray::HostArray(ub2 type, sb4 size, sb4 count) {
        m_type = type;
        m_elmBuffSize = size;
        m_count = count;
        m_curCount = 0;

        std::multiplies<sb4> x;
        sb4 buffSize = x.operator()(m_elmBuffSize, m_count);

        m_buffer = new char[buffSize];
        memset(m_buffer, 0, buffSize);

        m_indicators = new sb2[m_count];
        memset(m_indicators, 0, sizeof(sb2) * m_count);

        m_sizes = new ub2[m_count];
        memset(m_sizes, 0, sizeof(ub2) * m_count);
    }

    HostArray::~HostArray() {
        try {
            delete[] m_sizes;
            delete[] m_indicators;
            delete[](char*)m_buffer;
        }
        catch (...) {}
    }

    void HostArray::Assign(int inx, const dvoid* buffer, ub2 size) {

        if (inx < m_count) {
            if (size <= m_elmBuffSize) {

                std::multiplies<sb4> x;
                sb4 buffSize = x.operator()(m_elmBuffSize, inx);

                memcpy(((char*)m_buffer) + buffSize, buffer, size);
                m_sizes[inx] = size;
            }
            else
                throw OciException(0, "Oci20::HostArray::Assign(): Buffer overflow!");
        }
        else
            throw OciException(0, "Oci20::HostArray::Assign(): Out of range!");
    }

    void HostArray::Assign(int inx, int val) {
        Assign(inx, &val, sizeof(val));
    }

    void HostArray::Assign(int inx, long val) {
        Assign(inx, &val, sizeof(val));
    }

    const char* HostArray::At(int inx) const {

        if (inx >= static_cast<int>(m_curCount))
            throw OciException(0, "Oci20::HostArray::At(): Out of range!");

        std::multiplies<sb4> x;
        sb4 buffSize = x.operator()(m_elmBuffSize, inx);

        return ((const char*)m_buffer) + buffSize;
    }

    const char* HostArray::at(int inx) const { 
        std::multiplies<sb4> x;
        sb4 buffSize = x.operator()(m_elmBuffSize, inx);

        return ((const char*)m_buffer) + buffSize;
    }

    sword HostArray::Define(OCIStmt* ociStmt, OCIError* ociStmError, int pos) {
        OCIDefine* defhp;
        return OCIDefineByPos(ociStmt, &defhp, ociStmError, pos, m_buffer, m_elmBuffSize, m_type, m_indicators, m_sizes, 0, OCI_DEFAULT);
    }

    sword HostArray::Bind(OCIStmt* ociStmt, OCIError* ociStmError, int pos) {
        OCIBind* bndhp;
        return OCIBindByPos(ociStmt, &bndhp, ociStmError, pos, m_buffer, m_elmBuffSize, m_type, m_indicators, m_sizes, 0, m_count, 0, OCI_DEFAULT);
    }

    sword HostArray::Bind(OCIStmt* ociStmt, OCIError* ociStmError, const std::string& name) {
        OCIBind* bndhp;
        return OCIBindByName(ociStmt, &bndhp, ociStmError, (OraText*)name.c_str(), static_cast<ub4>(name.length()), m_buffer, m_elmBuffSize, m_type, m_indicators, m_sizes, 0, m_count, &m_curCount, OCI_DEFAULT);
    }

    bool HostArray::IsGood(int inx) const {
        return (m_indicators[inx] == -1 || m_indicators[inx] == 0) ? true : false;
    }

    bool HostArray::IsNull(int inx) const {
        return m_indicators[inx] == -1 ? true : false;
    }

    bool HostArray::IsTruncated(int inx) const {
        return m_indicators[inx] == -2 || m_indicators[inx] > 0 ? true : false;
    }

    StringArray::StringArray(int size, int count)
        : HostArray(SQLT_LVC, size + sizeof(int), count) {
    }

    sword StringArray::GetString(int inx, std::string& strbuff, const std::string& null) const {
        if (IsNull(inx)) {
            strbuff = null;
            return OCI_SUCCESS;
        }

        // 26.10.2003 workaround for oracle 8.1.6, removed trailing '0' for long columns and trigger text
        int length = *(int*)At(inx);
        for (; length > 0; length--)
            if ((at(inx) + sizeof(int))[length - 1] != 0)
                break;

        strbuff.assign(At(inx) + sizeof(int), length);

        return OCI_SUCCESS;
    }

    void StringArray::GetTime(int, struct tm&, struct tm*) const {

        throw OciException(0, "Oci20:StringArray::GetTime(): Cannot cast String to \"tm\"!");
    }

    int StringArray::ToInt(int, int) const {

        throw OciException(0, "Oci20:StringArray::ToInt(): Cannot cast String to Int!");
    }

    __int64 StringArray::ToInt64(int, __int64) const {

        throw OciException(0, "Oci20:StringArray::ToInt64(): Cannot cast Date to Int!");
    }

    double StringArray::ToDouble(int, double) const {

        throw OciException(0, "Oci20::StringArray::ToDouble(): Cannot cast String to Double!");
    }

    NumberArray::NumberArray(int count)
        : HostArray(SQLT_STR, 41, count) {
    }

    sword NumberArray::GetString(int inx, std::string& strbuff, const std::string& null) const {
        
        if (IsNull(inx))
            strbuff = null;
        else
            strbuff = At(inx);

        return OCI_SUCCESS;
    }

    void NumberArray::GetTime(int, struct tm&, struct tm*) const {
        throw OciException(0, "Oci20::NumberArray::GetTime(): Cannot cast Number to \"tm\"!");
    }

    int NumberArray::ToInt(int inx, int null) const {
        if (IsNull(inx))
            return null;

        return atoi(At(inx));
    }

    __int64 NumberArray::ToInt64(int inx, __int64 null) const {
        if (IsNull(inx))
            return null;

        return _atoi64(At(inx));
    }

    double NumberArray::ToDouble(int inx, double null) const {
        if (IsNull(inx))
            return null;

        return atof(At(inx));
    }

    const std::string DateArray::m_defaultDateFormat = "%Y.%m.%d %H:%M:%S";

    DateArray::DateArray(int count, const std::string& defaultDateFormat)
        : HostArray(SQLT_DAT, 7, count),
        m_dateFormat(defaultDateFormat),
        m_dataStrLength(0) {

        time_t t1;
        tm t2;

        time(&t1);
        errno_t err = gmtime_s(&t2, &t1);

        char buff[80];
        strftime(buff, sizeof(buff), m_dateFormat.c_str(), &t2);
        m_dataStrLength = static_cast<int>(strlen(buff));
    }

    sword DateArray::GetString(int inx, std::string& strbuff, const std::string& null) const {
        if (IsNull(inx)) {
            strbuff = null;
            return OCI_SUCCESS;
        }

        const char* data = At(inx);
        tm time;
        time.tm_isdst = time.tm_wday = time.tm_yday = -1;
        time.tm_year = (data[0] - 100) * 100 + (data[1] - 100) - 1900;
        time.tm_mon = data[2] - 1;
        time.tm_mday = data[3];
        time.tm_hour = data[4] - 1;
        time.tm_min = data[5] - 1;
        time.tm_sec = data[6] - 1;
        char buff[80];
        strftime(buff, sizeof buff, m_dateFormat.c_str(), &time);
        strbuff = buff;

        return OCI_SUCCESS;
    }

    void DateArray::GetTime(int inx, struct tm& time, struct tm* null /*= 0*/) const {
        if (!IsNull(inx)) {
            const char* data = At(inx);
            time.tm_isdst = time.tm_wday = time.tm_yday = -1;
            time.tm_year = (data[0] - 100) * 100 + (data[1] - 100) - 1900;
            time.tm_mon = data[2] - 1;
            time.tm_mday = data[3];
            time.tm_hour = data[4] - 1;
            time.tm_min = data[5] - 1;
            time.tm_sec = data[6] - 1;
        }
        else {
            if (null)
                time = *null;
            else
                memset(&time, 0, sizeof(time));
        }
    }

    int DateArray::ToInt(int, int) const {
        throw OciException(0, "Oci20::DateArray::ToInt(): Cannot cast Date to Int!");
    }

    __int64 DateArray::ToInt64(int, __int64) const {
        throw OciException(0, "Oci20::DateArray::ToInt64(): Cannot cast Date to Int!");
    }

    double DateArray::ToDouble(int, double) const {
        throw OciException(0, "Oci20::DateArray::ToDouble(): Cannot cast Date to Double!");
    }
}