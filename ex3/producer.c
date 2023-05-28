#include "producer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TYPES {"NEWS", "WHEATHER", "SPORTS"}

Producer* createProducer(BoundedBuffer* buffer, int id, int numItems) {
    Producer* producer = malloc(sizeof(Producer));
    producer->buffer = buffer;
    producer->id = id;
    producer->numItems = numItems;
    producer->numItemsByType[0] = 0;
    producer->numItemsByType[1] = 0;
    producer->numItemsByType[2] = 0;
    pthread_create(&producer->thread, NULL, runProducer, producer);
    return producer;
}

char* getRandomType() {
    char* types[] = TYPES;
    int numTypes = sizeof(types) / sizeof(char*);
    int typeIndex = rand() % numTypes;
    char* type = types[typeIndex];
    return type;
}

void* runProducer(void* arg) {
    char* type;
    Producer* producer = (Producer*) arg;
    int id = producer->id;
    int numItems = producer->numItems;
    char *item = malloc(sizeof(char) * 100);
    for (int i = 0; i < numItems; i++) {
        type = getRandomType();
        if (strcmp(type, "NEWS") == 0) {
            sprintf(item, "Producer %d %s %d", id, type, producer->numItemsByType[0]++);
        } else if (strcmp(type, "WHEATHER") == 0) {
            sprintf(item, "Producer %d %s %d", id, type, producer->numItemsByType[1]++);
        } else if (strcmp(type, "SPORTS") == 0) {
            sprintf(item, "Producer %d %s %d", id, type, producer->numItemsByType[2]++);
        }
        addBoundedBuffer(producer->buffer, item);
    }
    sprintf(item, "DONE");
    addBoundedBuffer(producer->buffer, item);
    free(item);
    return NULL;
}

void destroyProducer(Producer* producer) {
    pthread_join(producer->thread, NULL);
    free(producer);
}
