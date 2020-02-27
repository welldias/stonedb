#ifndef __PROJECT_STONE_OCI20_NATIVEOCIVARIABLE_H__
#define __PROJECT_STONE_OCI20_NATIVEOCIVARIABLE_H__

#include <oci.h>

#include <string>

#include "variable.h"

namespace Oci20 {

    class NativeOciVariable : public Variable {
    protected:
        NativeOciVariable(OCIEnv* ociEnv, ub2 sqltType, ub2 ociHandleType);
        ~NativeOciVariable();

        virtual sword Define(OCIStmt* ociStmt, OCIError* ociStmError, int pos);

        ub2 m_oci_handle_type;
    };
}

#endif // __PROJECT_STONE_OCI20_NATIVEOCIVARIABLE_H__
