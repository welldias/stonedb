#ifndef __PROJECT_STONE_UTILS_STRING_UTILS_H__
#define __PROJECT_STONE_UTILS_STRING_UTILS_H__

#include <string>

namespace Utils {
	class StringUtil {
	private:
		static const std::string  strKip;

	public:
		static bool    ToBool(const std::string& str);
		static int     ToInt(const std::string& str);
		static double  ToDouble(const std::string& str);

		static std::string ToLower(const std::string& str);
		static std::string ToUpper(const std::string& str);

		static std::string Rtrim(const std::string& str, const std::string& t = strKip);
		static std::string Ltrim(const std::string& str, const std::string& t = strKip);
		static std::string Trim(const std::string& str, const std::string& t = strKip);

		static int Compare(const std::string& str1, const std::string& str2, size_t size = -1);
		static int NoCaseCompare(const std::string& str1, const std::string& str2, size_t size = -1);
	};
}

#endif // __PROJECT_STONE_UTILS_STRING_UTILS_H__
