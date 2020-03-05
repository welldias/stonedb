#include "string_stream.h"

namespace Data {

	int StringStream::GetPosition() {
        return static_cast<int>(m_stream.tellp());
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
}
