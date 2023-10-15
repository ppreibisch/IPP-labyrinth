#include <stdlib.h>
#include <stdint.h>
#include "error.h"
#include "input.h"

void checkErrors(Array labirynth, Array x, Array y)
{
    uint64_t n = labirynth.size;
    int8_t errorNumber = -1;
    uint64_t size = 1;

    if(n == 0)
        errorNumber = 1;

    if (errorNumber == -1) {
        for (uint64_t i = 0; i < n; ++i) {
            if (labirynth.arr[i] < 1)
                errorNumber = 1;
            if (size > SIZE_MAX / labirynth.arr[i])
                errorNumber = 0;
            if (errorNumber != -1)
                break;
            size *= labirynth.arr[i];
        }
    }

    if (x.size != n && errorNumber == -1)
            errorNumber = 2;
    if (y.size != n && errorNumber == -1)
            errorNumber = 3;

    if (errorNumber == -1) {
        for (uint64_t i = 0; i < n; ++i) {
            if (x.arr[i] < 1 || x.arr[i] > labirynth.arr[i]) {
                errorNumber = 2;
                break;
            }
            if (y.arr[i] < 1 || y.arr[i] > labirynth.arr[i]) {
                errorNumber = 3;
                break;
            }
        }
    }

    if (errorNumber != -1) {
        free(labirynth.arr);
        free(x.arr);
        free(y.arr);
        returnError(errorNumber);
    }
}
