#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "error.h"


// Kolejka inspirowana kolejką z GeeksforGeeks
struct Queue {
    size_t front, back, size, capacity, sizeElem;
    size_t **array;
};

struct Queue* init(size_t sizeElem)
{
    struct Queue* queue = malloc(sizeof(struct Queue));
	if (queue == NULL)
		returnError(0);

	size_t capacity = 8;
    queue->sizeElem = sizeElem;
    queue->capacity = capacity;
    queue->front = queue->size = 0;

    queue->back = capacity - 1;
    queue->array = malloc(queue->capacity * sizeof(size_t*));
	if (queue->array == NULL)
		returnError(0);

	return queue;
}

bool isEmpty(struct Queue* queue)
{
    return (queue->size == 0);
}

void push(struct Queue* queue, size_t *element)
{
    if (queue->size == queue->capacity) {
		size_t **arr =  malloc(queue->size * sizeof(size_t*));
		if (arr == NULL)
			returnError(0);

        memcpy(arr, queue->array, queue->size * sizeof(size_t*));

        queue->capacity *= 2;
        queue->array = realloc(queue->array, queue->capacity * sizeof(size_t*));
		if (queue->array == NULL) returnError(0);

        for (size_t i = 0; i < queue->size; ++i)
            queue->array[(i - queue->front + queue->size) % (queue->size)] = arr[i];

        queue->front = 0;
        queue->back = queue->size-1;

		free(arr);
    }
    queue->back = (queue->back + 1) % queue->capacity;
    queue->array[queue->back] = element;
    queue->size = queue->size + 1;
}

size_t *front(struct Queue* queue)
{
	return queue->array[queue->front];
}

void pop(struct Queue* queue)
{
	free(queue->array[queue->front]);
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size -= 1;
}

void free_queue(struct Queue* queue)
{
	while (!isEmpty(queue)) {
		pop(queue);
	}
	free(queue->array);
	free(queue);
}
