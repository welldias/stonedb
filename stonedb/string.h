#ifndef __PROJECT_STONE_UTILS_STRING_H__
#define __PROJECT_STONE_UTILS_STRING_H__

#include <oci.h>

#include <string>

namespace Utils {

    class String  {
    public:
        static bool    ToBool(const std::string& str);
        static int     ToInt(const std::string& str);
        static double  ToDouble(const std::string& str);

        static std::string ToLower(const std::string& str);
        static std::string ToUpper(const std::string& str);
        static std::string Rtrim(const std::string& str, const std::string& t = " \t");
        static std::string Ltrim(const std::string& str, const std::string& t = " \t");
        static std::string Trim(const std::string& str, const std::string& t = " \t");
    };
}

#endif // __PROJECT_STONE_UTILS_STRING_H__
