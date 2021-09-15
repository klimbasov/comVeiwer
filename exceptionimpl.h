#ifndef EXCEPTIONIMPL_H
#define EXCEPTIONIMPL_H

#include <exception>


class exceptionImpl : public std::exception
{
    const char* message;
    const bool critical;
public:
    exceptionImpl(const char*, const bool);

    const char *what() const noexcept;

     bool isCritical();
};

#endif // EXCEPTIONIMPL_H
