// Nir Koren 316443902
#ifndef DISPATCHER_H
#define DISPATCHER_H
#include <pthread.h>
#include "unboundedBuffer.h"
#include "queue.h"
#include "producer.h"

typedef struct Dispatcher {
    UnboundedBuffer* newsQueue;
    UnboundedBuffer* sportsQueue;
    UnboundedBuffer* weatherQueue;
    pthread_t thread;
    int numProducers;
    Producer** producers;
} Dispatcher;

Dispatcher* createDispatcher(int numProducers, Producer** producers, int queuesSize);
void* runDispatcher(void* arg);
void destroyDispatcher(Dispatcher* dispatcher);
void joinDispatcher(Dispatcher* dispatcher);

#endif