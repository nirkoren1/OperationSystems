// Nir Koren 316443902
#ifndef COEDITOR_H
#define COEDITOR_H
#include <pthread.h>
#include "unboundedBuffer.h"
#include "producer.h"
#include "dispatcher.h"

typedef struct Coeditor {
    Dispatcher* dispatcher;
    UnboundedBuffer* queue;
    BoundedBuffer *buffer;
    char *type;
    pthread_t thread;
} Coeditor;

Coeditor* createCoeditor(Dispatcher* dispatcher, char* type, BoundedBuffer* buffer);
void* runCoeditor(void* arg);
void destroyCoeditor(Coeditor* coeditor);
void joinCoeditor(Coeditor* coeditor);

#endif