#ifndef __PROJECT_STONE_META_DICTIONARY_PACKAGE_H__
#define __PROJECT_STONE_META_DICTIONARY_PACKAGE_H__

#include <map>

#include "pls_code.h"

namespace MetaDictionary {

    class Package : public PlsCode {
    public:
        Package() : PlsCode() {};

        virtual const std::string GetTypeStr() const { return "PACKAGE"; };
        virtual const std::string GetDefExt()  const { return "pls"; };

    private:
        // copy-constraction & assign-operation is not supported
        Package(const Package&);
        void operator = (const Package&);
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_PACKAGE_H__*/
