// Nir Koren 316443902
#include "unboundedBuffer.h"
#include <stdlib.h>
#include "semaphore.h"

UnboundedBuffer* createUnboundedBuffer(int size) {
    UnboundedBuffer* b = malloc(sizeof(UnboundedBuffer));
    b->queue = createQueue();
    sem_init(&b->mutex, 0, 1);
    sem_init(&b->full, 0, 0);
    return b;
}

void addUnboundedBuffer(UnboundedBuffer* b, char* item) {
    sem_wait(&b->mutex);
    enqueue(b->queue, item);
    sem_post(&b->mutex);
    sem_post(&b->full);
}

char* removeUnboundedBuffer(UnboundedBuffer* b) {
    sem_wait(&b->full);
    sem_wait(&b->mutex);
    char* item = dequeue(b->queue);
    sem_post(&b->mutex);
    return item;
}

void destroyUnboundedBuffer(UnboundedBuffer* b) {
    freeQueue(b->queue);
    sem_destroy(&b->mutex);
    sem_destroy(&b->full);
    free(b);
}