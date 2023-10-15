#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void returnError(int8_t num)
{
    fprintf(stderr, "ERROR %d\n", num);
    exit(1);
}
