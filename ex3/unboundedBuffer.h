// Nir Koren 316443902
#ifndef UNBOUNDEDBUFFER_H
#define UNBOUNDEDBUFFER_H
#include "semaphore.h"
#include "queue.h"

typedef struct UnboundedBuffer {
    Queue* queue;
    sem_t mutex;
    sem_t full;
} UnboundedBuffer;

UnboundedBuffer* createUnboundedBuffer(int size);
void addUnboundedBuffer(UnboundedBuffer* b, char* item);
char* removeUnboundedBuffer(UnboundedBuffer* b);
void destroyUnboundedBuffer(UnboundedBuffer* b);

#endif