#include <string> 
#include <algorithm>
#include <sstream>

#include "parser_util.h"

namespace MetaDictionary {

    int ParserUtil::SkipSpace(const char* str)
    {
        int i = 0;
        for (; str && str[i] && isspace(str[i]); i++);
        return i;
    }

    int ParserUtil::SkipBackSpace(const char* str, int len)
    {
        if (len) {
            int i = len - 1;
            for (; str && i >= 0; i--)
                if (!isspace(str[i])) {
                    i++;
                    break;
                }
            return (i < 0) ? 0 : i;
        }
        return 0;
    }

    int ParserUtil::SkipSpaceAndComments(const char* str) {

        int i = 0;
        for (; str && str[i]; i++) {
            if (isspace(str[i])) {
                continue;
            }
            else if (str[i] == '-' && str[i + 1] == '-') {
                for (i += 2; str[i] && str[i] != '\n'; i++);
                continue;
            }
            else if (str[i] == '/' && str[i + 1] == '*') {
                for (i += 2; str[i] && !(str[i] == '*' && str[i + 1] == '/'); i++);
                i++;
                continue;
            }
            else
                break;
        }
        return i;
    }

    void ParserUtil::GetWordSP(const char* str, const char*& wordPtr, int& wordLen) {

        wordPtr = 0;
        wordLen = 0;

        int i = 0;
        for (; str && str[i] && (isspace(str[i]) || ispunct(str[i])); i++);
        wordPtr = str + i;
        for (; str && str[i] && !isspace(str[i]) && !ispunct(str[i]); i++, wordLen++);
    }

    void ParserUtil::GetWordID(const char* str, const char*& wordPtr, int& wordLen) {

        wordPtr = 0;
        wordLen = 0;

        int i = SkipSpaceAndComments(str);
        wordPtr = str + i;
        if (str && str[i] != '\"') {
            for (; str && str[i] && (isalnum(str[i]) || strchr("_$#", str[i])); i++, wordLen++);
        }
        else {
            for (i++, wordLen++; str && str[i] && str[i] != '\"'; i++, wordLen++);
            if (str && str[i] == '\"')
                wordLen++;
        }
    }

    int ParserUtil::SkipDbName(const char* str) {
        
        int len;
        const char* ptr;
        GetWordID(str, ptr, len);
        ptr += len;
        int offset = SkipSpaceAndComments(ptr);

        if (ptr[offset] == '.') {
            ptr += offset;
            ptr++;
            GetWordID(ptr, ptr, len);
            ptr += len;
        }

        return static_cast<int>(ptr - str);
    }

    int ParserUtil::CountLines(const char* str, int len) {

        int count = 0;
        for (int i(0); str[i] && i < len; i++)
            if (str[i] == '\n')
                count++;

        return count;
    }

    void ParserUtil::WriteTextBlock(MetaStream& out, const std::string& text, bool trunc, bool strip)
    {
        if (!trunc && strip) {
            throw Exception("Ilogic params trunc/strip");
        }

        auto len = SkipBackSpace(text.c_str(), static_cast<int>(text.size()));

        if (!trunc)
            out.PutLine(text.substr(len));
        else {
            std::string buffer;
            std::stringstream stream(text);
            while (std::getline(stream, buffer, '\n')) {
                len = SkipBackSpace(buffer.c_str(), static_cast<int>(buffer.size()));
                if (!strip || len)
                    out.PutLine(text.substr(len));
            }
        }
    }

    void ParserUtil::WriteSubstitutedClause(MetaStream& out, const std::string& text, const std::string& suffix) {

        auto clause = StringUtil::ToLower(text);
        if ((clause.size() + suffix.size()) > 30)
            clause.resize(30 - suffix.size());

        for (auto it  : clause)
            if (!isalnum(it)) it = '_';

        out.PutIndent();
        out.Put("&");
        out.Put(clause);
        out.Put(suffix);
        out.NewLine();
    }
}
