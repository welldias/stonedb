#ifndef __PROJECT_STONE_OCI20_DUMMYVAR_H__
#define __PROJECT_STONE_OCI20_DUMMYVAR_H__

#include "variable.h"

namespace Oci20 {

	class DummyVar : public Variable {
    public:
        DummyVar(bool skipped = false);

        virtual sword GetString(std::string&, const std::string& null = m_null) const;

    public:
        bool m_skipped;
        static const std::string m_unsupportedStr;
        static const std::string m_skippedStr;
	};
}

#endif /* __PROJECT_STONE_OCI20_DUMMYVAR_H__*/