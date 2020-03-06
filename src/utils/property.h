#ifndef __PROJECT_STONE_UTILS_PROPERTY_H__
#define __PROJECT_STONE_UTILS_PROPERTY_H__

#include <string>

namespace Utils {

    class Property {
    public:
        enum class Type {
            Unknown,
            String,
            Number,
            Double,
            Boolean,
        };

        Property(const std::string& name);
        Property(const std::string& name, const std::string& value);
        Property(const std::string& name, bool value);
        Property(const std::string& name, int value);
        Property(const std::string& name, double value);
        ~Property();

        const std::string& GetName()  const;
        const std::string& GetValue() const;
        const Type         GetType()  const;
        
        void SetValue(const std::string& value);
        void SetValue(bool   value);
        void SetValue(int    value);
        void SetValue(double value);

        bool   ToBool() const;
        int    ToInt() const;
        double ToDouble() const;
        std::string ToString() const;

    private:
        std::string m_name;
        std::string m_value;
        Type        m_type;
    };
}

#endif // __PROJECT_STONE_UTILS_PROPERTY_H__
