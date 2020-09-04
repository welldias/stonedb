#ifndef __PROJECT_STONE_META_DICTIONARY_SYNONYM_H__
#define __PROJECT_STONE_META_DICTIONARY_SYNONYM_H__

#include <map>

#include "pls_code.h"

namespace MetaDictionary {

    class Synonym : public DbObject {
    public:
        Synonym() : DbObject() {};

        virtual bool IsLight() const { return true; };
        virtual const std::string GetTypeStr() const { return "SYNONYM"; };
        int Write(MetaStream& out, const MetaSettings& settings) const;

    protected:
        std::string m_refOwner;
        std::string m_refName;
        std::string m_refDBLink;

    private:
        // copy-constraction & assign-operation is not supported
        Synonym(const Synonym&);
        void operator = (const Synonym&);
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_SYNONYM_H__*/
