// Nir Koren 316443902
#include "boundedbuffer.h"
#include <stdlib.h>
#include "semaphore.h"

BoundedBuffer* createBoundedBuffer(int size) {
    BoundedBuffer* b = malloc(sizeof(BoundedBuffer));
    b->queue = createQueue();
    sem_init(&b->mutex, 0, 1);
    sem_init(&b->empty, 0, size);
    sem_init(&b->full, 0, 0);
    return b;
}

void addBoundedBuffer(BoundedBuffer* b, char* item) {
    sem_wait(&b->empty);
    sem_wait(&b->mutex);
    enqueue(b->queue, item);
    sem_post(&b->mutex);
    sem_post(&b->full);
}

char* removeBoundedBuffer(BoundedBuffer* b) {
    sem_wait(&b->full);
    sem_wait(&b->mutex);
    char* item = dequeue(b->queue);
    sem_post(&b->mutex);
    sem_post(&b->empty);
    return item;
}

void destroyBoundedBuffer(BoundedBuffer* b) {
    freeQueue(b->queue);
    sem_destroy(&b->mutex);
    sem_destroy(&b->empty);
    sem_destroy(&b->full);
    free(b);
}