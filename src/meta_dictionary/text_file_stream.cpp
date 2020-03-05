#include "text_file_stream.h"

namespace Data {

    TextFileStream::TextFileStream(const std::string& fileName, bool lowerDBName)
        : MetaStream(lowerDBName) {
        
        m_stream.open(fileName);
    }

    TextFileStream::~TextFileStream() {
        m_stream.close();
    }

    int TextFileStream::GetPosition() {
        return static_cast<int>(m_stream.tellp());
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
