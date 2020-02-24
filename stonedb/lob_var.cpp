#include <sstream>
#include <iomanip>

#include "lob_var.h"
#include "error_util.h"

namespace Oci20 {

    LobVar::LobVar(OCIEnv* ociEnv, OCISvcCtx* ociSvcCtx, OCIError* ociError, Type subtype, int limit, CharForm charForm)
        : NativeOciVariable(ociEnv, static_cast<ub2>(subtype), OCI_DTYPE_LOB),
        m_ociSvcCtx(ociSvcCtx),
        m_ociError(ociError),
        m_limit(limit),
        m_charForm(charForm) {
    }

    sword LobVar::GetLobLength(ub4* length) const {
        
        if (!length) {
            return OCI_ERROR;
        }

        if (IsNull()) {
            *length = 0;
            return OCI_SUCCESS;
        }

        *length = 0;

        sword result = OCILobGetLength(m_ociSvcCtx, m_ociError, (OCILobLocator*)m_buffer, length);
        if (result == OCI_SUCCESS && *length > static_cast<ub4>(m_limit)) {
            *length = m_limit;
        }

        return result;
    }

    sword LobVar::GetString(std::string& strbuff, const std::string& null) const {
        if (IsNull()) {
            strbuff = null;
            return OCI_SUCCESS;
        }

        ub4 length = 0;

        sword result = GetLobLength(&length);
        if (result != OCI_SUCCESS) {
            return result;
        }

        strbuff.resize(result);

        ub4 offset = 1;
        ub4 buffsize = static_cast<ub4>(strbuff.size() + 1);
        char* pStrbuff = const_cast<char*>(strbuff.data());
        ub4 readsize = buffsize;

        return OCILobRead(m_ociSvcCtx, m_ociError, (OCILobLocator*)m_buffer, &readsize, offset, pStrbuff, readsize, 0, 0, 0, static_cast<ub1>(m_charForm));
    }

    BLobVar::BLobVar(OCIEnv* ociEnv, OCISvcCtx* ociSvcCtx, OCIError* ociError, Type type, int limit, int hexFormatLength)
        : LobVar(ociEnv, ociSvcCtx, ociError, type, limit),
        m_hexFormatLength(hexFormatLength) {

        if (!(m_hexFormatLength % 2 == 0)) {
            throw OciException(0, "Oci20::BLobVar::BLobVar: Hex line length must be even.");
        }
    }

    BLobVar::BLobVar(OCIEnv* ociEnv, OCISvcCtx* ociSvcCtx, OCIError* ociError, int limit, int hexFormatLength)
        : LobVar(ociEnv, ociSvcCtx, ociError, Type::BLob, limit),
        m_hexFormatLength(hexFormatLength) {
        if (!(m_hexFormatLength % 2 == 0)) {
            throw OciException(0, "Oci20::BLobVar::BLobVar: Hex line length must be even.");
        }
    }

    sword BLobVar::GetString(std::string& strbuff, const std::string& null) const
    {
        if (IsNull()) {
            strbuff = null;
            return OCI_SUCCESS;
        }

		std::string binBata;
        sword result = LobVar::GetString(binBata, null);
        if (result != OCI_SUCCESS) {
            return result;
        }

		std::ostringstream out;
		out << std::hex << std::setfill('0');

		std::string::const_iterator it = binBata.begin();
		for (int i = 0; it != binBata.end(); ++it) {
			out << std::setw(2) << (int)(unsigned char)*it;
			if (!(++i % m_hexFormatLength)) 
                out << std::endl;
		}

		strbuff = out.str();

        return result;
    }

    BFileVar::BFileVar(OCIEnv* ociEnv, OCISvcCtx* ociSvcCtx, OCIError* ociError, int limit, int hexFormatLength)
        : BLobVar(ociEnv, ociSvcCtx, ociError, Type::BFile, limit, hexFormatLength) {
    }

    sword BFileVar::GetString(std::string& buff, const std::string& null) const {

        sword result = OCILobFileOpen(m_ociSvcCtx, m_ociError, (OCILobLocator*)m_buffer, OCI_FILE_READONLY);
        if (result != OCI_SUCCESS) {
            return result;
        }

        result = BLobVar::GetString(buff, null);
        if (result != OCI_SUCCESS) {
            result = OCILobFileClose(m_ociSvcCtx, m_ociError, (OCILobLocator*)m_buffer);
            if (result != OCI_SUCCESS) {
                return result;
            }
        }

        return OCILobFileClose(m_ociSvcCtx, m_ociError, (OCILobLocator*)m_buffer);
    }
}