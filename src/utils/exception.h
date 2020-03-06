#ifndef __PROJECT_STONE_UTILS_EXCEPTION_H__
#define __PROJECT_STONE_UTILS_EXCEPTION_H__

#include <oci.h>

#include <string>

namespace Utils {

    class Exception : public std::exception {
    private:
        int m_errcode;
        std::string m_message;

    public:
        Exception();
        Exception(int code, const char* msg)  : m_errcode(code), m_message(msg) {}
        Exception(int code, const std::string& msg) : m_errcode(code), m_message(msg) {}
        Exception(const Exception& other)     : std::exception(other), m_errcode(other.m_errcode), m_message(other.what()) {}

        operator const char* () const { return what(); }
        operator int() const { return m_errcode; }

        //const char* what() const noexcept { return m_message.c_str(); }
    };


    class UserCancel : public Exception {
    public:
        UserCancel(int code, const std::string& msg) : Exception(code, msg) {}
    };

    class NotFound : public Exception {
    public:
        NotFound(const std::string& objName);
    };

    class AlreadyExists : public Exception {
    public:
        AlreadyExists(const std::string& objName);
    };

}

#endif // __PROJECT_STONE_UTILS_EXCEPTION_H__
