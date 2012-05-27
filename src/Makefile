INST_PATH = /usr/local/bin
OBJS = baps1.o timediff.o
CFLAGS = -Wall -Wextra
LDFLAGS = -lm

baps1: $(OBJS)

install: baps1
	install baps1 $(INST_PATH)/baps1

uninstall:
	rm $(INST_PATH)/baps1

.PHONY: clean
clean:
	rm baps1 $(OBJS)
