#include "property.h"

#include "string_util.h"

namespace Utils {

	Property::Property(const std::string& name)
		: m_name(name), m_type(Type::Unknown) {
	}

	Property::Property(const std::string& name, const std::string& value)
		: m_name(name), m_value(value), m_type(Type::String) {
	}

	Property::Property(const std::string& name, bool value)
		: m_name(name), m_type(Type::Boolean) {
		SetValue(value);
	}

	Property::Property(const std::string& name, int value)
		: m_name(name), m_type(Type::Number) {
		SetValue(value);
	}

	Property::Property(const std::string& name, double value)
		: m_name(name), m_type(Type::Number) {
		SetValue(value);
	}

	Property::~Property() {
	}

	bool Property::ToBool() const {
		return StringUtil::ToBool(m_value);
	}

	int Property::ToInt() const {
		return StringUtil::ToInt(m_value);
	}

	double Property::ToDouble() const {
		return StringUtil::ToDouble(m_value);
	}

	std::string Property::ToString() const {
		return m_value;
	}

	const std::string& Property::GetName() const {
		return m_name;
	}

	const std::string& Property::GetValue() const {
		return m_value;
	}

	void Property::SetValue(const std::string& value) {
		m_value = value;
	}

	void Property::SetValue(bool value) {
		std::string strValue = (value ? "true" : "false");
		SetValue(strValue);
	}

	void Property::SetValue(int value) {
		SetValue(std::to_string(value));
	}

	void Property::SetValue(double value) {
		SetValue(std::to_string(value));
	}
}