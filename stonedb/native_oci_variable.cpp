#include "native_oci_variable.h"
#include "error_util.h"

namespace Oci20 {

    NativeOciVariable::NativeOciVariable(OCIEnv* ociEnv, ub2 sqltType, ub2 ociHandleType)
        : Variable(0, sqltType, 0), m_oci_handle_type(ociHandleType) {
        
        sword result = OCIDescriptorAlloc(ociEnv, &m_buffer, m_oci_handle_type, 0, 0);
        if (result == OCI_INVALID_HANDLE) {
            throw OciException(result, "Oci20::NativeOciVariable: out-of-memory error!");
        }
    }

    NativeOciVariable::~NativeOciVariable() {
        OCIDescriptorFree(m_buffer, m_oci_handle_type);
        m_buffer = 0;
    }

    sword NativeOciVariable::Define(OCIStmt* ociStmt, OCIError* ociStmError, int pos) {
        OCIDefine* defhp = NULL;
        return OCIDefineByPos(ociStmt, &defhp, ociStmError, pos, &m_buffer, 0, m_type, &m_indicator, 0, 0, OCI_DEFAULT);
    }
}