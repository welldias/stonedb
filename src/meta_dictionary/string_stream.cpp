#include "string_stream.h"

namespace MetaDictionary {

	size_t StringStream::GetPosition() {
        return static_cast<int>(m_stream.tellp());
    }

    size_t StringStream::GetLength() {

        auto oldPos = m_stream.tellp();
        m_stream.seekg(0, std::ios::end);
        auto size = m_stream.tellg();
        m_stream.seekg(oldPos);

        return static_cast<int>(size);
    }

    void StringStream::PutIndent() {
        m_stream << m_indent;
    }

    void StringStream::NewLine() {
        m_stream << "\n";
    }

    void StringStream::Put(const std::string& buffer) {
        m_stream << buffer;
    }

    void StringStream::TrimRight(const std::string& charset)
    {
    }
}
