CC		= cc -Wall -Wextra
INST_PATH	= /usr/local/bin

baps1: main.o
	$(CC) -o baps1 main.o -lm

main.o: main.c
	$(CC) -c main.c
install:
	cp baps1 $(INST_PATH)/baps1
clean:
	rm baps1 *.o
