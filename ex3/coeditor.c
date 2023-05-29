// Nir Koren 316443902
#include "coeditor.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

Coeditor* createCoeditor(Dispatcher* dispatcher, char* type, BoundedBuffer* buffer) {
    Coeditor* coeditor = malloc(sizeof(Coeditor));
    coeditor->dispatcher = dispatcher;
    coeditor->buffer = buffer;
    coeditor->type = type;
    if (strcmp(type, "NEWS") == 0) {
        coeditor->queue = dispatcher->newsQueue;
    } else if (strcmp(type, "SPORTS") == 0) {
        coeditor->queue = dispatcher->sportsQueue;
    } else if (strcmp(type, "WEATHER") == 0) {
        coeditor->queue = dispatcher->weatherQueue;
    }
    pthread_create(&coeditor->thread, NULL, runCoeditor, coeditor);
    return coeditor;
}

void* runCoeditor(void* arg) {
    Coeditor* coeditor = (Coeditor*) arg;
    char* item;
    while (1) {
        item = removeUnboundedBuffer(coeditor->queue);
        if (strcmp(item, "DONE") == 0) {
            addBoundedBuffer(coeditor->buffer, item);
            break;
        }
        sleep(0.1);
        // add item to bounded buffer
        addBoundedBuffer(coeditor->buffer, item);
    }
    return NULL;
}

void destroyCoeditor(Coeditor* coeditor) {
    free(coeditor);
}

void joinCoeditor(Coeditor* coeditor) {
    pthread_join(coeditor->thread, NULL);
}