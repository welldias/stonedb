#include "dummy_var.h"

namespace Oci20 {
    
    const std::string DummyVar::m_unsupportedStr = "<Unsupported>";
    const std::string DummyVar::m_skippedStr = "<Skipped>";

    DummyVar::DummyVar(bool skipped)
        : Variable(0, 0), m_skipped(skipped) {
    }

    sword DummyVar::GetString(std::string& strbuff, const std::string& null) const {
        strbuff = m_skipped ? m_skippedStr : m_unsupportedStr;

        return OCI_SUCCESS;
    }


}
