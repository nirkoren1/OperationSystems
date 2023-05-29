// Nir Koren 316443902
#ifndef BOUNDEDBUFFER_H
#define BOUNDEDBUFFER_H
#include "semaphore.h"
#include "queue.h"

typedef struct BoundedBuffer {
    Queue* queue;
    sem_t mutex;
    sem_t empty;
    sem_t full;
} BoundedBuffer;

BoundedBuffer* createBoundedBuffer(int size);
void addBoundedBuffer(BoundedBuffer* b, char* item);
char* removeBoundedBuffer(BoundedBuffer* b);
void destroyBoundedBuffer(BoundedBuffer* b);

#endif