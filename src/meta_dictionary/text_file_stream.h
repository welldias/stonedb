#ifndef __PROJECT_STONE_META_DICTIONARY_FILESFILETREAM_H__
#define __PROJECT_STONE_META_DICTIONARY_FILESFILETREAM_H__

#include <fstream>

#include "meta_stream.h"

namespace MetaDictionary {

	class TextFileStream final : public MetaStream {
    private:
        std::ofstream m_stream;

	public:
        TextFileStream(const std::string& fileName, bool lowerDBName = false);
        virtual ~TextFileStream();

        virtual size_t  GetPosition();
        virtual size_t GetLength();
        virtual void PutIndent();
        virtual void NewLine();
        virtual void Put(const std::string& buffer);
	};

}

#endif/*__PROJECT_STONE_META_DICTIONARY_FILESFILETREAM_H__*/