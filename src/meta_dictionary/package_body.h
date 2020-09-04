#ifndef __PROJECT_STONE_META_DICTIONARY_PACKAGE_BODY_H__
#define __PROJECT_STONE_META_DICTIONARY_PACKAGE_BODY_H__

#include <map>

#include "pls_code.h"

namespace MetaDictionary {

    class PackageBody : public PlsCode {
    public:
        PackageBody() : PlsCode() {};

        virtual const std::string GetTypeStr() const { return "PACKAGE BODY"; };
        virtual const std::string GetDefExt()  const { return "pls"; };

    private:
        // copy-constraction & assign-operation is not supported
        PackageBody(const PackageBody&);
        void operator = (const PackageBody&);
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_PACKAGE_BODY_H__*/
