#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include "input.h"
#include "error.h"

static bool isHexLetter(int16_t letter)
{
    if (65 <= letter && letter <= 70)
        return true;
    if (97 <= letter && letter <= 102)
        return true;
    return false;
}

static unsigned char charToHex(int16_t c)
{
    if (isdigit(c))
        return c - '0';
    else
        return c - 'A' + 10;
}

Array inputData(uint8_t lineNumber, int8_t *errorPtr)
{
    size_t n = 0, size = 4, number = 0;
    size_t *arr;
    int16_t digit;

    arr = malloc(size * sizeof(size_t));
	if (arr == NULL) returnError(0);

    while ((digit = getchar()) != '\n' && digit != EOF && *errorPtr == -1) {
        if (isspace(digit)) {
            if (number != 0) {
                arr[n] = number;
                number = 0;
                ++n;
            }
            if (n == size) {
                size *= 2;
                arr = realloc(arr, size * sizeof(size_t));

				if (arr == NULL)
					returnError(0);
			}
        }
        else if (isdigit(digit)) {
			if (number > SIZE_MAX / 10 || (number == SIZE_MAX / 10 && (size_t)(digit - 48) > SIZE_MAX % 10))
				*errorPtr = lineNumber;
			else
	            number = number * 10 + (digit - 48);
		}
        else
            *errorPtr = lineNumber;
    }
	if (number != 0) {
        arr[n] = number;
        ++n;
    }
	if (*errorPtr == -1 && n == 0)
		*errorPtr = lineNumber;

    Array result;
    result.size = n;
    result.arr = arr;

    return result;
}

static void move1Bit(unsigned char *hex, size_t idx)
{
    unsigned char res;
    for(int64_t i = idx; i >= 0; --i) {
        if (i == 0) res = 0;
        else res = (hex[i-1] % 16) << 4;
        res |= hex[i] / 16;
        hex[i] = res;
    }
}

static void moveArray(unsigned char *hex, size_t idx, size_t move)
{
    unsigned char temp;
    for (int64_t i = idx; i >= 0; --i) {
        temp = hex[i + move];
        hex[i + move] = hex[i];
        hex[i] = temp;
    }
}

static unsigned char *optionHex(size_t labirynthSize, int8_t *errorPtr)
{
    size_t n = 0, i = 0, size;
    unsigned char *hex, res = 0;
    int16_t c;

    if (labirynthSize % 8 == 0) size = labirynthSize / 8;
    else size = labirynthSize / 8 + 1;
    hex = calloc(size, sizeof(unsigned char));
	if (hex == NULL) returnError(0);

    c = getchar(); // Pozbywamy się zbędnego x'a w naszym zapisie szesnastkowym.
    if (c != 'x') *errorPtr = 4;

    bool check = false;
    while ((c = getchar()) == '0') check = true; // Pozbywamy sie zer wiodących.
    while (!isspace(c) && c != EOF && *errorPtr == -1) {
        if (isdigit(c) || isHexLetter(c)) {
            c = charToHex(toupper(c));
            if (i % 2 == 0)
                res = c << 4;
            if (i % 2 == 1) {
                res |= c;
				if (n < size) {
					hex[n] = res;
                	++n;
				}
				else *errorPtr = 4;
            }
        }
        else *errorPtr = 4;
        ++i;
        c = getchar();
    }
	while (c != '\n' && isspace(c)) c = getchar();
	if (c != '\n' && c != EOF) // Sprawdzam czy wystąpił niepożądany znak na przed końcem lini.
		*errorPtr = 4;
    if (*errorPtr == -1) { // Sprawdzam czy jest 5 linijka.
		if (i % 2 == 1) {
        	hex[n] = res;
        	move1Bit(hex, n);
        	++n;
    	}
   		if (c == '\n') {
        	c = getchar();
        	if(c != EOF)
    	        *errorPtr = 5;
	    }
		// Sprawdzam czy labirynt nie jest za duży i poprawność zapisu szesnastkowego.
	    if ((i == 0 && !check) || 4 * i > labirynthSize) *errorPtr = 4;
	    if (size - n > 0) moveArray(hex, n-1, size-n);
	}

    return hex;
}

static unsigned char *optionR(size_t labirynthSize, int8_t *errorPtr)
{
    size_t size, number = 0, i = 0;
    uint8_t *walls;
    int16_t c;

    if (labirynthSize % 8 == 0) size = labirynthSize / 8;
    else size = labirynthSize / 8 + 1;
    walls = calloc(size, sizeof(uint8_t));
	if (walls == NULL) returnError(0);

    bool check = false;
    uint32_t a, b, m, r, s, prev_s; 
	size_t w;
    uint32_t data[8];
    while ((c = getchar()) != '\n' && c != EOF && *errorPtr == -1) {
        if (isdigit(c)) {
            check = true;
			if (number > UINT32_MAX / 10 || (number == UINT32_MAX / 10 && (size_t)(c - 48) > UINT32_MAX % 10))
				*errorPtr = 4;
			else
	            number = number * 10 + (c - 48);
        }
        else if (isspace(c)) {
            if (check) {
                if (i >= 5)
					*errorPtr = 4;
				data[i] = number;
                number = 0;
                ++i;
                check = false;
            }
        }
        else
            *errorPtr = 4;
    }
    if (c == '\n') {
        c = getchar();
        if(c != EOF)
            *errorPtr = 5;
    }
    a = data[0]; b = data[1]; m = data[2]; r = data[3]; prev_s = data[4];
    if (check) {
        prev_s = number;
        ++i;
    }
    if (i != 5 || m == 0) *errorPtr = 4;

	if (*errorPtr == -1) {
    	for (i = 1; i <= r; ++i) {
        	s = ((a % m) * (prev_s % m) + b % m) % m;
        	w = s % labirynthSize;
        	while (w < labirynthSize && w < (UINT32_MAX - ((size_t)1 << 32))) {
            	if ((int)(walls[size - 1 - w / 8]) & (1 << (w % 8)))
                	break;
            	walls[size - 1 - w / 8] |= 1 << (w % 8);
            	w +=  (size_t)1 << 32;
        	}
        	prev_s = s;
    	}
	}
    return walls;
}

unsigned char *inputWalls(size_t labirynthSize, int8_t *errorPtr)
{
    int16_t option;
    unsigned char *walls = NULL;

    while((option = getchar()) && isspace(option) && option != '\n');
    if (option == '0')
        walls = optionHex(labirynthSize, errorPtr);
    else if (option == 'R')
        walls = optionR(labirynthSize, errorPtr);
    else
        *errorPtr = 4;

    return walls;
}
