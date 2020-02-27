#ifndef __PROJECT_STONE_OCI20_REFCURSORVARIABLE_H__
#define __PROJECT_STONE_OCI20_REFCURSORVARIABLE_H__

#include "variable.h"
#include "connect.h"

namespace Oci20 {

	class RefCursorVariable : public Variable {
    public:
        RefCursorVariable(Connect& connect, int prefetch = 100);
        ~RefCursorVariable();

        virtual sword GetString(std::string&, const std::string& null = m_null) const;
        virtual sword Bind(OCIStmt* ociStmt, OCIError* ociStmError, int pos);
        virtual sword Bind(OCIStmt* ociStmt, OCIError* ociStmError, const std::string& name);

        bool IsStateInitialized();
        bool IsStateExecuted();
        bool IsStateEndOfFetch();

        OCIStmt* GetOCIStmt() const { return m_sttmp; }
        void SetOCIStmt(OCIStmt* stmtp) { m_sttmp = stmtp; }

    protected:
        //friend AutoCursor;

        Connect& m_connect;
        OCIStmt* m_sttmp;
        int m_prefetch;

        Connect& GetConnect() const { return m_connect; }
        OCIError* GetOCIError() const { return m_connect.GetOCIError(); }

        OCIEnv* GetOCIEnv() { return m_connect.GetOCIEnv(); }
        OCISvcCtx* GetOCISvcCtx() { return m_connect.GetOCISvcCtx(); }
	};
}
#endif  /* __PROJECT_STONE_OCI20_REFCURSORVARIABLE_H__ */

