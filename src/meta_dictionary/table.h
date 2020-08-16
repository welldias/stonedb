#ifndef __PROJECT_STONE_META_DICTIONARY_TABLE_H__
#define __PROJECT_STONE_META_DICTIONARY_TABLE_H__

#include <map>

#include "table_base.h"
#include "constraint.h"

namespace MetaDictionary {

    class Table : public TableBase {
    private:
        using ColumnContainer = std::map<int, TabColumn*>;
        using PartKeyColumnContainer = std::vector<std::string>;
        using PartitionContainer = std::vector<Partition*>;

    public:
        Table();

        enum class Type {
            Normal,
            Temporary,
            Iot,
            IotOverflow
        };

        enum class TemporaryDuration {
            Unknown,
            Transaction, 
            Session,
        };

        void WriteDefinition(MetaStream& out, const MetaSettings& settings) const;
        void WriteComments(MetaStream& out, const MetaSettings& settings) const;
        void WritePrimaryKeyForIOT(MetaStream& out, const MetaSettings& settings) const;
        const Constraint* FindPrimaryKey() const;

        virtual bool IsGrantable() const { return true; }
        virtual const std::string GetTypeStr() const { return "TABLE"; };
        virtual bool IsGenerated() const { return m_type == Table::Type::IotOverflow; }
        virtual int Write(MetaStream& out, const MetaSettings& settings) const;

    private:
        void WriteColumns(MetaStream& out, const MetaSettings& settings) const;
        void WriteTempClause(MetaStream& out, const MetaSettings& settings) const;
        void WriteIOTClause(MetaStream& out, const MetaSettings& settings) const;
        void WriteClusterClause(MetaStream& out, const MetaSettings& settings) const;
        void WritePartitions(MetaStream& out, const MetaSettings& settings) const;
        void WriteSubpartitions(MetaStream& out, const MetaSettings& settings, const PartitionContainer&) const;

    private:
        Type m_type;
        TemporaryDuration m_temporaryDuration;
        ColumnContainer m_Columns;

        // should the following attributes be moved to TableBase?
        int m_iotOverflowIncludeColumn;

        Partition::Type          m_partitioningType;
        PartKeyColumnContainer   m_PartKeyColumns;
        PartitionContainer       m_Partitions;
        Partition::Type          m_subpartitioningType;
        PartKeyColumnContainer   m_subpartKeyColumns;
        PartitionContainer       m_subpartTemplates;

        Variable<bool> m_rowMovement;
        std::string m_interval; // for range partitioning

    private:
        // copy-constraction & assign-operation is not supported
        Table(const Table&);
        void operator = (const Table&);
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_TABLE_H__*/