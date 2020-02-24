#ifndef __PROJECT_STONE_OCI20_STATEMENT_H__
#define __PROJECT_STONE_OCI20_STATEMENT_H__

#include <oci.h>

#include <string>

#include "object.h"
#include "connect.h"
#include "variable.h"
#include "host_array.h"
#include "lob_var.h"

namespace Oci20 {


	class Statement : public Object {
    public:

        enum class Type {
            Unknown = 0,
            Select = OCI_STMT_SELECT,
            Update = OCI_STMT_UPDATE,
            Delete = OCI_STMT_DELETE,
            Insert = OCI_STMT_INSERT,
            Create = OCI_STMT_CREATE,
            Drop = OCI_STMT_DROP,
            Alter = OCI_STMT_ALTER,
            Begin = OCI_STMT_BEGIN,
            Declare = OCI_STMT_DECLARE,
            Explain = 15,
            Merge = OCI_STMT_MERGE,
            Rollback = 17,
            Commit = 21,
        };

        Statement(Connect& connect, const std::string& statementString, int prefetch = 100);
        Statement(Connect& connect, int prefetch = 100);
        ~Statement();

        void Prepare(const std::string& statementString);
        void Execute(ub4 iters = 0, bool guaranteedSafe = false);
        void ExecuteShadow(ub4 iters = 0, bool guaranteedSafe = false);

        bool Fetch(sword* pstatus = 0);
        bool Fetch(ub4 rows, ub4& outrows, sword* pstatus = 0);

        void Define(int pos, Variable& var);
        void Define(int pos, HostArray& arr);

        void Bind(int pos, Variable& var);
        void Bind(int pos, HostArray& arr);
        void Bind(const std::string&, Variable& var);
        void Bind(const std::string&, HostArray& arr);

        bool IsOpen() const { return m_sttmp ? true : false; }
        virtual void Close(bool purge = false);

        Connect& GetConnect() const { return m_connect; }
        OCIStmt* GetOCIStmt() const { return m_sttmp; }
        OCIError* GetOCIError() const { return m_connect.GetOCIError(); }

        bool IsReadOnly();
        Type GetType();
        ub4 GetRowCount();
        ub2 GetSQLFunctionCode();
        std::string GetSQLFunctionDescription();
        bool PossibleCompilationErrors();
        ub2 GetParseErrorOffset();

        void EnableAutocommit(bool enable) { m_autocommit = enable; }
        bool IsAutocommitEnabled() const { return m_autocommit; }

        bool IsStateInitialized();
        bool IsStateExecuted();
        bool IsStateEndOfFetch();

    protected:
        Connect& m_connect;
        OCIStmt* m_sttmp;
        int m_prefetch;
        bool m_autocommit;
        std::string m_sttmText;

        OCIEnv*    GetOCIEnv()    { return m_connect.GetOCIEnv(); }
        OCISvcCtx* GetOCISvcCtx() { return m_connect.GetOCISvcCtx(); }

    private:
        // copy-constraction & assign-operation is not supported
        Statement(const Statement& statement);
        void operator = (const Statement& statement);
	};

}
#endif // __PROJECT_STONE_OCI20_STATEMENT_H__
