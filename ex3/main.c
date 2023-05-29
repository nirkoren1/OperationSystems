// Nir Koren 316443902
#include "producer.h"
#include "dispatcher.h"
#include "screenmanager.h"
#include "coeditor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <input file>\n", argv[0]);
        exit(1);
    }
    // create producers array
    Producer** producers = malloc(sizeof(Producer*));
    int producers_size = 0;
    // read input file
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error: Cannot open file %s\n", argv[1]);
        exit(1);
    }
    // read the lines of the file
    char line[100];
    char* lines[4] = {malloc(100), malloc(100), malloc(100), malloc(100)};
    int sumNumItems = 0;
    int index = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        strcpy(lines[index++], line);
        if (index == 4) {
            // create the producer
            BoundedBuffer *buffer = createBoundedBuffer(atoi(lines[2]));
            Producer* producer = createProducer(buffer, atoi(lines[0]), atoi(lines[1]));
            producers = realloc(producers, sizeof(Producer*) * ++producers_size);
            producers[producers_size - 1] = producer;
            sumNumItems += atoi(lines[1]);
            index = 0;
        }
    }
    fclose(file);
    // create dispatcher
    Dispatcher* dispatcher = createDispatcher(producers_size, producers, sumNumItems);
    // create bounded buffer for screen manager and coeditors
    BoundedBuffer* screenManagerBuffer = createBoundedBuffer(atoi(lines[0]));
    // create screen manager
    ScreenManager* screenManager = createScreenManager(screenManagerBuffer);
    // create coeditors
    Coeditor* newsCoeditor = createCoeditor(dispatcher, "NEWS", screenManagerBuffer);
    Coeditor* sportsCoeditor = createCoeditor(dispatcher, "SPORTS", screenManagerBuffer);
    Coeditor* weatherCoeditor = createCoeditor(dispatcher, "WEATHER", screenManagerBuffer);
    // join all producers
    for (int i = 0; i < producers_size; i++) {
        joinProducer(producers[i]);
    }
    // join all coeditors
    joinCoeditor(newsCoeditor);
    joinCoeditor(sportsCoeditor);
    joinCoeditor(weatherCoeditor);
    // join screen manager
    joinScreenManager(screenManager);
    //join dispatcher
    joinDispatcher(dispatcher);
    // destroy all producers
    for (int i = 0; i < producers_size; i++) {
        destroyProducer(producers[i]);
    }
    // destroy dispatcher
    destroyDispatcher(dispatcher);
    // destroy coeditors
    destroyCoeditor(newsCoeditor);
    destroyCoeditor(sportsCoeditor);
    destroyCoeditor(weatherCoeditor);
    // destroy screen manager
    destroyScreenManager(screenManager);
    // destroy bounded buffer for screen manager and coeditors
    destroyBoundedBuffer(screenManagerBuffer);
    // destroy producers array
    free(producers);
    // free lines
    for (int i = 0; i < 4; i++) {
        free(lines[i]);
    }
    return 0;
}