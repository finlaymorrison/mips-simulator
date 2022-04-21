#pragma once

#include <exception>

class ArgError : public std::exception
{
private:
    const char* msg;
public:
    ArgError(const char *msg) noexcept;
    const char* what() const throw() override;
};

class AsmError : public std::exception
{
private:
    const char* msg;
public:
    AsmError(const char *msg) noexcept;
    const char* what() const throw() override;
};