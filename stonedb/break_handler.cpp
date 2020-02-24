#include "break_handler.h"

namespace Oci20 {
    BreakHandler::~BreakHandler() {
        OCIHandleFree(m_errhp, OCI_HTYPE_ERROR);
    }

    void BreakHandler::Break() {
        m_break_time = SystemClock::StartCount();
        OCIBreak(m_svchp, m_errhp);
    }

    bool BreakHandler::IsCalledIn(unsigned int ms) {

        Clock64 result = static_cast<Clock64>(SystemClock::StopCount(m_break_time));
        if (result < ms)
            return true;

        return false;
    }
}