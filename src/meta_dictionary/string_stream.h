#ifndef __PROJECT_STONE_META_DICTIONARY_STRINGSTREAM_H__
#define __PROJECT_STONE_META_DICTIONARY_STRINGSTREAM_H__

#include <sstream>

#include "meta_stream.h"

namespace MetaDictionary {

	class StringStream : public MetaStream {
    private:
        std::stringstream m_stream;

	public:
        StringStream(bool lowerDBName = false)
            : MetaStream(lowerDBName) {}

		virtual ~StringStream() {}

        virtual size_t  GetPosition();
        virtual size_t GetLength();
        virtual void PutIndent();
        virtual void NewLine();
        virtual void Put(const std::string& buffer);


        std::string GetString() const { return m_stream.str();  }
	};

}

#endif/*__PROJECT_STONE_META_DICTIONARY_STRINGSTREAM_H__*/