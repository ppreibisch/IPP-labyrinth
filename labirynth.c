#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "input.h"
#include "queue.h"
#include "correct.h"
#include "error.h"

static bool isVisited(unsigned char *visited, size_t size, size_t index)
{
    return (int)(visited[size - 1 - index/8]) & (1 << (index % 8));
}

static void visit(unsigned char *visited, size_t size, size_t index)
{
    visited[size - 1 - index/8] |= (1 << (index % 8));
}

static size_t *calcProduct(size_t *labirynthSizes, size_t n)
{
    size_t *res;
    res = malloc(n * sizeof(size_t));

    res[0] = 1;
    for(size_t i = 1; i < n; ++i)
        res[i] = res[i-1] * labirynthSizes[i-1];

    return res;
}

int main()
{
    int8_t errorNumber = -1;
    int8_t *errorPtr = &errorNumber;
    Array labirynth = inputData(1, errorPtr);
    if (errorNumber != -1) {
        free(labirynth.arr);
        returnError(errorNumber);
    }
    Array startCoordinates = inputData(2, errorPtr);
    if (errorNumber != -1) {
        free(labirynth.arr);
        free(startCoordinates.arr);
        returnError(errorNumber);
    }
    Array endCoordinates = inputData(3, errorPtr);
    if (errorNumber != -1) {
        free(labirynth.arr);
        free(startCoordinates.arr);
        free(endCoordinates.arr);
        returnError(errorNumber);
    }

    checkErrors(labirynth, startCoordinates, endCoordinates);
    size_t n = labirynth.size;

    // Tablica iloczynów po rozmiarach n[k] oznacza  n1*n2*...*nk.
    size_t *product = calcProduct(labirynth.arr, n);
	if (product == NULL) returnError(0);

    size_t size = product[n - 1] * labirynth.arr[n - 1];

    size_t wallsSize;
    if (size % 8 == 0) wallsSize = size / 8;
    else wallsSize = size / 8 + 1;

    unsigned char *visited = inputWalls(size, errorPtr);

    size_t sum = 0, end_sum = 0, start_sum = 0;
    for (size_t i = 0; i < n; ++i) {
        start_sum += (startCoordinates.arr[i] - 1) * product[i];
        end_sum += (endCoordinates.arr[i] - 1) * product[i]; // Suma policzona ze wzoru definiujaca koniec trasy.
    }

    if (errorNumber == -1 && (isVisited(visited, wallsSize, start_sum) || isVisited(visited, wallsSize, end_sum)))
       errorNumber = 4;

    if (errorNumber != -1) {
        free(labirynth.arr);
        free(startCoordinates.arr);
        free(endCoordinates.arr);
        free(visited);
		free(product);
        returnError(errorNumber);
    }

    struct Queue* q = init(n);
	size_t *startPos = malloc(n * sizeof(size_t));
	if (startPos == NULL) returnError(0);
    memcpy(startPos, startCoordinates.arr, n * sizeof(size_t));
    push(q, startPos);

    bool found = false;
    visit(visited, wallsSize, start_sum);
    size_t level = 0, act = 1, next = 0;
    while (!isEmpty(q)) {
        size_t *actPos = front(q);
		if (actPos == NULL || q == NULL) returnError(0);

        sum = 0;
        for (size_t i = 0; i < n; ++i)
            sum += (actPos[i] - 1) * product[i];

        if (sum == end_sum)
            break;

        for (size_t i = 0; i < n; ++i) {
            if (actPos[i] + 1 <= labirynth.arr[i] && !isVisited(visited, wallsSize, sum + product[i])) {
                if (sum + product[i] == end_sum) {
					found = true;
					break;
				}
				size_t *newAct = malloc(n * sizeof(size_t));
				if (newAct == NULL) returnError(0);
                memcpy(newAct, actPos, n * sizeof(size_t)); // Tworzymy nową tablice by nie zmieniac wartosci w srodku bfs'a zmieniajac wskaznik.
                ++newAct[i];
                visit(visited, wallsSize, sum + product[i]);
                push(q, newAct);
                ++next;
            }
            if (actPos[i] - 1 >= 1 && !isVisited(visited, wallsSize, sum - product[i])) {
                if (sum - product[i] == end_sum) {
					found = true;
					break;
				}
				size_t *newAct = malloc(n * sizeof(size_t));
				if (newAct == NULL) returnError(0);
                memcpy(newAct, actPos, n * sizeof(size_t));
                --newAct[i];
                visit(visited, wallsSize, sum - product[i]);
                push(q, newAct);
                ++next;
            }
        }
		pop(q);
        if (found) {
            ++level;
            break;
        }
        --act;
        if (act == 0) {
            act = next;
            next = 0;
            ++level;
        }
    }
	if (q == NULL) returnError(0);
    if (sum == end_sum || found) printf("%zu\n", level);
    else printf("NO WAY\n");

	free_queue(q);
    free(product);
	free(visited);
    free(labirynth.arr);
    free(startCoordinates.arr);
    free(endCoordinates.arr);

    return 0;
}
