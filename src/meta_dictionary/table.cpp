#include "table.h"
#include "index.h"
#include "sequence.h"
#include "string_stream.h"

namespace MetaDictionary {

    Table::Table() {
        m_type = Table::Type::Normal;
        m_temporaryDuration = Table::TemporaryDuration::Unknown;
        m_partitioningType = Partition::Type::None;
        m_subpartitioningType = Partition::Type::None;
        m_iotOverflowIncludeColumn = 0;
    }

    void Table::WriteDefinition(MetaStream& out, const MetaSettings& settings) const {
        if (settings.GeneratePrompts) {
            out.PutIndent();
            out.Put("PROMPT CREATE TABLE ");
            out.PutOwnerAndName(m_owner, m_name, settings.SchemaName);
            out.NewLine();
        }

        out.PutIndent();
        out.Put("CREATE");
        if (settings.SQLPlusCompatibility) {
            out.Put(" ");
        }
        else {
            out.NewLine();
            out.PutIndent();
        }

        if (m_type == Table::Type::Temporary)
            out.Put("GLOBAL TEMPORARY ");

        out.Put("TABLE ");
        out.PutOwnerAndName(m_owner, m_name, settings.SchemaName);
        out.Put(" (");
        if (!settings.SQLPlusCompatibility && settings.CommentsAfterColumn && !m_comments.empty())
        {
            out.Put(" -- ");
            out.Put(m_comments);
        }
        out.NewLine();

        out.MoveIndent(2);

        WriteColumns(out, settings);

        if (m_type == Table::Type::Iot)
            WritePrimaryKeyForIOT(out, settings);

        out.MoveIndent(-2);
        out.PutLine(")");

        out.MoveIndent(2);

        if (settings.StorageSubstitutedClause) {
            if (m_type == Table::Type::Temporary)
                WriteTempClause(out, settings);
            else
                DbObject::WriteSubstitutedClause(out, m_name, "_st");
        }
        else {
            if (m_type == Table::Type::Temporary)
                WriteTempClause(out, settings);
            else if (!m_clusterName.empty())
                WriteClusterClause(out, settings);
            else if (m_type == Table::Type::Iot)
                WriteIOTClause(out, settings);
            else
            {
                TableStorage defStorage;
                //TODO: Isto esta horrivel, verificar forma mais elegante para resolver essa amarra.
                //BuildDefault(defStorage, m_Dictionary, m_strOwner, m_strTablespaceName.GetValue());
                WriteStorage(out, defStorage, settings);

                if (m_partitioningType != Partition::Type::None)
                    WritePartitions(out, settings);
            }

            if (!m_cache.IsNull() && m_cache.GetValue())
                out.PutLine("CACHE");
        }

        out.MoveIndent(-2);

        out.PutLine("/");
        out.NewLine();
    }

    void Table::WriteColumns(MetaStream& out, const MetaSettings& settings) const {
        
        int nMaxNameLen(0);     // max column name length
        int nMaxTypeLen(0);     // max type-specification length
        int nMaxOtherLen(0);    // max default-specification and null-specification length

        std::list<std::string> listTypeSpecs;

        int i = 0;
        for (auto it : m_columns) {

            //const TableColumn& column = *it.second;
            auto &column = *it.second;

            std::string type;
            column.GetSpecString(type, settings);
            listTypeSpecs.push_back(type);

            nMaxNameLen = std::max<int>(nMaxNameLen, static_cast<int>(column.GetColumnName().size()));
            nMaxTypeLen = std::max<int>(nMaxTypeLen, static_cast<int>(type.size()));

            size_t nLength = 0;
            if (column.GetIdentity()) {
                Sequence sequence;
                //TODO: Isto esta horrivel, verificar forma mais elegante para resolver essa amarra.
                //sequence = m_Dictionary.LookupSequence(m_owner, column.GetSequenceName());
                std::string identityClause = " GENERATED " + column.GetIndentityGenerationType() + " AS IDENTITY";
                std::string identityOptions = sequence.GetIdentityOptions(settings);
                if (!identityOptions.empty())
                    identityClause += " (" + identityOptions + ")";
                nLength += identityClause.length();
            }
            else if (!column.GetDataDefault().empty()) {
                nLength += ((column.GetVirtual()) ? sizeof(" AS ()") : sizeof(" DEFAULT ")) - 1
                    + column.GetDataDefault().size();
            }

            if (!column.GetIdentity())
            {
                nLength += ((column.GetNullable()) ? sizeof(" NULL") : sizeof(" NOT NULL")) - 1;
            }

            if (i < (m_columns.size() - 1))
                nLength++;      // add place for ',' 

            nMaxOtherLen = std::max<int>(nMaxOtherLen, static_cast<int>(nLength));
            i++;
        }

        auto itTypes = listTypeSpecs.begin();

        i = 0;
        for (auto it : m_columns) {

            //const TableColumn& column = *it.second;
            auto& column = *it.second;

            out.PutIndent();
            auto nPos = out.GetPosition();
            out.SafeWriteDBName(column.GetColumnName());

            auto j = column.GetColumnName().size();
            do out.Put(" "); while (++j < nMaxNameLen + 1);

            out.Put(*itTypes);
            j = (*itTypes).size();
            if (column.GetIdentity()) {
                while (j++ < nMaxTypeLen) out.Put(" ");

                Sequence sequence;
                //TODO: Isto esta horrivel, verificar forma mais elegante para resolver essa amarra.
                //Sequence& seq = m_Dictionary.LookupSequence(m_strOwner.c_str(), column.m_strSequenceName.c_str());
                std::string identityClause = " GENERATED " + column.GetIndentityGenerationType() + " AS IDENTITY";
                std::string identityOptions = sequence.GetIdentityOptions(settings);
                if (!identityOptions.empty())
                    identityClause += " (" + identityOptions + ")";
                out.Put(identityClause);
            }
            else if (column.GetVirtual() || !column.GetDataDefault().empty()) {
                while (j++ < nMaxTypeLen) out.Put(" ");
                out.Put(column.GetVirtual() ? " AS (" : " DEFAULT ");
                out.Put(column.GetDataDefault());
                if (column.GetVirtual()) out.Put(")");
            }

            if (!column.GetIdentity()) {
                while (j++ < nMaxTypeLen) out.Put(" ");
                out.Put(column.GetNullable() ? " NULL" : " NOT NULL");
            }

            if (i < (m_columns.size() - 1) || m_type == Table::Type::Iot)
                out.Put(",");

            if (settings.CommentsAfterColumn && !column.GetComments().empty()) {
                auto j = out.GetPosition() - nPos;
                int nMaxLen = nMaxNameLen + nMaxTypeLen + nMaxOtherLen + 2;
                nMaxLen = std::max<int>(nMaxLen, settings.CommentsPos);

                while (j++ < nMaxLen)
                    out.Put(" ");

                out.Put("-- ");
                out.Put(column.GetComments());
            }

            out.NewLine();
        }
    }

    void Table::WriteTempClause(MetaStream& out, const MetaSettings& settings) const {

        out.PutIndent();

        if (m_temporaryDuration == Table::TemporaryDuration::Transaction)
            out.Put("ON COMMIT DELETE ROWS");
        else if (m_temporaryDuration == Table::TemporaryDuration::Session)
            out.Put("ON COMMIT PRESERVE ROWS");
        else
            throw Exception("Temporary table error: invalid duration!");

        out.NewLine();
    }

    void Table::WriteIOTClause(MetaStream& out, const MetaSettings& settings) const {

        const Constraint* pk = FindPrimaryKey();
        if(pk == NULL)
            throw Exception("IOT error: PK not found!");

        Index index;
        //TODO: Isto esta horrivel, verificar forma mais elegante para resolver essa amarra.
        //const Index& index = m_dictionary.LookupIndex(pk->m_strOwner.c_str(), pk->m_strName.c_str());

        out.PutIndent();
        out.Put("ORGANIZATION INDEX");

        if (m_iotOverflowIncludeColumn > 0) {

            out.Put(" INCLUDING ");
            
            auto it = m_columns.find(m_iotOverflowIncludeColumn - 1);
            if (it != m_columns.end() == NULL)
                throw Exception("IOT error: Invalid include column!");
            out.SafeWriteDBName(it->second->GetColumnName());
        }
        out.NewLine();

        index.WriteIOTClause(out, settings, m_iotOverflowIncludeColumn > 0);
    }

    void Table::WriteClusterClause(MetaStream& out, const MetaSettings& settings) const {
        out.PutIndent();
        out.Put("CLUSTER ");
        out.SafeWriteDBName(m_clusterName);
        out.Put(" (");
        out.NewLine();
        out.WriteColumns(m_clusterColumns, 2);
        out.PutLine(")");
    }

    void Table::WritePartitions(MetaStream& out, const MetaSettings& settings) const {

        switch (m_partitioningType) {
        case Partition::Type::Range:
        case Partition::Type::Hash:
        case Partition::Type::List:
            break;
        default:
            throw Exception("Table writting error: unsupported partition type!");
        }

        switch (m_partitioningType) {
        case Partition::Type::Range:
            out.PutLine("PARTITION BY RANGE (");
            out.WriteColumns(m_partKeyColumns, 2, true);
            out.PutLine(")");
            break;
        case Partition::Type::Hash:
            out.PutLine("PARTITION BY HASH (");
            out.WriteColumns(m_partKeyColumns, 2, true);
            out.PutLine(")");
            break;
        case Partition::Type::List:
            out.PutIndent();
            out.Put("PARTITION BY LIST (");
            out.SafeWriteDBName(!m_partKeyColumns.empty() ? *m_partKeyColumns.begin() : std::string());
            out.Put(")");
            out.NewLine();
            break;
        }

        if (!m_interval.empty()) {
            out.PutIndent();
            out.Put("INTERVAL (");
            out.Put(m_interval);
            out.Put(")");
            out.NewLine();
        }

        switch (m_partitioningType) {
        case Partition::Type::Range:
            out.PutLine("SUBPARTITION BY RANGE (");
            out.WriteColumns(m_subpartKeyColumns, 2, true);
            out.PutLine(")");
            break;
        case Partition::Type::Hash:
            out.PutLine("SUBPARTITION BY HASH (");
            out.WriteColumns(m_subpartKeyColumns, 2, true);
            out.PutLine(")");
            break;
        case Partition::Type::List:
            out.PutIndent();
            out.Put("SUBPARTITION BY LIST (");
            out.SafeWriteDBName(!m_subpartKeyColumns.empty() ? *m_subpartKeyColumns.begin() : std::string());
            out.Put(")");
            out.NewLine();
            break;
        }

        if (!m_subpartTemplates.empty()) {
            out.PutLine("SUBPARTITION TEMPLATE");
            out.PutLine("(");
            out.MoveIndent(2);

            int nMaxNameLen = 0;
            for (auto it : m_subpartTemplates)
                nMaxNameLen = std::max<int>(nMaxNameLen, static_cast<int>(it->GetName().length()));

            int i = 0;
            auto count = m_subpartTemplates.size();
            for (auto it : m_subpartTemplates) {
                out.PutIndent();
                out.Put("SUBPARTITION ");
                out.SafeWriteDBName(it->GetName());

                if (m_subpartitioningType != Partition::Type::Hash) {
                    auto j = it->GetName().size();
                    do out.Put(" "); while (++j < nMaxNameLen + 1);

                    out.Put(m_subpartitioningType == Partition::Type::Range ? "VALUES LESS THAN (" : "VALUES (");
                    out.Put(it->GetHighValue());
                    out.Put(")");
                }

                if (!it->m_tablespaceName.IsNull())
                {
                    out.Put(" TABLESPACE ");
                    out.SafeWriteDBName(it->m_tablespaceName.GetValue());
                }

                if ((i < count - 1)) out.Put(",");
                out.NewLine();

                i++;
            }

            out.MoveIndent(-2);
            out.PutLine(")");
        }

        out.PutLine("(");
        out.MoveIndent(2);

        int nMaxNameLen = 0;
        for (auto it : m_partitions)
            nMaxNameLen = std::max<int>(nMaxNameLen, static_cast<int>(it->GetName().length()));

        int i = 0;
        auto count = m_partitions.size();
        for (auto it : m_partitions) {

            out.PutIndent();
            out.Put("PARTITION ");
            out.SafeWriteDBName(it->GetName());

            if (m_partitioningType != Partition::Type::Hash) {
                auto j = it->GetName().size();
                do out.Put(" "); while (++j < nMaxNameLen + 1);

                out.Put(m_partitioningType == Partition::Type::Range ? "VALUES LESS THAN (" : "VALUES (");
                out.Put(it->GetHighValue());
                out.Put(")");
            }

            switch (m_partitioningType)
            {
            case Partition::Type::Range:
            case Partition::Type::List:
            {
                StringStream out2(true);
                out2.SetLike(out);
                out2.MoveIndent(2);

                TableStorage defStorage;
                //TODO: Isto esta horrivel, verificar forma mais elegante para resolver essa amarra.
                //BuildDefault(defStorage, m_dictionary, m_owner, it->m_tablespaceName.GetValue());
                defStorage.CopyNotNulls(*this); // copy defaults defined on table level

                it->WriteStorage(out2, defStorage, settings);
                out2.TrimRight(" \n\r");

                if (out2.GetLength()) {
                    out.NewLine();
                    out.Put(out2.GetString());
                }
            }
            break;
            case Partition::Type::Hash: // only tablespace is supported for hash partition
            {
                if (!it->m_tablespaceName.IsNull() &&
                    (
                        settings.IsStorageAlways()
                        || settings.AlwaysPutTablespace
                        //TODO: Isto esta horrivel, verificar forma mais elegante para resolver essa amarra.
                        //|| !isDefaultTablespace(m_dictionary, it->m_tablespaceName.GetValue(), m_owner)
                        )
                    )
                {
                    out.Put(" TABLESPACE ");
                    out.SafeWriteDBName(it->m_tablespaceName.GetValue());
                }
            }
            break;
            }

            if (!it->GetSubpartitions().empty())
                Table::WriteSubpartitions(out, settings, it->GetSubpartitions());

            if ((i < count - 1)) out.Put(",");
            out.NewLine();

            i++;
        }

        out.MoveIndent(-2);
        out.PutLine(")");

        if (!m_rowMovement.IsNull() && m_rowMovement.GetValue())
            out.PutLine("ENABLE ROW MOVEMENT");
    }

    void Table::WriteSubpartitions(MetaStream& out, const MetaSettings& settings, const PartitionContainer& subpartitions) const {
       
        bool is_default = !settings.IsStorageAlways() && !settings.AlwaysPutTablespace;

        if (is_default) {
            if (subpartitions.size() != m_subpartTemplates.size())
                is_default = false;

            PartitionContainer::const_iterator it1 = m_subpartTemplates.begin();
            PartitionContainer::const_iterator it2 = subpartitions.begin();

            for (; it1 != m_subpartTemplates.end() && it2 != subpartitions.end(); ++it1, ++it2) {
                if ((*it1)->GetHighValue() != (*it2)->GetHighValue()) {
                    is_default = false;
                    break;
                }
                if (!(*it1)->m_tablespaceName.IsNull() && (*it1)->m_tablespaceName != (*it2)->m_tablespaceName
                    || (*it1)->m_tablespaceName.IsNull() && m_tablespaceName != (*it2)->m_tablespaceName) {
                    is_default = false;
                    break;
                }
            }
        }

        if (is_default)
            return;

        out.NewLine();
        out.MoveIndent(2);
        out.PutLine("(");
        out.MoveIndent(2);

        int nMaxNameLen = 0;
        for (PartitionContainer::const_iterator it = subpartitions.begin(); it != subpartitions.end(); ++it)
            nMaxNameLen = std::max<int>(nMaxNameLen, static_cast<int>((*it)->GetName().length()));

        int i = 0;
        auto count = subpartitions.size();
        for (PartitionContainer::const_iterator it = subpartitions.begin(); it != subpartitions.end(); ++it, ++i) {
            out.PutIndent();
            out.Put("SUBPARTITION ");
            out.SafeWriteDBName((*it)->GetName());

            if (m_subpartitioningType != Partition::Type::Hash) {
                auto j = (*it)->GetName().size();
                do out.Put(" "); while (++j < nMaxNameLen + 1);

                out.Put(m_subpartitioningType == Partition::Type::Range ? "VALUES LESS THAN (" : "VALUES (");
                out.Put((*it)->GetHighValue());
                out.Put(")");
            }

            if (!(*it)->m_tablespaceName.IsNull()
                &&
                (
                    settings.IsStorageAlways()
                    || settings.AlwaysPutTablespace
                    //TODO: Isto esta horrivel, verificar forma mais elegante para resolver essa amarra.
                    //|| !isDefaultTablespace(m_Dictionary, (*it)->m_strTablespaceName.GetValue(), m_strOwner)
                    )
                )
            {
                out.Put(" TABLESPACE ");
                out.SafeWriteDBName((*it)->m_tablespaceName.GetValue());
            }

            if ((i < count - 1)) out.Put(",");
            out.NewLine();
        }
        out.MoveIndent(-2);
        out.PutIndent();
        out.Put(")");
        out.MoveIndent(-2);
    }

    void Table::WriteComments(MetaStream& out, const MetaSettings& settings) const {

        if (!m_comments.empty()) {
            out.PutIndent();
            out.Put("COMMENT ON TABLE ");
            out.PutOwnerAndName(m_owner, m_name, settings.SchemaName);
            out.Put(" IS ");
            out.WriteSQLString(m_comments);
            out.Put(settings.EndOfShortStatement);
            out.NewLine();
            out.NewLine();
        }

        for (auto it : m_columns) {
            //const TableColumn& column = *it->second.get();
            auto& column = *it.second;

            if (!column.GetComments().empty()) {
                out.PutIndent();
                out.Put("COMMENT ON COLUMN ");
                out.PutOwnerAndName(m_owner, m_name, settings.SchemaName);
                out.Put(".");
                out.SafeWriteDBName(column.GetColumnName());
                out.Put(" IS ");
                out.WriteSQLString(column.GetComments());
                out.Put(settings.EndOfShortStatement);
                out.NewLine();
            }
        }
        if (m_columns.size())
            out.NewLine();
    }

    int Table::Write(MetaStream& out, const MetaSettings& settings) const {

        if (settings.TableDefinition)
            WriteDefinition(out, settings);

        if (settings.Indexes)
            WriteIndexes(out, settings);

        if (settings.Constraints)
        {
            WriteConstraints(out, settings, 'C');

            if (m_type != Table::Type::Iot)
                WriteConstraints(out, settings, 'P');

            WriteConstraints(out, settings, 'U');
            WriteConstraints(out, settings, 'R');
        }

        if (settings.Triggers)
            WriteTriggers(out, settings);

        if (settings.Comments)
            WriteComments(out, settings);

        return 0;
    }

    const Constraint* Table::FindPrimaryKey() const {
        
        for (auto it : m_constraints) {
            Constraint* constraint = NULL;
            //TODO: Isto esta horrivel, verificar forma mais elegante para resolver essa amarra.
            //Constraint& constraint = m_Dictionary.LookupConstraint(m_strOwner.c_str(), it->c_str());
            if (constraint->GetType() == 'P') {
                return constraint;
            }
        }

        return 0;
    }

    void Table::WritePrimaryKeyForIOT(MetaStream& out, const MetaSettings& settings) const {
        const Constraint* pk = FindPrimaryKey();
        if (pk)
            pk->WritePrimaryKeyForIOT(out, settings);
    }
}
