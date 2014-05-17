CC = gcc 
CFLAGS = -g -Wall -ansi -DSTRATEGY=2
BIN = tstmerge tstalgorithms tstextreme tstmalloc tstmemory tstrealloc

all: $(BIN)
	exec ./RUN_TESTS

malloc.o: malloc.c
	$(CC) -c $(CFLAGS) -o $@ malloc.c

$(BIN): % : %.c malloc.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(BIN) malloc.o
