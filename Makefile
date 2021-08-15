CC = gcc
CFLAGS = -g -Wall
OBJECTS = sfind.o sfind_helper_functions.o

all:sfind

sfind: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

%.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f sfind
	rm -f sfind.o
	rm -f sfind_helper_functions.o
	rm -f sfind.dSYM
