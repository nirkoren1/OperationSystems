#ifndef DISPATCHER_H
#define DISPATCHER_H
#include <pthread.h>
#include "queue.h"
#include "producer.h"

typedef struct Dispatcher {
    Queue* newsQueue;
    Queue* sportsQueue;
    Queue* weatherQueue;
    pthread_t thread;
    int numProducers;
    Producer** producers;
} Dispatcher;

Dispatcher* createDispatcher(int numProducers, Producer** producers);
void* runDispatcher(void* arg);
void destroyDispatcher(Dispatcher* dispatcher);

#endif