#include "index.h"
#include "string_stream.h"

namespace MetaDictionary {

    Index::Index() {
        m_type = Type::Normal;
        m_uniqueness = false;
        m_temporary = false;
        m_generated = false;
        m_reverse = false;
        m_partitioningType = Partition::Type::None;
        m_subpartitioningType = Partition::Type::None;
        m_local = false;
        m_iOTOverflow_PCTThreshold = 0;
        m_indexCompression = false;
    };

    int Index::Write(MetaStream& out, const MetaSettings& settings) const {

        if (settings.GetGeneratePrompts()) {
            out.PutIndent();
            out.Put("PROMPT CREATE INDEX ");
            out.PutOwnerAndName(m_owner, m_name, settings.GetShemaName() || m_owner != m_tableOwner);
            out.NewLine();
        }

        out.PutIndent();
        out.Put("CREATE ");

        if (m_type == Type::Bitmap || m_type == Type::FunctionBasedBitmap)
            out.Put("BITMAP ");
        else
            if (m_type != Type::Cluster && m_uniqueness)
                out.Put("UNIQUE ");

        out.Put("INDEX ");
        out.PutOwnerAndName(m_owner, m_name, settings.GetShemaName() || m_owner != m_tableOwner);
        out.NewLine();

        out.MoveIndent(2);
        out.PutIndent();
        out.Put("ON ");

        if (m_type != Type::Cluster) {
            out.PutOwnerAndName(m_tableOwner, m_tableName, settings.GetShemaName());
            out.Put(" (");
            out.NewLine();

            std::map<int, bool> safeWriteDBName;
            std::map<int, std::string>::const_iterator it(m_columns.begin()), end(m_columns.end());
            for (int i(0); it != end; it++, i++) {
                safeWriteDBName[i] = (m_isExpression.find(i) == m_isExpression.end());
            }

            out.WriteColumns(m_columns, 2, safeWriteDBName);
            out.PutLine(")");
        }
        else {
            out.Put("CLUSTER ");
            out.PutOwnerAndName(m_tableOwner, m_tableName, settings.GetShemaName());
            out.NewLine();
        }

        if (m_type == Type::Domain)
            WriteDomainClause(out, settings);
        else
            WriteIndexParams(out, settings);

        out.MoveIndent(-2);
        out.PutLine("/");
        out.NewLine();

        return 0;
    }

    void Index::WriteIndexParams(MetaStream& out, const MetaSettings& settings, bool skip_parallel) const {

        if (m_reverse)
            out.PutLine("REVERSE");

        if (!m_temporary) {

            TableStorage defStorage;
            
            //TODO: Isto esta horrivel, verificar forma mais elegante para resolver essa amarra.
            //BuildDefault(defStorage, m_dictionary, m_owner, m_tablespaceName.GetValue());

            defStorage.m_iniTrans.SetDefault(2);

            // parallel cannot be part create constraint
            if (skip_parallel) {
                TableStorage storage(*((TableStorage*)this));
                storage.m_degree.SetNull();
                storage.m_instances.SetNull();
                storage.WriteStorage(out, defStorage, settings);
            }
            else
                WriteStorage(out, defStorage, settings);

            WriteCompress(out, settings);

            if (m_partitioningType != Partition::Type::None)
                WriteIndexPartitions(out, settings);
        }

        if (settings.GetStorageSubstitutedClause())
            DbObject::WriteSubstitutedClause(out, m_name, "_st");
    }

    void Index::WriteCompress(MetaStream& out, const MetaSettings& settings) const  {

        if (!m_indexCompression)
            return; 
        
        out.PutIndent();
        out.Put("COMPRESS");

        if (settings.IsStorageAlways() || (!(m_uniqueness && m_compressionPrefixLength == static_cast<int>(m_columns.size()) - 1) && !(!m_uniqueness && m_compressionPrefixLength == static_cast<int>(m_columns.size())))) {
            char buff[128]; 

            memset(buff, 0, sizeof(buff));
            std::snprintf(buff, sizeof(buff) - 1, " %d", m_compressionPrefixLength);
            out.Put(buff);
        }

        out.NewLine();
    }

    void Index::WriteDomainClause(MetaStream& out, const MetaSettings& settings) const {

        out.PutIndent();
        out.Put("INDEXTYPE IS ");
        out.PutOwnerAndName(m_domainOwner, m_domain, settings.GetShemaName() || m_owner != m_domainOwner);
        out.NewLine();

        if (!m_domainParams.empty()) {
            out.MoveIndent(2);
            out.PutIndent();
            out.Put("PARAMETERS('");
            out.Put(m_domainParams);
            out.Put("')");
            out.NewLine();
            out.MoveIndent(-2);
        }
    }

    void Index::WriteIndexPartitions(MetaStream& out, const MetaSettings& settings) const {

        if (m_partitioningType != Partition::Type::None) {

            if (m_local && m_type != Type::IoitTop) {
                out.PutLine("LOCAL (");
            } else {

                out.PutIndent();

                if (m_type != Type::IoitTop)
                    out.Put("GLOBAL ");

                switch (m_partitioningType)
                {
                case Partition::Type::Range:
                    out.Put("PARTITION BY RANGE (");
                    out.NewLine();
                    out.WriteColumns(m_partKeyColumns, 2, true);
                    out.PutLine(")");
                    break;
                case Partition::Type::Hash:
                    out.Put("PARTITION BY HASH (");
                    out.NewLine();
                    out.WriteColumns(m_partKeyColumns, 2, true);
                    out.PutLine(")");
                    break;
                case Partition::Type::List:
                    out.Put("PARTITION BY LIST (");
                    out.SafeWriteDBName(!m_partKeyColumns.empty() ? *m_partKeyColumns.begin() : std::string());
                    out.Put(")");
                    out.NewLine();
                    break;
                }

                out.PutLine("(");
            }
            out.MoveIndent(2);

            int maxNameLen = 0;
            int nameLen = 0;
            for (auto it = m_partitions.begin(); it != m_partitions.end(); ++it) {
                nameLen = static_cast<int>((*it)->Name().length());
                maxNameLen = maxNameLen > nameLen ? maxNameLen : nameLen;
            }

            int i = 0, count = m_partitions.size();
            for (auto it = m_partitions.begin(); it != m_partitions.end(); ++it, ++i) {
                out.PutIndent();
                out.Put("PARTITION ");
                out.SafeWriteDBName(((DbObject*)(*it))->m_name);

                if ((!m_local || m_type == Type::IoitTop) && m_partitioningType != Partition::Type::Hash) {
                    int j = (*it)->Name().size();
                    do out.Put(" "); while (++j < maxNameLen + 1);

                    out.Put(m_partitioningType == Partition::Type::Range ? "VALUES LESS THAN (" : "VALUES (");
                    out.Put((*it)->HighValue());
                    out.Put(")");
                }

                {
                    StringStream out2(true);
                    out2.SetLike(out);

                    TableStorage defStorage;
                    //TODO: Isto esta horrivel, verificar forma mais elegante para resolver essa amarra.
                    //BuildDefault(defStorage, m_dictionary, m_owner, (*it)->m_tablespaceName.GetValue());

                    defStorage.CopyNotNulls(*((TableStorage*)this)); // copy defaults defined on index level

                    out2.MoveIndent(2);
                    (*it)->WriteStorage(out2, defStorage, settings);
                    out2.MoveIndent(-2);

                    if (m_type == Type::IoitTop && !m_iOTOverflowPartitions.empty()) {
                        StringStream out3(true);
                        out3.SetLike(out);

                        TableStorage defStorage;
                        //TODO: Isto esta horrivel, verificar forma mais elegante para resolver essa amarra.
                        //BuildDefault(defStorage, m_dictionary, m_owner, m_iOTOverflowPartitions.at(i)->m_tablespaceName.GetValue());
                        defStorage.CopyNotNulls(m_iOTOverflowPartitionDefaultStrorage);

                        out3.MoveIndent(2);
                        m_iOTOverflowPartitions.at(i)->WriteStorage(out3, defStorage, settings);
                        out3.MoveIndent(-2);

                        if (out3.GetLength())
                        {
                            out2.PutLine("OVERFLOW");
                            out2.Put(out3.GetString());
                        }
                    }

                    if (!(*it)->SubPartitions().empty())
                        WriteSubpartitions(out2, settings, (*it)->SubPartitions());

                    out2.TrimRight(" \n\r");

                    if (out2.GetLength()) {
                        out.NewLine();
                        out.Put(out2.GetString());
                    }

                    if ((i < count - 1)) out.Put(",");
                    out.NewLine();
                }
            }

            out.MoveIndent(-2);
            out.PutLine(")");
        }
    }

    void Index::WriteSubpartitions(MetaStream& out, const MetaSettings& settings, const std::vector<IndexPartition*>& subpartitions) const {

        //TODO: how to handle defaul subpartitioning?
        out.MoveIndent(2);
        out.PutLine("(");
        out.MoveIndent(2);

        int maxNameLen = 0;
        int nameLen = 0;
        for (auto it = subpartitions.begin(); it != subpartitions.end(); ++it) {
            nameLen = static_cast<int>((*it)->Name().length());
            maxNameLen = maxNameLen > nameLen ? maxNameLen : nameLen;
        }

        int i = 0, count = subpartitions.size();
        for (auto it = subpartitions.begin(); it != subpartitions.end(); ++it, ++i) {

            out.PutIndent();
            out.Put("SUBPARTITION ");
            out.SafeWriteDBName((*it)->Name());

            if (!m_local && m_subpartitioningType != Partition::Type::Hash)
            {
                int j = (*it)->Name.size();
                do out.Put(" "); while (++j < maxNameLen + 1);

                out.Put(m_subpartitioningType == Partition::Type::Range ? "VALUES LESS THAN (" : "VALUES (");
                out.Put((*it)->HighValue());
                out.Put(")");
            }

            //bool isDefaultTablespace(const Dictionary & dictionary, const string & tablespace, const string & owner)

            bool isDefaultTablespace = false;
            try
            {
                if (tablespace == m_dictionary.LookupUser(owner.c_str()).m_strDefTablespace)
                    return true;
            }
            catch (const XNotFound&) {}


            if (!(*it)->m_tablespaceName.IsNull() && (settings.IsStorageAlways() || settings.GetAlwaysPutTablespace() || !isDefaultTablespace(m_Dictionary, (*it)->m_tablespaceName.GetValue(), m_owner)))
            {
                out.Put(" TABLESPACE ");
                out.SafeWriteDBName((*it)->m_strTablespaceName.GetValue());
            }

            if ((i < count - 1)) out.Put(",");
            out.NewLine();
        }
        out.MoveIndent(-2);
        out.PutIndent();
        out.Put(")");
        out.MoveIndent(-2);
    }
}
