#include "exceptionimpl.h"

exceptionImpl::exceptionImpl(const char * message, const bool critical):message(message), critical(critical)
{

}

const char *exceptionImpl::what() const noexcept
{
    return message;
}

bool exceptionImpl::isCritical()
{
    return critical;
}
