#include "queue.h"
#include <stdlib.h>

Queue* createQue(int size) {
    Queue* q = malloc(sizeof(Queue));
    q->buffer = malloc(sizeof(char*) * size);
    q->head = 0;
    q->tail = 0;
    q->size = size;
    q->count = 0;
    return q;
}

void enqueue(Queue* q, char* item) {
    q->buffer[q->tail] = item;
    q->tail = (q->tail + 1) % q->size;
    q->count++;
}

char* dequeue(Queue* q) {
    char* item = q->buffer[q->head];
    q->head = (q->head + 1) % q->size;
    q->count--;
    return item;
}

int isEmpty(Queue* q) {
    return q->count == 0;
}

int isFull(Queue* q) {
    return q->count == q->size;
}

void destroyQue(Queue* q) {
    free(q->buffer);
    free(q);
}