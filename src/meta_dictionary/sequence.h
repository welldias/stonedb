#ifndef __PROJECT_STONE_META_DICTIONARY_SEQUENCE_H__
#define __PROJECT_STONE_META_DICTIONARY_SEQUENCE_H__

#include "db_object.h"

namespace MetaDictionary {

    class Sequence : public DbObject {
    public:
        Sequence();

        virtual bool IsGrantable() const { return true; }
        virtual const std::string GetTypeStr() const { return "SEQUENCE"; };
        virtual int Write(MetaStream& out, const MetaSettings& settings) const;

        std::string GetIdentityOptions(const MetaSettings& settings) const;

    public:
        std::string m_minValue;
        std::string m_maxValue;
        std::string m_incrementBy;
        std::string m_cacheSize;
        std::string m_lastNumber;
        bool m_cycleFlag;
        bool m_orderFlag;

    private:
        // copy-constraction & assign-operation is not supported
        Sequence(const Sequence&);
        void operator = (const Sequence&);
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_SEQUENCE_H__*/