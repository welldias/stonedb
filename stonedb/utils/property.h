#ifndef __PROJECT_STONE_UTILS_PROPERTY_H__
#define __PROJECT_STONE_UTILS_PROPERTY_H__

#include <oci.h>

#include <string>

namespace Utils {

    class Property {
    private:
        std::string m_name;
        std::string m_value;
    public:
        //Property();
        Property(const std::string& name);
        Property(const std::string& name, const std::string& value);
        ~Property();

        const std::string& GetName() const;
        const std::string& GetValue() const;
        
        void SetValue(const std::string& value);

        bool   ToBool() const;
        int    ToInt() const;
        double ToDouble() const;
        std::string ToString() const;
    };
}

#endif // __PROJECT_STONE_UTILS_PROPERTY_H__
