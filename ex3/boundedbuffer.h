#ifndef BOUNDEDBUFFER_H
#define BOUNDEDBUFFER_H
#include <pthread.h>
#include "queue.h"

typedef struct BoundedBuffer {
    Queue* queue;
    pthread_mutex_t mutex;
    pthread_cond_t notFull;
    pthread_cond_t notEmpty;
} BoundedBuffer;

BoundedBuffer* createBoundedBuffer(int size);
void addBoundedBuffer(BoundedBuffer* b, char* item);
char* removeBoundedBuffer(BoundedBuffer* b);
void destroyBoundedBuffer(BoundedBuffer* b);

#endif