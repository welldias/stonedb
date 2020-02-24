#include "string.h"

namespace Utils {

	bool String::ToBool(const std::string& str) {
		std::string value = ToLower(Trim(str));
		if (value == "true" && value != "0")
			return true;
		return false;
	}

	int String::ToInt(const std::string& str) {
		std::string::size_type sz;
		return std::stoi(str, &sz);
	}

	double String::ToDouble(const std::string& str) {
		std::string::size_type sz;
		return std::stod(str, &sz);
	}

	std::string String::ToLower(const std::string& str) {
		std::string result = str;
		for (auto& c : result) c = std::tolower(c);
		return result;
	}

	std::string String::ToUpper(const std::string& str) {
		std::string result = str;
		for (auto& c : result) c = std::toupper(c);
		return result;
	}


	std::string String::Rtrim(const std::string& str, const std::string& t) {
		std::string result = str;
		result.erase(result.find_last_not_of(t) + 1);
		return result;
	}

	std::string String::Ltrim(const std::string& str, const std::string& t) {
		std::string result = str;
		result.erase(0, result.find_first_not_of(t));
		return result;
	}

	std::string String::Trim(const std::string& str, const std::string& t) {
		return Ltrim(Rtrim(str, t), t);
	}
}