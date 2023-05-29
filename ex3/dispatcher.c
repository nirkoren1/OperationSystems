// Nir Koren 316443902
#include "dispatcher.h"
#include "unboundedBuffer.h"
#include <stdlib.h>
#include <string.h>

Dispatcher* createDispatcher(int numProducers, Producer** producers, int size) {
    Dispatcher* dispatcher = malloc(sizeof(Dispatcher));
    dispatcher->newsQueue = createUnboundedBuffer(size);
    dispatcher->sportsQueue = createUnboundedBuffer(size);
    dispatcher->weatherQueue = createUnboundedBuffer(size);
    dispatcher->numProducers = numProducers;
    dispatcher->producers = producers;
    pthread_create(&dispatcher->thread, NULL, runDispatcher, dispatcher);
    return dispatcher;
}

void* runDispatcher(void* arg) {
    Dispatcher* dispatcher = (Dispatcher*) arg;
    int numProducers = dispatcher->numProducers;
    Producer** producers = dispatcher->producers;
    int* validIndex = malloc(sizeof(int) * numProducers);
    for (int i = 0; i < numProducers; i++) {
        validIndex[i] = 1;
    }
    int numDone = 0;
    char* dispatcherItem;
    int j = 0;
    int index = 0;
    char *token;
    char* producedItem = malloc(sizeof(char) * 100);
    while (numDone < numProducers) {
        while (j == 5 || validIndex[index] == 0) {
            j = 0;
            index = (index + 1) % numProducers;
        }
        dispatcherItem = removeBoundedBuffer(producers[index]->buffer);
        if (dispatcherItem == NULL) {
            j++;
            continue;
        }
        strcpy(producedItem, dispatcherItem);
        if (strcmp(dispatcherItem, "DONE") == 0) {
            numDone++;
            validIndex[index] = 0;
            index = (index + 1) % numProducers;
            j++;
            continue;
        }
        // get the type of the dispatcherItem
        token = strtok(producedItem, " ");
        token = strtok(NULL, " ");
        token = strtok(NULL, " ");
        if (token == NULL) {
            j++;
            continue;
        }
        if (strcmp(token, "NEWS") == 0) {
            addUnboundedBuffer(dispatcher->newsQueue, dispatcherItem);
        } else if (strcmp(token, "SPORTS") == 0) {
            addUnboundedBuffer(dispatcher->sportsQueue, dispatcherItem);
        } else if (strcmp(token, "WHEATHER") == 0) {
            addUnboundedBuffer(dispatcher->weatherQueue, dispatcherItem);
        }
        j++;
    }
    addUnboundedBuffer(dispatcher->newsQueue, "DONE");
    addUnboundedBuffer(dispatcher->sportsQueue, "DONE");
    addUnboundedBuffer(dispatcher->weatherQueue, "DONE");
    return NULL;
}

void destroyDispatcher(Dispatcher* dispatcher) {
    destroyUnboundedBuffer(dispatcher->newsQueue);
    destroyUnboundedBuffer(dispatcher->sportsQueue);
    destroyUnboundedBuffer(dispatcher->weatherQueue);
    free(dispatcher);
}

void joinDispatcher(Dispatcher* dispatcher) {
    pthread_join(dispatcher->thread, NULL);
}