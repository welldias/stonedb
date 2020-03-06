#ifndef __PROJECT_STONE_META_DICTIONARY_STORAGE_H__
#define __PROJECT_STONE_META_DICTIONARY_STORAGE_H__

#include <map>
#include <set>
#include <string>

#include "meta_stream.h"
#include "meta_settings.h"

#include "..\utils\utils.h"

namespace MetaDictionary {

    using namespace Utils;

    /// Storage parameters for tablespaces, tables, indices & clusters //////
    class Storage
    {
    public:
        Storage();

        void WriteTablespace(MetaStream& out) const;
        void WriteStorage(MetaStream& out, const Storage& defStorage, const MetaSettings& settings) const;
        void CopyNotNulls(const Storage&);

    public:
        Variable<std::string> m_tablespaceName;
        Variable<int>         m_initialExtent;
        Variable<int>         m_nextExtent;
        Variable<int>         m_minExtents;
        Variable<int>         m_maxExtents;
        Variable<int>         m_pctIncrease;
        Variable<bool>        m_logging;
        Variable<bool>        m_compression;
        Variable<std::string> m_compressFor;
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_STORAGE_H__*/