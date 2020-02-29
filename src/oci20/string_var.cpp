#include <cstring>

#include "string_var.h"
#include "error_util.h"

namespace Oci20 {

    StringVar::StringVar(int size)
        : Variable(SQLT_LVC, size + sizeof(int)) {
    }

    StringVar::StringVar(const std::string& str)
        : Variable(SQLT_LVC, static_cast<sb4>(str.size() + sizeof(int))) {
        Assign(str);
    }

    void StringVar::Assign(const std::string& str) {

        int size = static_cast<int>(str.length());

        if (size) {
            if ((size + static_cast<int>(sizeof(int))) > m_buffer_size)
                throw Exception(0, "Oci20::StringVar::Assign(): String buffer overflow!");

            m_indicator = OCI_IND_NOTNULL;

            *(int*)m_buffer = size;

            m_size = size + sizeof(int);

            memcpy((char*)m_buffer + sizeof(int), str.data(), size);
        }
        else {
            m_indicator = OCI_IND_NULL;
        }
    }

    sword StringVar::GetString(std::string& strbuff, const std::string& null) const
    {
        if (IsNull()) {
            strbuff = null;
            return OCI_SUCCESS;
        }

		// 26.10.2003 workaround for oracle 8.1.6, removed trailing '0' for long columns and trigger text
		int length = *(int*)m_buffer;

		for (; length > 0; length--)
			if (((const char*)m_buffer + sizeof(int))[length - 1] != 0)
				break;

		strbuff.assign((char*)m_buffer + sizeof(int), length);

        return OCI_SUCCESS;
    }
}