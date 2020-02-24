#ifndef __PROJECT_STONE_OCI20_LOBVAR_H__
#define __PROJECT_STONE_OCI20_LOBVAR_H__

#include "native_oci_variable.h"

namespace Oci20 {

	class LobVar : public NativeOciVariable {
    public:
        enum class Type {
            CLob = SQLT_CLOB,
            BLob = SQLT_BLOB,
            BFile = SQLT_FILE
        };

        enum class CharForm {
            Implicit = SQLCS_IMPLICIT,
            Nchar = SQLCS_NCHAR
        };


    protected:
        OCISvcCtx* m_ociSvcCtx;
        OCIError* m_ociError;

        int m_limit;
        CharForm m_charForm;
        
        LobVar(OCIEnv* ociEnv, OCISvcCtx* ociSvcCtx, OCIError* ociError, Type type, int limit, CharForm charForm = CharForm::Implicit);

        virtual sword GetString(std::string& strbuff, const std::string& null = m_null) const;

        sword GetLobLength(ub4* lenght) const;
	};

    class CLobVar : public LobVar {
    public:
        CLobVar(OCIEnv* ociEnv, OCISvcCtx* ociSvcCtx, OCIError* ociError, int limit, CharForm charForm = CharForm::Implicit)
            : LobVar(ociEnv, ociSvcCtx, ociError, Type::CLob, limit, charForm) {}

        using LobVar::GetString;
    };

    class BLobVar : public LobVar {
    protected:
        int m_hexFormatLength;
        BLobVar(OCIEnv* ociEnv, OCISvcCtx* ociSvcCtx, OCIError* ociError, Type type, int limit, int hexFormatLength);

    public:
        BLobVar(OCIEnv* ociEnv, OCISvcCtx* ociSvcCtx, OCIError* ociError, int limit, int hexFormatLength);

        virtual sword GetString(std::string& strbuff, const std::string& null = m_null) const;
    };

    class BFileVar : public BLobVar {
    public:
        BFileVar(OCIEnv* ociEnv, OCISvcCtx* ociSvcCtx, OCIError* ociError, int limit, int hexFormatLength);

        virtual sword GetString(std::string& strbuff, const std::string& null = m_null) const;
    };

}

#endif // __PROJECT_STONE_OCI20_LOBVAR_H__