#include <cstdio>

#include "storage.h"

namespace MetaDictionary {
    const int unlimitedVal = 2147483645;

#define IS_PRINTABLE(par, storage, settings) ((par).IsPrintable((settings).IsStorageAlways(), &((storage).par) ) )

    struct PintLineInt {
    public:
        void operator()(MetaStream& out, const std::string& format, int value) {
            memset(buff, 0, sizeof(buff));
            std::snprintf(buff, sizeof(buff) - 1, format.c_str(), value);
            out.PutLine(buff);
        }
    private:
        char buff[128];
    };

    struct PintInt {
    public:
        void operator()(MetaStream& out, const std::string& format, int value) {
            memset(buff, 0, sizeof(buff));
            std::snprintf(buff, sizeof(buff) - 1, format.c_str(), value);
            out.Put(buff);
        }
    private:
        char buff[128];
    };

    Storage::Storage() {
    }

    void Storage::WriteTablespace(MetaStream& out) const {
        if (!m_tablespaceName.IsNull()) {
            out.PutIndent();
            out.Put("TABLESPACE ");
            out.SafeWriteDBName(m_tablespaceName.GetValue());
            out.NewLine();
        }
    }

    void Storage::WriteStorage(MetaStream& out, const Storage& defStorage, const MetaSettings& settings) const {
        if (settings.IsStorageEnabled()) {
            PintLineInt printLineint;
            if (IS_PRINTABLE(m_initialExtent, defStorage, settings))
                printLineint(out, "INITIAL %7d K", m_initialExtent.GetValue() / 1024);
            if (IS_PRINTABLE(m_nextExtent, defStorage, settings))
                printLineint(out, "NEXT    %7d K", m_nextExtent.GetValue() / 1024);
            if (IS_PRINTABLE(m_minExtents, defStorage, settings))
                printLineint(out, "MINEXTENTS  %3d", m_minExtents.GetValue());
            if (IS_PRINTABLE(m_maxExtents, defStorage, settings)) {
                if (m_maxExtents.GetValue() < unlimitedVal)
                    printLineint(out, "MAXEXTENTS  %3d", m_maxExtents.GetValue());
                else
                    out.PutLine("MAXEXTENTS    UNLIMITED");
            }
            if (IS_PRINTABLE(m_pctIncrease, defStorage, settings)) 
                printLineint(out, "PCTINCREASE %3d", m_pctIncrease.GetValue());
        }
    }

    void Storage::CopyNotNulls(const Storage& other) {

        //;-)
        m_tablespaceName.SetIfNotNull(other.m_tablespaceName);
         m_initialExtent.SetIfNotNull(other.m_initialExtent);
            m_nextExtent.SetIfNotNull(other.m_nextExtent);
            m_minExtents.SetIfNotNull(other.m_minExtents);
            m_maxExtents.SetIfNotNull(other.m_maxExtents);
           m_pctIncrease.SetIfNotNull(other.m_pctIncrease);
               m_logging.SetIfNotNull(other.m_logging);
           m_compression.SetIfNotNull(other.m_compression);
           m_compressFor.SetIfNotNull(other.m_compressFor);
    }
}