#ifndef __PROJECT_STONE_META_FUNCTION_PARSER_UTIL_H__
#define __PROJECT_STONE_META_FUNCTION_PARSER_UTIL_H__

#include <map>

#include "db_object.h"

namespace MetaDictionary {

    class ParserUtil {
    public:
        ParserUtil() {};

        static int  SkipSpace(const char* str);
        static int  SkipBackSpace(const char* str, int len);
        static int  SkipSpaceAndComments(const char* str);
        static void GetWordSP(const char* str, const char*& wordPtr, int& wordLen);
        static void GetWordID(const char* str, const char*& wordPtr, int& wordLen);
        static int  SkipDbName(const char* str);
        static int  CountLines(const char* str, int len);
        static void WriteTextBlock(MetaStream& out, const std::string& text, bool trunc, bool strip);
        static void WriteSubstitutedClause(MetaStream& out, const std::string& text, const std::string& suffix);
    };
}

#endif/*__PROJECT_STONE_META_FUNCTION_PARSER_UTIL_H__*/