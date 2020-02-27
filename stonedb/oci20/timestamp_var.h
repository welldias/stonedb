#ifndef __PROJECT_STONE_OCI20_TIMESTAMPVAR_H__
#define __PROJECT_STONE_OCI20_TIMESTAMPVAR_H__

#include "native_oci_variable.h"

namespace Oci20 {

    class TimestampVar : public NativeOciVariable {
    public:

        enum class Type {
            Date = SQLT_DATE,
            Time = SQLT_TIME,
            Timestamp = SQLT_TIMESTAMP,
            TimestampTz = SQLT_TIMESTAMP_TZ,
            TimestampLtz = SQLT_TIMESTAMP_LTZ,
        };

        TimestampVar(OCIEnv* ociEnv, Type type);
        TimestampVar(OCIEnv* ociEnv, Type type, const std::string& dateFormat);

        virtual sword GetString(std::string& strbuff, const std::string& null = m_null) const;

        virtual bool IsDatetime() const { return true; }
        virtual std::string Backcast(const std::string& val);

    protected:
        std::string m_dateFormat;
        static ub2 SqltDtypeMap(ub2);
    };
}

#endif // __PROJECT_STONE_OCI20_TIMESTAMPVAR_H__
