// Nir Koren 316443902
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

int isEmpty(Queue* queue) {
    return queue->front == NULL;
}

void enqueue(Queue* queue, char* data) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->data = data;
    newNode->next = NULL;

    if (isEmpty(queue)) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

char* dequeue(Queue* queue) {
    if (isEmpty(queue)) {
        printf("Error: Queue is empty.\n");
        return NULL;
    }
    QueueNode* temp = queue->front;
    char* data = temp->data;

    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);
    return data;
}

void freeQueue(Queue* queue) {
    while (!isEmpty(queue)) {
        char* data = dequeue(queue);
        free(data);
    }
    free(queue);
}