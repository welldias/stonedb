#include <cstdio>

#include "table_storage.h"
#include "string_stream.h"

namespace MetaDictionary {

    TableStorage::TableStorage() {
    }

    void TableStorage::WriteParallel(MetaStream& out, const TableStorage& defStorage, const MetaSettings& settings) const {

        bool degree = IS_PRINTABLE(m_degree, defStorage, settings);
        bool instances = IS_PRINTABLE(m_instances, defStorage, settings);
        if (degree || instances) {

            PintInt pintInt;

            out.PutIndent();
            out.Put("PARALLEL (");
            if (degree)
                pintInt(out, "DEGREE %d", m_degree.GetValue());

            if (degree && instances)
                out.Put(" ");

            if (instances)
                pintInt(out, "INSTANCES %d", m_instances.GetValue());

            out.Put(")");
            out.NewLine();
        }
    }

    void TableStorage::WriteStorage(MetaStream& out, const TableStorage& defStorage, const MetaSettings& settings) const {

        if (settings.IsStorageEnabled()) {

            PintLineInt pintLineInt;

            char buff[128]; buff[sizeof(buff) - 1] = 0;

            if (IS_PRINTABLE(m_tablespaceName, defStorage, settings))
                WriteTablespace(out);

            if (IS_PRINTABLE(m_pctFree, defStorage, settings))
                pintLineInt(out, "PCTFREE  %3d", m_pctFree.GetValue());
            if (IS_PRINTABLE(m_pctUsed, defStorage, settings))
                pintLineInt(out, "PCTUSED  %3d", m_pctUsed.GetValue());
            if (IS_PRINTABLE(m_iniTrans, defStorage, settings))
                pintLineInt(out, "INITRANS %3d", m_iniTrans.GetValue());
            if (IS_PRINTABLE(m_maxTrans, defStorage, settings))
                pintLineInt(out, "MAXTRANS %3d", m_maxTrans.GetValue());

            {
                StringStream out2(true);
                out2.SetLike(out);
                out2.MoveIndent(2);

                Storage::WriteStorage(out2, defStorage, settings);

                if (IS_PRINTABLE(m_freeLists, defStorage, settings))
                    pintLineInt(out2, "FREELISTS   %3d", m_freeLists.GetValue());

                if (IS_PRINTABLE(m_freeListGroups, defStorage, settings))
                    pintLineInt(out2, "FREELIST GROUPS %d", m_freeListGroups.GetValue());

                if (IS_PRINTABLE(m_bufferPool, defStorage, settings)) {
                    out2.PutIndent();
                    out2.Put("BUFFER_POOL   ");
                    out2.Put(m_bufferPool.GetValue());
                    out2.NewLine();
                }

                out2.MoveIndent(-2);

                if (out2.GetLength()) {
                    out.PutLine("STORAGE (");
                    out.Put(out2.GetString());
                    out.PutLine(")");
                }
            }

            if (IS_PRINTABLE(m_logging, defStorage, settings))
                out.PutLine(m_logging.GetValue() ? "LOGGING" : "NOLOGGING");

            if (IS_PRINTABLE(m_compression, defStorage, settings) || IS_PRINTABLE(m_compressFor, defStorage, settings)) {
                out.PutIndent();
                out.Put(m_compression.GetValue() ? "COMPRESS" : "NOCOMPRESS");
                if (!m_compressFor.IsNull()) {
                    out.Put(" ");
                    out.Put(m_compressFor.GetValue());
                }
                out.NewLine();
            }

            WriteParallel(out, defStorage, settings);
        }
        else if (settings.AlwaysPutTablespace) {
            WriteTablespace(out);
        }
    }


    void TableStorage::CopyNotNulls(const TableStorage& other) {
        
        Storage::CopyNotNulls(other);

             m_freeLists.SetIfNotNull(other.m_freeLists);
        m_freeListGroups.SetIfNotNull(other.m_freeListGroups);
               m_pctFree.SetIfNotNull(other.m_pctFree);
               m_pctUsed.SetIfNotNull(other.m_pctUsed);
              m_iniTrans.SetIfNotNull(other.m_iniTrans);
              m_maxTrans.SetIfNotNull(other.m_maxTrans);
                m_degree.SetIfNotNull(other.m_degree);
             m_instances.SetIfNotNull(other.m_instances);
            m_bufferPool.SetIfNotNull(other.m_bufferPool);
    }
}