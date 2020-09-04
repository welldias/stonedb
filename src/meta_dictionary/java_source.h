#ifndef __PROJECT_STONE_META_DICTIONARY_JAVA_SOURCE_H__
#define __PROJECT_STONE_META_DICTIONARY_JAVA_SOURCE_H__

#include <map>

#include "pls_code.h"

namespace MetaDictionary {

    class JavaSource : public PlsCode {
    public:
        JavaSource() : PlsCode() {};

        virtual const std::string GetTypeStr() const { return "JAVA SOURCE"; };
        virtual const std::string GetDefExt()  const { return "java"; };

        int Write(MetaStream& out, const MetaSettings& settings) const;

    private:
        // copy-constraction & assign-operation is not supported
        JavaSource(const JavaSource&);
        void operator = (const JavaSource&);
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_JAVA_SOURCE_H__*/
