#ifndef QUEUE_H
#define QUEUE_H

typedef struct Queue {
    char** buffer;
    int head;
    int tail;
    int size;
    int count;
} Queue;

Queue* createQue(int size);
void enqueue(Queue* q, char* item);
char* dequeue(Queue* q);
int isEmpty(Queue* q);
int isFull(Queue* q);
void destroyQue(Queue* q);

#endif