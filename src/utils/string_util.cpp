#include <string.h> 

#include "string_util.h"


namespace Utils {
	const std::string  StringUtil::strKip = " \t\n\r";

	bool StringUtil::ToBool(const std::string& str) {
		std::string value = ToLower(Trim(str));
		if (value == "true" && value != "0")
			return true;
		return false;
	}

	int StringUtil::ToInt(const std::string& str) {
		std::string::size_type sz;
		return std::stoi(str, &sz);
	}

	double StringUtil::ToDouble(const std::string& str) {
		std::string::size_type sz;
		return std::stod(str, &sz);
	}

	std::string StringUtil::ToLower(const std::string& str) {
		std::string result = str;
		for (auto& c : result) c = std::tolower(c);
		return result;
	}

	std::string StringUtil::ToUpper(const std::string& str) {
		std::string result = str;
		for (auto& c : result) c = std::toupper(c);
		return result;
	}

	std::string StringUtil::Rtrim(const std::string& str, const std::string& t) {
		std::string result = str;
		result.erase(result.find_last_not_of(t) + 1);
		return result;
	}

	std::string StringUtil::Ltrim(const std::string& str, const std::string& t) {
		std::string result = str;
		result.erase(0, result.find_first_not_of(t));
		return result;
	}

	std::string StringUtil::Trim(const std::string& str, const std::string& t) {
		return Ltrim(Rtrim(str, t), t);
	}

	int StringUtil::Compare(const std::string& str1, const std::string& str2, size_t size) {

		if (size >= 0)
			return strncmp(str1.c_str(), str2.c_str(), size);
		else
			return strcmp(str1.c_str(), str2.c_str());
	}

	int StringUtil::NoCaseCompare(const std::string& str1, const std::string& str2, size_t size) {
#ifdef  _WINDOWS
#define strncasecmp(str1, str2, size) _strnicmp(str1, str2, size)
#define strcasecmp(str1, str2) _stricmp(str1, str2)
#endif
		if(size >= 0)
			return strncasecmp(str1.c_str(), str2.c_str(), size);
		else
			return strcasecmp(str1.c_str(), str2.c_str());

		return 0;
	}

}