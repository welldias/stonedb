#include <map>

#include "substitutor.h"

namespace Utils {

    void Substitutor::ResetContent() {
        m_result.erase();
        m_whatFind.clear();
        m_withReplace.clear();
    }

    void Substitutor::AddPair(const char* find, const char* replace) {
        m_whatFind.push_back(find);
        m_withReplace.push_back(replace);
    }

    void Substitutor::AddPair(const std::string& find, const std::string& replace) {
        m_whatFind.push_back(find);
        m_withReplace.push_back(replace);
    }

    Substitutor& Substitutor::operator << (const char* input) {
        std::map<unsigned char, bool> fast_map;

        std::vector<std::string>::const_iterator
            it(m_whatFind.begin()), end(m_whatFind.end());

        if (m_casesensitive) {
            for (; it != end; ++it)
                fast_map[(*it)[0]] = true;
        }
        else {
            for (; it != end; ++it)
                fast_map[static_cast<unsigned char>(toupper((*it)[0]))] = true;
        }

        const char* chunk_begin = input;
        const char* chunk_end = input;

        while (*chunk_end) {
            if (m_casesensitive && fast_map[*chunk_end] || (!m_casesensitive && fast_map[static_cast<unsigned char>(toupper(*chunk_end))])) {
                bool hit = false;
                it = m_whatFind.begin();

                int i = 0;
                for (; it != end; ++it, i++) {
                    const std::string& str = (*it);

                    if ((m_casesensitive && !strncmp(chunk_end, str.c_str(), str.size())) 
                        || (!m_casesensitive && !_strnicmp(chunk_end, str.c_str(), str.size()))) {
                        hit = true;
                        break;
                    }
                }

                if (hit) {
                    m_result.append(chunk_begin, chunk_end - chunk_begin);
                    m_result.append(m_withReplace[i].c_str(), m_withReplace[i].size());
                    chunk_end += m_whatFind[i].size();
                    chunk_begin = chunk_end;
                    continue;
                }
            }
            chunk_end++;
        }

        m_result.append(chunk_begin);

        return *this;
    }
}