#ifndef __PROJECT_STONE_META_DICTIONARY_INDEXSTORAGE_H__
#define __PROJECT_STONE_META_DICTIONARY_INDEXSTORAGE_H__

#include "storage.h"

namespace MetaDictionary {

    class IndexStorage : public Storage {
    public:
        IndexStorage() : m_nCompressionPrefixLength(-1) { }

    protected:
        int m_nCompressionPrefixLength;
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_INDEXSTORAGE_H__*/