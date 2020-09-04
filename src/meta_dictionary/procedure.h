#ifndef __PROJECT_STONE_META_DICTIONARY_PROCEDURE_H__
#define __PROJECT_STONE_META_DICTIONARY_PROCEDURE_H__

#include <map>

#include "pls_code.h"

namespace MetaDictionary {

    class Procedure : public PlsCode {
    public:
        Procedure() : PlsCode() {};

        virtual const std::string GetTypeStr() const { return "PROCEDURE"; };
        virtual const std::string GetDefExt()  const { return "pls"; };

    private:
        // copy-constraction & assign-operation is not supported
        Procedure(const Procedure&);
        void operator = (const Procedure&);
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_PROCEDURE_H__*/
