#ifndef __PROJECT_STONE_META_DICTIONARY_PLS_CODE_H__
#define __PROJECT_STONE_META_DICTIONARY_PLS_CODE_H__

#include <map>

#include "db_object.h"

namespace MetaDictionary {

    class PlsCode : public DbObject {
    public:
        PlsCode() {};

        virtual bool IsGrantable() const { return true; }
        virtual bool IsCode() const { return true; };
        virtual const std::string GetTypeStr() const { return "sql"; };
        virtual int Write(MetaStream& out, const MetaSettings& settings) const;

    public:
        std::string m_type;
        std::string m_text;

    private:
        // copy-constraction & assign-operation is not supported
        PlsCode(const PlsCode&);
        void operator = (const PlsCode&);
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_PLS_CODE_H__*/
