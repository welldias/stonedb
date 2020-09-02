#ifndef __PROJECT_STONE_META_DICTIONARY_TRIGGER_H__
#define __PROJECT_STONE_META_DICTIONARY_TRIGGER_H__

#include <map>

#include "db_object.h"

namespace MetaDictionary {

    class Trigger : public DbObject {

    public:
        Trigger() {};

        virtual bool IsCode()     const { return true; };
        virtual const std::string GetDefExt()  const { return "trg"; };
        virtual const std::string GetTypeStr() const { return "TRIGGER"; };
        virtual int Write(MetaStream& out, const MetaSettings& settings) const;

    private:
        std::string m_tableOwner;
        std::string m_tableName;
        std::string m_description;
        std::string m_whenClause;
        std::string m_refNames;
        std::string m_triggerBody;
        std::string m_status;
        std::string m_actionType;
        std::string m_baseObjectType;

    private:
        // copy-constraction & assign-operation is not supported
        Trigger(const Trigger&);
        void operator = (const Trigger&);
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_TRIGGER_H__*/