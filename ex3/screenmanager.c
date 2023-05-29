// Nir Koren 316443902
#include "screenmanager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ScreenManager* createScreenManager(BoundedBuffer* buffer) {
    ScreenManager* screenManager = malloc(sizeof(ScreenManager));
    screenManager->buffer = buffer;
    pthread_create(&screenManager->thread, NULL, runScreenManager, screenManager);
    return screenManager;
}

void* runScreenManager(void* arg) {
    ScreenManager* screenManager = (ScreenManager*) arg;
    int dones = 0;
    char* screenItem;
    while (1) {
        screenItem = removeBoundedBuffer(screenManager->buffer);
        if (strcmp(screenItem, "DONE") == 0) {
            dones++;
            if (dones == 3) {
                break;
            }
            continue;
        }
        printf("%s\n", screenItem);
        free(screenItem);
    }
    printf("DONE");
    return NULL;
}

void destroyScreenManager(ScreenManager* screenManager) {
    free(screenManager);
}

void joinScreenManager(ScreenManager* screenManager) {
    pthread_join(screenManager->thread, NULL);
}