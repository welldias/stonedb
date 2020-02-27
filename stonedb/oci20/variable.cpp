#include "variable.h"
#include "error_util.h"

namespace Oci20 {

    const std::string Variable::m_null;

	Variable::Variable(ub2 type, sb4 size) {
        m_type = type;
        m_indicator = OCI_IND_NULL;
        m_buffer_size = m_size = size;
        m_buffer = new char[m_size];
        memset(m_buffer, 0, m_size);
        m_owner = true;
        m_ret_size = 0;
	}

    Variable::Variable(dvoid* buffer, ub2 type, sb4 size) {
        m_type = type;
        m_indicator = OCI_IND_NULL;
        m_buffer_size = m_size = size;
        m_buffer = buffer;
        m_owner = false;
        m_ret_size = 0;
    }

    Variable::~Variable() {
		if (m_owner)
			delete[] std::exchange(m_buffer, nullptr);
	}

    sword Variable::GetString(std::string&, const std::string& ) const {
        throw OciException(0, "Oci20::Variable::GetString(): method is not implemented.");
    }

    sword Variable::Define(OCIStmt* ociStmt, OCIError* ociStmError, int pos) {

        OCIDefine* defhp = NULL;

        return OCIDefineByPos(ociStmt, &defhp, ociStmError, pos, m_buffer, m_buffer_size, m_type, &m_indicator, &m_ret_size, 0, OCI_DEFAULT);
    }

    /** @brief Bind variable by position.
     *
     *  @arg sttm: Statement to bind - must be prepared.
     *  @arg pos: Position of bind variable within statement.
     */
    sword Variable::Bind(OCIStmt* ociStmt, OCIError* ociStmError, int pos) {
        
        OCIBind* bndhp = NULL;
        sword result = OCI_SUCCESS;

        switch (m_type)
        {
        case SQLT_CLOB:
        case SQLT_BLOB:
        case SQLT_BFILEE:
        case SQLT_CFILEE:
            result = OCIBindByPos(ociStmt, &bndhp, ociStmError, pos, &m_buffer, m_size, m_type, &m_indicator, 0, 0, 0, 0, OCI_DEFAULT);
            break;
        default:
            result = OCIBindByPos(ociStmt, &bndhp, ociStmError, pos, m_buffer, m_size, m_type, &m_indicator, 0, 0, 0, 0, OCI_DEFAULT);
            break;
        }

        return result;
    }

    /** @brief Bind variable by name.
     *
     *  @arg sttm: Statement to bind - must be prepared.
     *  @arg name: Name of bind variable.
     */
    sword Variable::Bind(OCIStmt* ociStmt, OCIError* ociStmError, const std::string& name) {
        
        OCIBind* bndhp = NULL;
        sword result = OCI_SUCCESS;

        switch (m_type)
        {
        case SQLT_CLOB:
        case SQLT_BLOB:
        case SQLT_BFILEE:
        case SQLT_CFILEE:
            result = OCIBindByName(ociStmt, &bndhp, ociStmError, (OraText*)name.c_str(), static_cast<ub4>(name.length()), &m_buffer, m_size, m_type, &m_indicator, 0, 0, 0, 0, OCI_DEFAULT);
            break;
        default:
            result = OCIBindByName(ociStmt, &bndhp, ociStmError, (OraText*)name.c_str(), static_cast<ub4>(name.length()), m_buffer, m_size, m_type, &m_indicator, 0, 0, 0, 0, OCI_DEFAULT);
            break;
        }

        return result;
    }
}