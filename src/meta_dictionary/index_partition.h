#ifndef __PROJECT_STONE_META_DICTIONARY_INDEXPARTITION_H__
#define __PROJECT_STONE_META_DICTIONARY_INDEXPARTITION_H__

#include <string>
#include <vector>


#include "index_storage.h"

namespace MetaDictionary {


    class IndexPartition : public IndexStorage
    {
    protected:
        std::vector<IndexPartition*> m_subpartitions;
        std::string m_strName;
        std::string m_strHighValue;

    public:
        IndexPartition() {}

    private:
        // copy-constraction & assign-operation is not supported
        IndexPartition(const IndexPartition&);
        void operator = (const IndexPartition&);
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_INDEXPARTITION_H__*/