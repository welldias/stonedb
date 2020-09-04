#ifndef __PROJECT_STONE_META_DICTIONARY_FUNCTION_H__
#define __PROJECT_STONE_META_DICTIONARY_FUNCTION_H__

#include <map>

#include "pls_code.h"

namespace MetaDictionary {

    class Function : public PlsCode {
    public:
        Function() : PlsCode() {};

        virtual const std::string GetTypeStr() const { return "FUNCTION"; };
        virtual const std::string GetDefExt()  const { return "pls"; };

    private:
        // copy-constraction & assign-operation is not supported
        Function(const Function&);
        void operator = (const Function&);
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_FUNCTION_H__*/
