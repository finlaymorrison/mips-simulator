#include "exceptions.h"

ArgError::ArgError(const char* msg) noexcept :
    msg(msg)
{}

const char* ArgError::what() const throw()
{
    return msg;
}