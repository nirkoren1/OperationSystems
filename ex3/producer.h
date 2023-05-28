#ifndef PRODUCER_H
#define PRODUCER_H
#include <pthread.h>
#include "boundedbuffer.h"

typedef struct Producer {
    BoundedBuffer* buffer;
    pthread_t thread;
    int id;
    int numItems;
    int numItemsByType[3];
} Producer;

Producer* createProducer(BoundedBuffer* buffer, int id, int numItems);
void* runProducer(void* arg);
void destroyProducer(Producer* producer);

#endif