#include "number_var.h"
#include "error_util.h"

namespace Oci20 {
    const char overflow[] = "###";

    NumberVar::NumberVar(OCIError* ociError)
        : Variable(&m_value, SQLT_VNU, sizeof(m_value)), m_ociError(ociError) {

        OCINumberSetZero(ociError, &m_value);
    }

    NumberVar::NumberVar(OCIError* ociError, const std::string& numberFormat)
        : Variable(&m_value, SQLT_VNU, sizeof(m_value)), m_ociError(ociError), m_numberFormat(numberFormat) {

        OCINumberSetZero(ociError, &m_value);
    }

    NumberVar::NumberVar(OCIError* ociError, int value)
        : Variable(&m_value, SQLT_VNU, sizeof(m_value)), m_ociError(ociError) {

        Assign(value);
    }

    NumberVar::NumberVar(OCIError* ociError, double value)
        : Variable(&m_value, SQLT_VNU, sizeof(m_value)), m_ociError(ociError) {

        Assign(value);
    }

    sword NumberVar::Assign(int val) {

        sword result = OCINumberFromInt(m_ociError, &val, sizeof(val), OCI_NUMBER_SIGNED, &m_value);

        if(result == OCI_SUCCESS)
            m_indicator = OCI_IND_NOTNULL;

        return result;
    }

    sword NumberVar::Assign(double val) {

        sword result = OCINumberFromReal(m_ociError, &val, sizeof(val), &m_value);

        if (result == OCI_SUCCESS)
            m_indicator = OCI_IND_NOTNULL;

        return result;
    }

	sword NumberVar::GetString(std::string& strbuff, const std::string& numericCharacters, const std::string& null) const {

		if (IsNull()) {
			strbuff = null;
			return OCI_SUCCESS;
		}

		OraText buffer[256];
		ub4 buffer_size = sizeof(buffer);
		ub1 fmt_length = (ub1)m_numberFormat.length();
		const OraText* fmt = fmt_length ? (const OraText*)m_numberFormat.c_str() : 0;

		//TODO: improve it - cache it or something else
		std::string nls_param;
		if (!numericCharacters.empty()) {
			nls_param = "NLS_NUMERIC_CHARACTERS='";
			nls_param += numericCharacters;
			nls_param += '\'';
		}

		sword result = OCINumberToText(m_ociError, &m_value, fmt, fmt_length, (const OraText*)nls_param.data(), static_cast<ub4>(nls_param.length()), &buffer_size, buffer);
		if (result == OCI_SUCCESS) {
			strbuff.assign(reinterpret_cast<char*>(buffer), buffer_size);
		}
		else if (result == 22065) {
			strbuff = "###";
			result = OCI_SUCCESS;
		}

		return result;
	}

    sword NumberVar::ToInt(int* value, int null) const {
        
        if (!value) {
            return OCI_SUCCESS;
        }

        if (IsNull()) {
            *value = null;
            return OCI_SUCCESS;
        }

        *value = 0;
        return OCINumberToInt(m_ociError, &m_value, sizeof(value), OCI_NUMBER_SIGNED, &value);
    }

    sword NumberVar::ToDouble(double* value, double null) const {
        
        if (!value) {
            return OCI_SUCCESS;
        }

        if (IsNull()) {
            *value = null;
            return OCI_SUCCESS;
        }

        return OCINumberToReal(m_ociError, &m_value, sizeof(value), &value);
    }
}