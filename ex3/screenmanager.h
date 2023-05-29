// Nir Koren 316443902
#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H
#include <pthread.h>
#include "boundedbuffer.h"
#include "producer.h"

typedef struct ScreenManager {
    BoundedBuffer *buffer;
    pthread_t thread;
} ScreenManager;

ScreenManager* createScreenManager(BoundedBuffer* buffer);
void* runScreenManager(void* arg);
void destroyScreenManager(ScreenManager* screenManager);
void joinScreenManager(ScreenManager* screenManager);

#endif