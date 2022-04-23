#include "exceptions.h"

ArgError::ArgError(const char* msg) noexcept :
    msg(msg)
{}

const char* ArgError::what() const throw()
{
    return msg;
}

AsmError::AsmError(const char* msg) noexcept :
    msg(msg)
{}

const char* AsmError::what() const throw()
{
    return msg;
}

TestError::TestError(const char* msg) noexcept :
    msg(msg)
{}

const char* TestError::what() const throw()
{
    return msg;
}