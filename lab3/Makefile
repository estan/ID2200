CC = gcc 
CFLAGS = -g -Wall -ansi -D_GNU_SOURCE -DSTRATEGY=2
BIN = tstmerge tstalgorithms tstextreme tstmalloc tstmemory tstrealloc

all: $(BIN)
	exec ./RUN_TESTS

malloc.o: malloc.c
	$(CC) -c $(CFLAGS) -o $@ malloc.c

$(BIN): % : malloc.o %.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(BIN) malloc.o
