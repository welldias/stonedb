#ifndef __PROJECT_STONE_META_DICTIONARY_SNAPSHOT_H__
#define __PROJECT_STONE_META_DICTIONARY_SNAPSHOT_H__

#include <map>

#include "db_object.h"
#include "table_storage.h"

namespace MetaDictionary {

    class Snapshot : public DbObject, public TableStorage {

    public:
        Snapshot() : DbObject() {};

        virtual bool IsGrantable() const { return true; }
        virtual const std::string GetTypeStr() const { return "SNAPSHOT"; };
        virtual int Write(MetaStream& out, const MetaSettings& settings) const;

    private:
        std::string m_refreshType;
        std::string m_startWith;
        std::string m_nextTime;
        std::string m_query;
        std::string m_clusterName;
        std::string m_comments;
        std::map<int, std::string> m_clusterColumns;

    private:
        // copy-constraction & assign-operation is not supported
        Snapshot(const Snapshot&);
        void operator = (const Snapshot&);
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_SNAPSHOT_H__*/