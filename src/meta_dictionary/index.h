#ifndef __PROJECT_STONE_META_DICTIONARY_INDEX_H__
#define __PROJECT_STONE_META_DICTIONARY_INDEX_H__

#include <map>

#include "db_object.h"
#include "index_storage.h"
#include "table_storage.h"
#include "partition.h"
#include "index_partition.h"
#include "meta_stream.h"
#include "meta_settings.h"

namespace MetaDictionary {

    class Index : public DbObject, public IndexStorage {
    public:
        enum Type { 
            Normal, 
            Bitmap, 
            Cluster, 
            FunctionBased, 
            FunctionBasedBitmap, 
            IoitTop, 
            Domain 
        };

        Index();

        virtual const std::string& GetTypeStr() const { return "INDEX"; };
        virtual bool IsGenerated()              const { return m_generated; }
        virtual int Write(MetaStream& out, const MetaSettings& settings) const;

        void WriteIndexParams(MetaStream&, const MetaSettings& settings, bool skip_parallel = false) const;
        void WriteCompress(MetaStream&, const MetaSettings& settings) const;
        void WriteIOTClause(MetaStream&, const MetaSettings& settings, bool overflow) const;
        //void WriteDomainClause(MetaStream& out, const MetaSettings& settings) const;
        //void WriteIndexPartitions(MetaStream& out, const MetaSettings& settings) const;

    protected:
        std::string m_tableOwner;
        std::string m_tableName; // may be cluster

        std::string m_domainOwner; // 03.08.2003 improvement, domain index support
        std::string m_domain;
        std::string m_domainParams;

        Type m_type;
        bool m_uniqueness;
        bool m_temporary;
        bool m_generated;
        bool m_reverse;
        bool m_indexCompression;

        std::map<int, std::string> m_columns; // or expression for eitFunctionBased
        std::map<int, bool>        m_isExpression;

        TableStorage m_iOTOverflow_Storage;
        int          m_iOTOverflow_PCTThreshold;

        Partition::Type              m_partitioningType;
        std::vector<std::string>     m_partKeyColumns;
        std::vector<IndexPartition*> m_partitions;
        Partition::Type              m_subpartitioningType;
        std::string                  m_interval; // for range partitioning

        std::vector<TableStorage*>   m_iOTOverflowPartitions;
        TableStorage                 m_iOTOverflowPartitionDefaultStrorage;

        bool m_local;

    private:
        void WriteDomainClause(MetaStream& out, const MetaSettings& settings) const;
        void WriteIndexPartitions(MetaStream& out, const MetaSettings& settings) const;
        void WriteSubpartitions(MetaStream& out, const MetaSettings& settings, const PartitionContainer& subpartitions) const;

        // copy-constraction & assign-operation is not supported
        Index(const Index&);
        void operator = (const Index&);
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_INDEX_H__*/