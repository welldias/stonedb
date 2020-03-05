#ifndef __PROJECT_STONE_META_DICTIONARY_STRINGSTREAM_H__
#define __PROJECT_STONE_META_DICTIONARY_STRINGSTREAM_H__

#include <sstream>

#include "meta_stream.h"

namespace Data {

	class StringStream final : public MetaStream {
    private:
        std::stringstream m_stream;

	public:
        StringStream(bool lowerDBName = false)
            : MetaStream(lowerDBName) {}

		virtual ~StringStream() {}

        virtual int  GetPosition();
        virtual void PutIndent();
        virtual void NewLine();
        virtual void Put(const std::string& buffer);
	};

}

#endif/*__PROJECT_STONE_META_DICTIONARY_STRINGSTREAM_H__*/