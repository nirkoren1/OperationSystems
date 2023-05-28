#include "boundedbuffer.h"
#include <stdlib.h>

BoundedBuffer* createBoundedBuffer(int size) {
    BoundedBuffer* b = malloc(sizeof(BoundedBuffer));
    b->queue = createQue(size);
    pthread_mutex_init(&b->mutex, NULL);
    pthread_cond_init(&b->notFull, NULL);
    pthread_cond_init(&b->notEmpty, NULL);
    return b;
}

void addBoundedBuffer(BoundedBuffer* b, char* item) {
    pthread_mutex_lock(&b->mutex);
    while (isFull(b->queue)) {
        pthread_cond_wait(&b->notFull, &b->mutex);
    }
    enqueue(b->queue, item);
    pthread_cond_signal(&b->notEmpty);
    pthread_mutex_unlock(&b->mutex);
}

char* removeBoundedBuffer(BoundedBuffer* b) {
    pthread_mutex_lock(&b->mutex);
    while (isEmpty(b->queue)) {
        pthread_cond_wait(&b->notEmpty, &b->mutex);
    }
    char* item = dequeue(b->queue);
    pthread_cond_signal(&b->notFull);
    pthread_mutex_unlock(&b->mutex);
    return item;
}

void destroyBoundedBuffer(BoundedBuffer* b) {
    destroyQue(b->queue);
    pthread_mutex_destroy(&b->mutex);
    pthread_cond_destroy(&b->notFull);
    pthread_cond_destroy(&b->notEmpty);
    free(b);
}