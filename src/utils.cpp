#include <cstring>
#include <stdexcept>
#include <stdlib.h>

void cerror(std::string message)
{
    throw std::runtime_error(message + ": " + strerror(errno));
}
