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
}