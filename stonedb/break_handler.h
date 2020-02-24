#ifndef __PROJECT_STONE_OCI20_BREAKHANDLER_H__
#define __PROJECT_STONE_OCI20_BREAKHANDLER_H__

#include <oci.h>

#include "system_clock.h"

namespace Oci20 {

    using namespace Utils;


	class BreakHandler {
        OCISvcCtx* m_svchp;
        OCIError* m_errhp;
        Clock64 m_break_time;

    public:
        BreakHandler(OCISvcCtx* svchp, OCIError* errhp) 
            : m_svchp(svchp), m_errhp(errhp), m_break_time(0) {}

        ~BreakHandler();

        void Break();
        bool IsCalledIn(unsigned int ms);

        void* GetCS() { return NULL; }

    private:
        BreakHandler(const BreakHandler&);
        void operator = (const BreakHandler&);
	};
}

#endif /*__PROJECT_STONE_OCI20_BREAKHANDLER_H__*/