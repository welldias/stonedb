#ifndef __PROJECT_STONE_META_DICTIONARY_CLUSTER_H__
#define __PROJECT_STONE_META_DICTIONARY_CLUSTER_H__

#include <map>

#include "db_object.h"
#include "table_storage.h"
#include "column.h"

namespace MetaDictionary {

    class Cluster : public DbObject, public TableStorage {
    public:
        Cluster();

        virtual bool IsGrantable() const { return true; }
        virtual const std::string GetTypeStr() const { return "CLUSTER"; };
        virtual int Write(MetaStream& out, const MetaSettings& settings) const;

    protected:
        bool m_isHash;
        std::string m_size;
        std::string m_hashKeys;
        std::string m_hashExpression;
        bool m_cache;

        std::map<int, Column*> m_columns;
        std::set<std::string>  m_indexes;

    private:
        // copy-constraction & assign-operation is not supported
        Cluster(const Cluster&);
        void operator = (const Cluster&);
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_CLUSTER_H__*/
