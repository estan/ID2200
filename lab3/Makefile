CC = gcc
CFLAGS = -g -Wall -ansi -D_GNU_SOURCE -DSTRATEGY=2
BENCHMARK_CFLAGS = -O4 -Wall -ansi -D_GNU_SOURCE

TESTS = tstmerge \
        tstalgorithms \
        tstextreme \
        tstmalloc \
        tstmemory \
        tstrealloc

BENCHMARKS = benchmark-worst-first-fit \
             benchmark-worst-best-fit \
             benchmark-worst-system \
             benchmark-block-sizes-first-fit \
             benchmark-block-sizes-best-fit \
             benchmark-block-sizes-system \
			 benchmark-memory-first-fit \
			 benchmark-memory-best-fit

all: $(TESTS) $(BENCHMARKS)
	#exec ./RUN_TESTS

malloc.o: malloc.c
	$(CC) -c $(CFLAGS) -o $@ $^

$(TESTS): % : malloc.o %.c
	$(CC) $(CFLAGS) -o $@ $^

benchmark-worst-first-fit: malloc.c benchmark-worst.c
	$(CC) $(BENCHMARK_CFLAGS) -DSTRATEGY=1 -o $@ $^

benchmark-worst-best-fit: malloc.c benchmark-worst.c
	$(CC) $(BENCHMARK_CFLAGS) -DSTRATEGY=2 -o $@ $^

benchmark-worst-system: benchmark-worst.c
	$(CC) $(BENCHMARK_CFLAGS) -DUSE_SYSTEM_MALLOC=1 -o $@ $^

benchmark-block-sizes-first-fit: malloc.c benchmark-block-sizes.c
	$(CC) $(BENCHMARK_CFLAGS) -DSTRATEGY=1 -o $@ $^

benchmark-block-sizes-best-fit: malloc.c benchmark-block-sizes.c
	$(CC) $(BENCHMARK_CFLAGS) -DSTRATEGY=2 -o $@ $^

benchmark-block-sizes-system: benchmark-block-sizes.c
	$(CC) $(BENCHMARK_CFLAGS) -DUSE_SYSTEM_MALLOC=1 -o $@ $^

benchmark-memory-first-fit: malloc.c benchmark-memory.c
	$(CC) $(BENCHMARK_CFLAGS) -DSTRATEGY=1 -o $@ $^

benchmark-memory-best-fit: malloc.c benchmark-memory.c
	$(CC) $(BENCHMARK_CFLAGS) -DSTRATEGY=2 -o $@ $^

clean:
	rm -f $(TESTS) $(BENCHMARKS) malloc.o
