#include "index.h"

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

    int Index::Write(MetaStream& out, const MetaSettings& settings) const
    {
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
            writeDomainClause(out, settings);
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
                TableStorage storage(*this);
                storage.m_degree.SetNull();
                storage.m_instances.SetNull();
                storage.WriteStorage(out, defStorage, settings);
            }
            else
                WriteStorage(out, defStorage, settings);

            WriteCompress(out, settings);

            if (m_partitioningType != Partition::Type::None)
                writeIndexPartitions(out, settings);
        }

        if (settings.GetStorageSubstitutedClause())
            write_substituted_clause(out, m_name, "_st");
    }
}
