// Nir Koren 316443902
#ifndef QUEUE_H
#define QUEUE_H

typedef struct QueueNode {
    char* data;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
} Queue;

Queue* createQueue();

int isEmpty(Queue* queue);

void enqueue(Queue* queue, char* data);

char* dequeue(Queue* queue);

void freeQueue(Queue* queue);

#endif