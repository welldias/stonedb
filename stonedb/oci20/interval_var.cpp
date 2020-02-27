#include "interval_var.h"
#include "error_util.h"

namespace Oci20 {

    IntervalVar::IntervalVar(OCIEnv* ociEnv, Type type)
        : NativeOciVariable(ociEnv, static_cast<ub2>(type), SqltDtypeMap(static_cast<ub2>(type))) {
    }

    sword IntervalVar::GetString(std::string& strbuff, const std::string& null) const {

        if (IsNull()) {
            strbuff = null;
            return OCI_SUCCESS;
        }

        //TODO: Fix it! (this functionality is only available for OCI 8.1.X and later)
#if 0
        char buffer[256];
        size_t resultlen;

        m_connect.IntervalToText((const OCIInterval*)m_buffer, 0/*lfprec*/, 2/*fsprec*/, buffer, sizeof(buffer), &resultlen);

        strbuff.assign(buffer, resultlen);
#else
        strbuff.assign("FIX IT!", 7);
#endif
        return OCI_ERROR;
    }

    std::string IntervalVar::Backcast(const std::string& val) {
        return "INTERVAL '" + val + (m_oci_handle_type == SQLT_INTERVAL_DS ? "' DAY TO SECOND" : "' YEAR TO MONTH"); 
    }

    ub2 IntervalVar::SqltDtypeMap(ub2 dtype) {
        switch (dtype)
        {
        case SQLT_INTERVAL_YM:  return OCI_DTYPE_INTERVAL_YM;
        case SQLT_INTERVAL_DS:  return OCI_DTYPE_INTERVAL_DS;
        }

        throw OciException(0, "Oci20:IntervalVar::TimestampVar: unsupported SQLT_.");
    }
}
