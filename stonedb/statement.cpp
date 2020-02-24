#include "statement.h"
#include "error_util.h"
#include "function_codes.h"

namespace Oci20
{
    // 10.09.2003 bug fix, 8.0.5 compatibility
    const int OCI80X_ATTR_PARSE_ERROR_OFFSET = 128;

    Statement::Statement(Connect& connect, const std::string& statementString, int prefetch)
        : Object(),
        m_connect(connect),
        m_sttmp(0),
        m_prefetch(prefetch),
        m_autocommit(false) {

        Prepare(statementString);
    }

    Statement::Statement(Connect& connect, int prefetch)
        : Object(),
        m_connect(connect),
        m_sttmp(0),
        m_prefetch(prefetch),
        m_autocommit(false) {

    }

    Statement::~Statement() {
            Close();
    }

    void Statement::Close(bool purge) {
        if (m_sttmp)
            OCIHandleFree(m_sttmp, OCI_HTYPE_STMT);

        m_sttmp = 0;
    }

    void Statement::Prepare(const std::string& statementString) {

        m_sttmText = statementString;

        ERROR_UTIL_CHECKALLOC(OCIHandleAlloc(m_connect.GetOCIEnv(), (dvoid**)&m_sttmp, OCI_HTYPE_STMT, 0, 0));

        //8171  1315603 when fetching NULL data.
        //              ie: indp / rcodep may be incorrect.
        //              Workaround: Set OCI_ATTR_PREFETCH_ROWS to 0
        //              This problem was introduced in 8.1.6
        if (m_connect.GetClientVersion() >= ClientVersion::Client9X) {
            ERROR_UTIL_CHECKTHROW(OCIAttrSet(m_sttmp, OCI_HTYPE_STMT, &m_prefetch, 0, OCI_ATTR_PREFETCH_ROWS, GetOCIError()), m_connect, m_sttmText);
        }
        ERROR_UTIL_CHECKTHROW(OCIStmtPrepare(m_sttmp, GetOCIError(), (oratext*)statementString.c_str(), static_cast<ub4>(statementString.length()), OCI_NTV_SYNTAX, OCI_DEFAULT), m_connect, m_sttmText);
    }

    void Statement::Execute(ub4 iters, bool guaranteedSafe) {

        ERROR_UTIL_CHECKINTERRUPT(m_connect);

        if (!guaranteedSafe && m_connect.GetSafety() == Safety::ReadOnly && !IsReadOnly()) {
            throw OciException(0, "Oci20::Statement::Execute: Only SELECT is allowed for Read-Only connection!");
        }

        m_connect.SetSession();

        // 27.06.2004 bug fix, Autocommit does not work
        ERROR_UTIL_CHECKTHROW(OCIStmtExecute(GetOCISvcCtx(), m_sttmp, GetOCIError(), iters, 0, (OCISnapshot*)NULL, (OCISnapshot*)NULL, m_autocommit ? OCI_COMMIT_ON_SUCCESS : OCI_DEFAULT), m_connect, m_sttmText);
    }

    void Statement::ExecuteShadow(ub4 iters, bool guaranteedSafe) {

        ERROR_UTIL_CHECKINTERRUPT(m_connect);

        if (!guaranteedSafe && m_connect.GetSafety() == Safety::ReadOnly && !IsReadOnly()) {
            throw OciException(0, "Oci20::Statement::Execute: Only SELECT is allowed for Read-Only connection!");
        }

        m_connect.SetShadowSession();

        // 27.06.2004 bug fix, Autocommit does not work
        ERROR_UTIL_CHECKTHROW(OCIStmtExecute(GetOCISvcCtx(), m_sttmp, GetOCIError(), iters, 0, (OCISnapshot*)NULL, (OCISnapshot*)NULL, m_autocommit ? OCI_COMMIT_ON_SUCCESS : OCI_DEFAULT), m_connect, m_sttmText);
    }

    bool Statement::Fetch(sword* pstatus) {

        bool result = true;

        ERROR_UTIL_CHECKINTERRUPT(m_connect);

        sword status = OCIStmtFetch(m_sttmp, GetOCIError(), 1, OCI_FETCH_NEXT, OCI_DEFAULT);

        if (pstatus)
            *pstatus = status;

        switch (status) {
        case OCI_NO_DATA:
            result = false;
        case OCI_SUCCESS:
        case OCI_SUCCESS_WITH_INFO:
            break;
        default:
            ERROR_UTIL_CHECKTHROW(status, m_connect, m_sttmText);
        }

        m_connect.SetLastExecutionClockTime();

        return result;
    }

    void Statement::Define(int pos, Variable& var) {
        
        ERROR_UTIL_CHECKINTERRUPT(m_connect);
        ERROR_UTIL_CHECKTHROW(var.Define(GetOCIStmt(), GetOCIError(), pos), m_connect, m_sttmText);
    }

    void Statement::Define(int pos, HostArray& arr) {
        
        ERROR_UTIL_CHECKINTERRUPT(m_connect);
        ERROR_UTIL_CHECKTHROW(arr.Define(GetOCIStmt(), GetOCIError(), pos), m_connect, m_sttmText);
    }

    void Statement::Bind(int pos, Variable& var) {
        ERROR_UTIL_CHECKINTERRUPT(m_connect);
        ERROR_UTIL_CHECKTHROW(var.Bind(GetOCIStmt(), GetOCIError(), pos), m_connect, m_sttmText);
    }

    void Statement::Bind(int pos, HostArray& arr) {
        ERROR_UTIL_CHECKINTERRUPT(m_connect);
        ERROR_UTIL_CHECKTHROW(arr.Bind(GetOCIStmt(), GetOCIError(), pos), m_connect, m_sttmText);
    }

    void Statement::Bind(const std::string& name, Variable& var) {
        ERROR_UTIL_CHECKINTERRUPT(m_connect);
        ERROR_UTIL_CHECKTHROW(var.Bind(GetOCIStmt(), GetOCIError(), name), m_connect, m_sttmText);
    }

    void Statement::Bind(const std::string& name, HostArray& arr) {
        ERROR_UTIL_CHECKINTERRUPT(m_connect);
        ERROR_UTIL_CHECKTHROW(arr.Bind(GetOCIStmt(), GetOCIError(), name), m_connect, m_sttmText);
    }

    bool Statement::Fetch(ub4 rows, ub4& outrows, sword* pstatus) {

        bool result = true;

        ERROR_UTIL_CHECKINTERRUPT(m_connect);

        sword status = OCIStmtFetch(m_sttmp, GetOCIError(), rows, OCI_FETCH_NEXT, OCI_DEFAULT);

        if (pstatus)
            *pstatus = status;

        switch (status) {
        case OCI_NO_DATA:
            result = false;
        case OCI_SUCCESS:
        case OCI_SUCCESS_WITH_INFO:
            break;
        default:
            ERROR_UTIL_CHECKTHROW(status, m_connect, m_sttmText);
        }

        ERROR_UTIL_CHECKTHROW(OCIAttrGet(m_sttmp, OCI_HTYPE_STMT, &outrows, (ub4*)0, OCI_ATTR_ROW_COUNT, GetOCIError()), m_connect, m_sttmText);

        m_connect.SetLastExecutionClockTime();

        return result;
    }

    bool Statement::IsReadOnly() {
        
        switch (GetType()) {
        case StatementType::Select:
        case StatementType::Explain:
            return true;
        }

        return false;
    }

    StatementType Statement::GetType() {
        
        ub2 type = 0;

        ERROR_UTIL_CHECKTHROW(OCIAttrGet(m_sttmp, OCI_HTYPE_STMT, &type, (ub4*)0, OCI_ATTR_STMT_TYPE, GetOCIError()), m_connect, m_sttmText);
        
        StatementType result = static_cast<StatementType>(type);

        switch (result) {
        case StatementType::Unknown:
        case StatementType::Select:
        case StatementType::Update:
        case StatementType::Delete:
        case StatementType::Insert:
        case StatementType::Create:
        case StatementType::Drop:
        case StatementType::Alter:
        case StatementType::Begin:
        case StatementType::Declare:
        case StatementType::Explain:
        case StatementType::Merge:
        case StatementType::Rollback:
        case StatementType::Commit:
            break;
        default:
            throw OciException(type, "Oci20::Statement::GetType: statement type not supported!");
        }

        return result;
    }

    ub4 Statement::GetRowCount() {
        ub4 rows = 0;

        ERROR_UTIL_CHECKTHROW(OCIAttrGet(m_sttmp, OCI_HTYPE_STMT, &rows, (ub4*)0, OCI_ATTR_ROW_COUNT, GetOCIError()), m_connect, m_sttmText);

        return rows;
    }

    ub2 Statement::GetSQLFunctionCode() {
        ub2 fncode = 0;

        ERROR_UTIL_CHECKTHROW(OCIAttrGet(m_sttmp, OCI_HTYPE_STMT, &fncode, (ub4*)0, OCI_ATTR_SQLFNCODE, GetOCIError()), m_connect, m_sttmText);

        return fncode;
    }

    std::string Statement::GetSQLFunctionDescription() {

        return FunctionNames::Get(GetSQLFunctionCode());
    }

    bool Statement::IsStateInitialized() {
        ub4 state;

        ERROR_UTIL_CHECKTHROW(OCIAttrGet(m_sttmp, OCI_HTYPE_STMT, &state, (ub4*)0, OCI_ATTR_STMT_STATE, GetOCIError()), m_connect, m_sttmText);

        return state == OCI_STMT_STATE_INITIALIZED;
    }

    bool Statement::IsStateExecuted() {
        ub4 state;

        ERROR_UTIL_CHECKTHROW(OCIAttrGet(m_sttmp, OCI_HTYPE_STMT, &state, (ub4*)0, OCI_ATTR_STMT_STATE, GetOCIError()), m_connect, m_sttmText);

        return state == OCI_STMT_STATE_EXECUTED;
    }

    bool Statement::IsStateEndOfFetch() {
        ub4 state;

        ERROR_UTIL_CHECKTHROW(OCIAttrGet(m_sttmp, OCI_HTYPE_STMT, &state, (ub4*)0, OCI_ATTR_STMT_STATE, GetOCIError()), m_connect, m_sttmText);

        return state == OCI_STMT_STATE_END_OF_FETCH;
    }

    bool Statement::PossibleCompilationErrors() {
        return FunctionNames::PossibleCompilationErrors(GetSQLFunctionCode());
    }

    ub2 Statement::GetParseErrorOffset() {
        ub2 offset;

        ub4 attrType = OCI_ATTR_PARSE_ERROR_OFFSET;
        if (m_connect.GetClientVersion() == ClientVersion::Client80X)
            attrType = OCI80X_ATTR_PARSE_ERROR_OFFSET;

        // 10.09.2003 bug fix, 8.0.5 compatibility
        ERROR_UTIL_CHECKTHROW(OCIAttrGet(m_sttmp, OCI_HTYPE_STMT, &offset, (ub4*)0, attrType, GetOCIError()), m_connect, m_sttmText);
        return offset;
    }
}