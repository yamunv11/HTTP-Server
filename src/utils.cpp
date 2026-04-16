#include <stdio.h>
#include <stdlib.h>

void error(const char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}
