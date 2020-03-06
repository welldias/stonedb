#include "text_file_stream.h"

namespace MetaDictionary {

    TextFileStream::TextFileStream(const std::string& fileName, bool lowerDBName)
        : MetaStream(lowerDBName) {
        
        m_stream.open(fileName);
    }

    TextFileStream::~TextFileStream() {
        m_stream.close();
    }


    size_t TextFileStream::GetPosition() {
        return static_cast<int>(m_stream.tellp());
    }

    size_t TextFileStream::GetLength() {

        auto oldPos = m_stream.tellp();
        m_stream.seekp(0, std::ios::end);
        auto size = m_stream.tellp();
        m_stream.seekp(oldPos);

        return static_cast<int>(size);
    }

    void TextFileStream::PutIndent() {
        m_stream << m_indent;
    }

    void TextFileStream::NewLine() {
        m_stream << "\n";
    }

    void TextFileStream::Put(const std::string& buffer) {
        m_stream << buffer;
    }
}
