CC = gcc
CFLAGS = -g -Wall -ansi -D_GNU_SOURCE -DSTRATEGY=2
BENCHMARK_CFLAGS = -O4 -Wall -ansi -D_GNU_SOURCE

TESTS = tstmerge \
        tstalgorithms \
        tstextreme \
        tstmalloc \
        tstmemory \
        tstrealloc

BENCHMARKS = benchmark-small-first-fit \
             benchmark-small-best-fit \
             benchmark-small-system \
             benchmark-big-first-fit \
             benchmark-big-best-fit \
             benchmark-big-system


all: $(TESTS) $(BENCHMARKS)
	#exec ./RUN_TESTS

malloc.o: malloc.c
	$(CC) -c $(CFLAGS) -o $@ $^

$(TESTS): % : malloc.o %.c
	$(CC) $(CFLAGS) -o $@ $^

benchmark-small-first-fit: malloc.c benchmark-small.c
	$(CC) $(BENCHMARK_CFLAGS) -DSTRATEGY=1 -o $@ $^

benchmark-small-best-fit: malloc.c benchmark-small.c
	$(CC) $(BENCHMARK_CFLAGS) -DSTRATEGY=2 -o $@ $^

benchmark-small-system: benchmark-small.c
	$(CC) $(BENCHMARK_CFLAGS) -DUSE_SYSTEM_MALLOC=1 -o $@ $^

benchmark-big-first-fit: malloc.c benchmark-big.c
	$(CC) $(BENCHMARK_CFLAGS) -DSTRATEGY=1 -o $@ $^

benchmark-big-best-fit: malloc.c benchmark-big.c
	$(CC) $(BENCHMARK_CFLAGS) -DSTRATEGY=2 -o $@ $^

benchmark-big-system: benchmark-big.c
	$(CC) $(BENCHMARK_CFLAGS) -DUSE_SYSTEM_MALLOC=1 -o $@ $^

clean:
	rm -f $(TESTS) $(BENCHMARKS) malloc.o
