CC		= cc -Wall -Wextra
INST_PATH	= /usr/local/bin

baps1: main.o timediff.o
	$(CC) -o baps1 main.o timediff.o -lm

main.o: main.c
	$(CC) -c main.c

timediff.o: timediff.c
	$(CC) -c timediff.c

install: baps1
	install baps1 $(INST_PATH)/baps1

uninstall:
	rm $(INST_PATH)/baps1

clean:
	rm baps1 *.o
