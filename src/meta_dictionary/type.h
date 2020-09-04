#ifndef __PROJECT_STONE_META_DICTIONARY_TYPE_H__
#define __PROJECT_STONE_META_DICTIONARY_TYPE_H__

#include <map>

#include "pls_code.h"


namespace MetaDictionary {

    class Type : public PlsCode {
    public:
        Type() : PlsCode() {};

        void WriteAsIncopmlete(MetaStream& out, const MetaSettings& settings) const;

        virtual const std::string GetTypeStr() const { return "TYPE"; };
        virtual const std::string GetDefExt()  const { return "pls"; };

    private:
        // copy-constraction & assign-operation is not supported
        Type(const Type&);
        void operator = (const Type&);
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_TYPE_H__*/
