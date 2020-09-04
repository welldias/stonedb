#ifndef __PROJECT_STONE_META_DICTIONARY_TYPE_BODY_H__
#define __PROJECT_STONE_META_DICTIONARY_TYPE_BODY_H__

#include <map>

#include "pls_code.h"


namespace MetaDictionary {

    class TypeBody : public PlsCode {
    public:
        TypeBody() : PlsCode() {};

        virtual const std::string GetTypeStr() const { return "TYPE BODY"; };
        virtual const std::string GetDefExt()  const { return "plb"; };

    private:
        // copy-constraction & assign-operation is not supported
        TypeBody(const TypeBody&);
        void operator = (const TypeBody&);
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_TYPE_BODY_H__*/
