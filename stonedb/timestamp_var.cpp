#include "timestamp_var.h"
#include "error_util.h"

namespace Oci20 {

    TimestampVar::TimestampVar(OCIEnv* ociEnv, Type type)
        : NativeOciVariable(ociEnv, static_cast<ub2>(type), SqltDtypeMap(static_cast<ub2>(type))) {
    }

    TimestampVar::TimestampVar(OCIEnv* ociEnv, Type type, const std::string& dateFormat)
        : NativeOciVariable(ociEnv, static_cast<ub2>(type), SqltDtypeMap(static_cast<ub2>(type))),
        m_dateFormat(dateFormat) {

        if (m_dateFormat.empty()) {
            throw OciException(0, "Oci20::TimestampVar: Invalid DateFormat!");
        }
    }

    sword TimestampVar::GetString(std::string& strbuff, const std::string& null) const {

        if (IsNull()) {
            strbuff = null;
            return OCI_SUCCESS;
        }

        
        //TODO: Fix it! (this functionality is only available for OCI 8.1.X and later)
#if 0
        char buffer[256];
        size_t buffer_size = sizeof(buffer);
        size_t fmt_length = m_dateFormat.length();
        const char* fmt = fmt_length ? m_dateFormat.c_str() : 0;

        m_connect.DateTimeToText((const OCIDateTime*)m_buffer,
            fmt, fmt_length, 0/*fsprec*/,
            m_connect.GetNlsLanguage().c_str(), m_connect.GetNlsLanguage().length(),
            //0/*lang_name*/, 0/*lang_length*/,
            buffer, &buffer_size);
        strbuff.assign(buffer, buffer_size);
#else
        strbuff.assign("FIX IT!", 7);
#endif
        return OCI_ERROR;
    }

    std::string TimestampVar::Backcast(const std::string& val)
    {
        std::string result;

        if (m_oci_handle_type == OCI_DTYPE_TIMESTAMP_TZ || m_oci_handle_type == OCI_DTYPE_TIMESTAMP_LTZ)
            result = "To_Timestamp_TZ('";
        else
            result = "To_Timestamp('";

        result += val + "', '" + m_dateFormat + "')";

        return result;
    }

    ub2 TimestampVar::SqltDtypeMap(ub2 dtype) {

        switch (dtype) {
        case SQLT_DATE:          return OCI_DTYPE_DATE;
        case SQLT_TIME:          return OCI_DTYPE_TIME;
        case SQLT_TIMESTAMP:     return OCI_DTYPE_TIMESTAMP;
        case SQLT_TIMESTAMP_TZ:  return OCI_DTYPE_TIMESTAMP_TZ;
        case SQLT_TIMESTAMP_LTZ: return OCI_DTYPE_TIMESTAMP_LTZ;
        }

        throw OciException(0, "Oci20::TimestampVar: unsupported SQLT_.");
    }
}