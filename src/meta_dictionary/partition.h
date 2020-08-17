#ifndef __PROJECT_STONE_META_DICTIONARY_PARTITION_H__
#define __PROJECT_STONE_META_DICTIONARY_PARTITION_H__

#include <vector>

#include "table_storage.h"

namespace MetaDictionary {

    class Partition : public TableStorage
    {
    public:
        enum class Type { 
            None, 
            Range, 
            List, 
            Hash 
        };

        Partition() {}

        inline const std::vector<Partition*>& GetSubpartitions() const { return m_subpartitions; };
        inline const std::string& GetName() const { return m_name; };
        inline const std::string& GetHighValue() const { return m_highValue; };

    protected:
        std::vector<Partition*> m_subpartitions;
        std::string m_name;
        std::string m_highValue;

    private:
        // copy-constraction & assign-operation is not supported
        Partition(const Partition&);
        void operator = (const Partition&);
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_PARTITION_H__*/