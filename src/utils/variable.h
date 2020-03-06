#ifndef __PROJECT_STONE_VARIABLE_H__
#define __PROJECT_STONE_VARIABLE_H__

namespace Utils {

    template <class T>
    class Variable {
        T m_value;
        int m_status;
    public:

        Variable() : m_status(0) {}

        void Set(const T& val) { m_value = val; m_status = 0x01; }
        void SetDefault(const T& val) { m_value = val; m_status = 0x01 | 0x02; }
        void SetIfNotNull(const Variable<T>& other) {
            if (!other.IsNull()) {
                m_value = other.m_value;
                m_status = other.m_status;
            }
        }

        void SetNull() { m_status = 0; }
        void SetDefault() { m_status |= 0x02; }

        bool IsNull() const { return !m_status; }
        bool IsDefault() const { return 0x02 & m_status ? true : false; }

        bool IsPrintable(bool force, const Variable<T>* other) const {
            if (IsNull()) return false;
            if (force) return true;
            if (other && !other->IsNull() && m_value == other->m_value) return false;
            return true;
        }

        const T& GetValue() const { return m_value; }

        bool operator != (const Variable& other) const {
            return IsNull() && !other.IsNull()
                || !IsNull() && other.IsNull()
                || GetValue() != other.GetValue();
        }

        bool operator == (const Variable& other) const {
            return (IsNull() && other.IsNull())
                || (!IsNull() && !other.IsNull())
                || (GetValue() == other.GetValue());
        }
    };
}

#endif /*__PROJECT_STONE_VARIABLE_H__*/