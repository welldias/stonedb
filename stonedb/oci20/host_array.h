#ifndef __PROJECT_STONE_OCI20_HOSTARRAY_H__
#define __PROJECT_STONE_OCI20_HOSTARRAY_H__

#include <oci.h>

#include <string>

namespace Oci20 {

    class HostArray {

    public:
        HostArray(ub2 type, sb4 size, sb4 count);
        virtual ~HostArray();

        virtual sword GetString(int, std::string& strbuff, const std::string& null = m_null) const = 0;
        virtual void GetTime(int, struct tm&, struct tm* null = 0) const = 0;

        virtual int     ToInt(int, int = 0) const = 0;
        virtual __int64 ToInt64(int, __int64 = 0) const = 0;
        virtual double  ToDouble(int, double = 0) const = 0;

        bool IsGood(int inx) const;
        bool IsNull(int inx) const;
        bool IsTruncated(int inx) const;

        void Assign(int inx, const dvoid* buffer, ub2 size);
        void Assign(int inx, int);
        void Assign(int inx, long);

        sword Define(OCIStmt* ociStmt, OCIError* ociStmError, int pos);
        sword Bind(OCIStmt* ociStmt, OCIError* ociStmError, int pos);
        sword Bind(OCIStmt* ociStmt, OCIError* ociStmError, const std::string& name);

        ub4   GetCurCoutn() { return m_curCount; }
        void  SetCurCoutn(ub4 curCount) { m_curCount = curCount; }

    protected:
        const char* At(int inx) const;
        const char* at(int inx) const;

        ub2    m_type;
        sb2*   m_indicators;
        ub2*   m_sizes;
        sb4    m_elmBuffSize;
        sb4    m_count;
        dvoid* m_buffer;
        ub4    m_curCount;
        //DWORD  m_threadId;

    public:
        static const std::string m_null;
    };

    class StringArray : public HostArray {
    public:
        StringArray(int size, int count);

        virtual sword GetString(int, std::string& strbuff, const std::string& null = m_null) const;
        virtual void GetTime(int, struct tm&, struct tm* null = 0) const;

        virtual int     ToInt(int, int = 0) const;
        virtual __int64 ToInt64(int, __int64 = 0) const;
        virtual double  ToDouble(int, double = 0) const;
    };

    class NumberArray : public HostArray {
    public:
        NumberArray(int count);

        virtual sword GetString(int, std::string& strbuff, const std::string& null = m_null) const;
        virtual void GetTime(int, struct tm&, struct tm* null = 0) const;

        virtual int    ToInt(int, int = 0) const;
        virtual __int64 ToInt64(int, __int64 = 0) const;
        virtual double ToDouble(int, double = 0) const;
    };

    class DateArray : public HostArray {
    public:
        DateArray(int count, const std::string& dateForamt = m_defaultDateFormat);

        virtual sword GetString(int, std::string& strbuff, const std::string& null = m_null) const;
        virtual void GetTime(int, struct tm&, struct tm* null = 0) const;

        virtual int    ToInt(int, int = 0) const;
        virtual __int64 ToInt64(int, __int64 = 0) const;
        virtual double ToDouble(int, double = 0) const;

    protected:
        int m_dataStrLength;
        std::string m_dateFormat;
    public:
        static const std::string m_defaultDateFormat; // "%Y.%m.%d %H:%M:%S"
    };
}

#endif /*__PROJECT_STONE_OCI20_HOSTARRAY_H__*/

