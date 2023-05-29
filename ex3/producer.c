// Nir Koren 316443902
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
    for (int i = 0; i < numItems; i++) {
        char *producerItem = malloc(sizeof(char) * 100);
        type = getRandomType();
        if (strcmp(type, "NEWS") == 0) {
            sprintf(producerItem, "Producer %d %s %d", id, type, producer->numItemsByType[0]++);
        } else if (strcmp(type, "WHEATHER") == 0) {
            sprintf(producerItem, "Producer %d %s %d", id, type, producer->numItemsByType[1]++);
        } else if (strcmp(type, "SPORTS") == 0) {
            sprintf(producerItem, "Producer %d %s %d", id, type, producer->numItemsByType[2]++);
        } else {
            continue;
        }
        addBoundedBuffer(producer->buffer, producerItem);
    }
    addBoundedBuffer(producer->buffer, "DONE");
    return NULL;
}

void destroyProducer(Producer* producer) {
    destroyBoundedBuffer(producer->buffer);
    free(producer);
}

void joinProducer(Producer* producer) {
    pthread_join(producer->thread, NULL);
}
