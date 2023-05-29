OBJS	= main.o queue.o boundedbuffer.o unboundedBuffer.o producer.o dispatcher.o coeditor.o screenmanager.o
SOURCE	= main.c queue.c boundedbuffer.c unboundedBuffer.c producer.c dispatcher.c coeditor.c screenmanager.c
HEADER	= queue.h boundedbuffer.h unboundedBuffer.h producer.h dispatcher.h coeditor.h screenmanager.h
OUT	= ex3.out
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = -lpthread

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c -std=c17

queue.o: queue.c
	$(CC) $(FLAGS) queue.c -std=c17

boundedbuffer.o: boundedbuffer.c
	$(CC) $(FLAGS) boundedbuffer.c -std=c17

unboundedBuffer.o: unboundedBuffer.c
	$(CC) $(FLAGS) unboundedBuffer.c -std=c17

producer.o: producer.c
	$(CC) $(FLAGS) producer.c -std=c17

dispatcher.o: dispatcher.c
	$(CC) $(FLAGS) dispatcher.c -std=c17

coeditor.o: coeditor.c
	$(CC) $(FLAGS) coeditor.c -std=c17

screenmanager.o: screenmanager.c
	$(CC) $(FLAGS) screenmanager.c -std=c17


clean:
	rm -f $(OBJS) $(OUT)