#ifndef input
#define input

#include <stdbool.h>

struct dynamicArray {
    size_t size;
    size_t *arr;
};
typedef struct dynamicArray Array;

Array inputData(uint8_t lineNumber, int8_t *errorPtr);
unsigned char *inputWalls(size_t labirynthSize, int8_t *errorPtr);

#endif
