#include "ref_cursor_variable.h"
#include "error_util.h"

namespace Oci20 {

	RefCursorVariable::RefCursorVariable(Connect& connect, int prefetch)
		: Variable(0, static_cast<ub2>(SQLT_RSET), 0),
		m_connect(connect),
		m_sttmp(0),
		m_prefetch(prefetch) {

		ERROR_UTIL_CHECKALLOC(OCIHandleAlloc(GetOCIEnv(), (dvoid**)&m_sttmp, OCI_HTYPE_STMT, 0, 0));
	}

	RefCursorVariable::~RefCursorVariable() {
		try {
			if (m_sttmp)
				OCIHandleFree(m_sttmp, OCI_HTYPE_STMT);
			m_sttmp = 0;
		}
		catch (...) {}
	}

	sword RefCursorVariable::GetString(std::string& str, const std::string& null) const {
		str = "Use PRINT varible to see the result set";

		return OCI_SUCCESS;
	}

	sword RefCursorVariable::Bind(OCIStmt* ociStmt, OCIError* ociStmError, int pos) {
		sword result = OCI_SUCCESS;
		//8171  1315603 when fetching NULL data.
		//              ie: indp / rcodep may be incorrect.
		//              Workaround: Set OCI_ATTR_PREFETCH_ROWS to 0
		//              This problem was introduced in 8.1.6
		if (m_connect.GetClientVersion() >= Connect::ClientVersion::Client9X)
			result = OCIAttrSet(m_sttmp, OCI_HTYPE_STMT, &m_prefetch, 0, OCI_ATTR_PREFETCH_ROWS, GetOCIError());

		if (result == OCI_SUCCESS) {
			OCIBind* bndhp;
			result = OCIBindByPos(ociStmt, &bndhp, ociStmError, pos, &m_sttmp, 0, SQLT_RSET, 0, 0, 0, 0, 0, OCI_DEFAULT);
		}

		return result;
	}

	sword RefCursorVariable::Bind(OCIStmt* ociStmt, OCIError* ociStmError, const std::string& name) {

		OCIBind* bndhp;
		return OCIBindByName(ociStmt, &bndhp, ociStmError, (OraText*)name.c_str(), static_cast<ub4>(name.length()), &m_sttmp, 0, SQLT_RSET, 0, 0, 0, 0, 0, OCI_DEFAULT);
	}

	bool RefCursorVariable::IsStateInitialized() {
		ub4 state;

		ERROR_UTIL_CHECKTHROW(OCIAttrGet(m_sttmp, OCI_HTYPE_STMT, &state, (ub4*)0, OCI_ATTR_STMT_STATE, GetOCIError()), m_connect, "");

		return state == OCI_STMT_STATE_INITIALIZED;
	}

	bool RefCursorVariable::IsStateExecuted() {
		ub4 state;

		ERROR_UTIL_CHECKTHROW(OCIAttrGet(m_sttmp, OCI_HTYPE_STMT, &state, (ub4*)0, OCI_ATTR_STMT_STATE, GetOCIError()), m_connect, "");

		return state == OCI_STMT_STATE_EXECUTED;
	}

	bool RefCursorVariable::IsStateEndOfFetch() {
		ub4 state;

		ERROR_UTIL_CHECKTHROW(OCIAttrGet(m_sttmp, OCI_HTYPE_STMT, &state, (ub4*)0, OCI_ATTR_STMT_STATE, GetOCIError()), m_connect, "");

		return state == OCI_STMT_STATE_END_OF_FETCH;
	}
}