CC=gcc
CFLAGS=-I.
DEPS = action.h board.h
OBJ = main.o board.o action.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

basilisk: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
