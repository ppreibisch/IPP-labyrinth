#ifndef queue
#define queue

#include <stdbool.h>
#include <stdint.h>

struct Queue {
    size_t front, back, size, capacity;
    size_t **array;
};

struct Queue* init();
void push(struct Queue* q, size_t *k);
bool isEmpty(struct Queue* q);
void pop(struct Queue* q);
size_t *front(struct Queue* q);
void free_queue(struct Queue* q);

#endif
