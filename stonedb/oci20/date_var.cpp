#include "date_var.h"
#include "error_util.h"

namespace Oci20 {

    DateVar::DateVar(OCIError* ociError, const std::string& nlsLanguage)
        : Variable(&m_value, SQLT_ODT, sizeof(m_value)) {

        m_ociError = ociError;
        m_nlsLanguage = nlsLanguage;
    }

    DateVar::DateVar(OCIError* ociError, const std::string& nlsLanguage, const std::string& dateFormat)
        : Variable(&m_value, SQLT_ODT, sizeof(m_value)) {
        m_ociError = ociError;
        m_nlsLanguage = nlsLanguage;
        m_dateFormat = dateFormat;
    }

    sword DateVar::GetString(std::string& strbuff, const std::string& null) const {
        if (IsNull()) {
            strbuff = null;
            return OCI_SUCCESS;
        }

		OraText buffer[256];
        ub4 buffer_size = sizeof(buffer);
		size_t fmt_length = m_dateFormat.length();
		const OraText* fmt = fmt_length ? (const OraText*)m_dateFormat.data() : 0;

        sword result = OCIDateToText(m_ociError, &m_value, fmt, static_cast<ub1>(fmt_length), (const OraText*)m_nlsLanguage.data(), static_cast<ub4>(m_nlsLanguage.length()), &buffer_size, buffer);
        if (result == OCI_SUCCESS) {
            strbuff.assign(reinterpret_cast<char*>(buffer), buffer_size);
        }

        return result;
		
    }
}