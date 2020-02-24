#ifndef __PROJECT_STONE_STONE_EXCEPTION_H__
#define __PROJECT_STONE_STONE_EXCEPTION_H__

#include <oci.h>

#include <string>

namespace Stone {

    class Exception : public std::exception {
    private:
        int m_errcode;

    public:
        Exception();
        Exception(int code, const char* msg)  : std::exception(msg), m_errcode(code) {}
        Exception(int code, const std::string& msg) : std::exception(msg.c_str()), m_errcode(code) {}
        Exception(const Exception& other)     : std::exception(other), m_errcode(other.m_errcode) {}

        operator const char* () const { return what(); }
        operator int() const { return m_errcode; }
    };


    class UserCancel : public Exception {
    public:
        UserCancel(int code, const std::string& msg) : Exception(code, msg) {}
    };
}

#endif // __PROJECT_STONE_STONE_EXCEPTION_H__
