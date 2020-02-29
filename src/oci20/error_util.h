#ifndef __PROJECT_STONE_OCI20_ERROR_UTIL_H__
#define __PROJECT_STONE_OCI20_ERROR_UTIL_H__

#include <oci.h>

#include "connect.h"

#include "../utils/utils.h"

namespace Oci20 {

    using namespace Utils;

#define ERROR_UTIL_CHECKTHROW(s,c,m) ErrorUtil::CheckErrorAndThrow(s,c,m)
#define ERROR_UTIL_CHECKALLOC(s) ErrorUtil::CheckAllocErrorAndThrow(s)
#define ERROR_UTIL_CHECKINTERRUPT(c) ErrorUtil::CheckInterruptAndThrow(c)

    class ErrorUtil {
    public:
        static void CheckErrorAndThrow(sword status, Connect& connectBase, const std::string& exceptionMsg);
        static void CheckAllocErrorAndThrow(sword status);
        static void CheckInterruptAndThrow(Connect& connectBase);
    };

    class OciException : public Exception {
    public:
        OciException(int code, const std::string& msg) : Exception(code, msg) {}
    };
}

#endif // __PROJECT_STONE_OCI20_ERROR_UTIL_H__
