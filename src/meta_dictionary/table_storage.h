#ifndef __PROJECT_STONE_META_DICTIONARY_TABLESTORAGE_H__
#define __PROJECT_STONE_META_DICTIONARY_TABLESTORAGE_H__

#include <map>
#include <set>
#include <string>

#include "storage.h"
#include "meta_settings.h"

namespace MetaDictionary {

    class TableStorage : public Storage
    {
    public:
        TableStorage();

        void WriteParallel(MetaStream& out, const TableStorage& defStorage, const MetaSettings& settings) const;
        void WriteStorage(MetaStream& out, const TableStorage& defStorage, const MetaSettings& settings) const;
        void CopyNotNulls(const TableStorage&);

        //TODO: Isto esta horrivel, verificar forma mais elegante para resolver essa amarra.
        //static void BuildDefault(TableStorage&, const std::string& schema, const std::string& tablespace);

    public:
        Variable<int> m_freeLists;
        Variable<int> m_freeListGroups;
        Variable<int> m_pctFree;
        Variable<int> m_pctUsed;
        Variable<int> m_iniTrans;
        Variable<int> m_maxTrans;
        Variable<int> m_degree;
        Variable<int> m_instances;
        Variable<std::string> m_bufferPool;
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_TABLESTORAGE_H__*/