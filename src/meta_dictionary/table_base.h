#ifndef __PROJECT_STONE_META_DICTIONARY_TABLEBASE_H__
#define __PROJECT_STONE_META_DICTIONARY_TABLEBASE_H__

#include <map>

#include "db_object.h"
#include "index_storage.h"
#include "table_storage.h"
#include "partition.h"
#include "index_partition.h"
#include "meta_stream.h"
#include "meta_settings.h"

namespace MetaDictionary {

    class TableBase : public DbObject, public TableStorage {
    public:
        TableBase();

        void WriteIndexes(MetaStream& out, const MetaSettings& settings) const;
        void WriteConstraints(MetaStream& out, const MetaSettings& settings, char chType) const;
        void WriteTriggers(MetaStream& out, const MetaSettings& settings) const;

    protected:
        std::string m_clusterName;
        std::string m_comments;
        Variable<bool> m_cache;

        std::set<std::string> m_constraints;
        std::set<std::string> m_indexes;
        std::set<std::string> m_triggers;

        std::map<int, std::string> m_clusterColumns; // map table to cluster columns

    private:
        // copy-constraction & assign-operation is not supported
        TableBase(const TableBase&);
        void operator = (const TableBase&);
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_TABLEBASE_H__*/