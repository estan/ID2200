CC = gcc
CFLAGS = -g -Wall -ansi -D_GNU_SOURCE -DSTRATEGY=2
BENCHMARK_CFLAGS = -O4 -Wall -ansi -D_GNU_SOURCE

TESTS = tstmerge \
        tstalgorithms \
        tstextreme \
        tstmalloc \
        tstmemory \
        tstrealloc

BENCHMARKS = benchmark-random-first-fit \
             benchmark-random-best-fit \
             benchmark-random-system \
             benchmark-worst-first-fit \
             benchmark-worst-best-fit \
             benchmark-worst-system \
             benchmark-best-first-fit \
             benchmark-best-best-fit \
             benchmark-best-system

all: $(TESTS) $(BENCHMARKS)
	#exec ./RUN_TESTS

malloc.o: malloc.c
	$(CC) -c $(CFLAGS) -o $@ $^

$(TESTS): % : malloc.o %.c
	$(CC) $(CFLAGS) -o $@ $^

benchmark-random-first-fit: malloc.c benchmark-random.c
	$(CC) $(BENCHMARK_CFLAGS) -DSTRATEGY=1 -o $@ $^

benchmark-random-best-fit: malloc.c benchmark-random.c
	$(CC) $(BENCHMARK_CFLAGS) -DSTRATEGY=2 -o $@ $^

benchmark-random-system: benchmark-random.c
	$(CC) $(BENCHMARK_CFLAGS) -DUSE_SYSTEM_MALLOC=1 -o $@ $^

benchmark-worst-first-fit: malloc.c benchmark-worst.c
	$(CC) $(BENCHMARK_CFLAGS) -DSTRATEGY=1 -o $@ $^

benchmark-worst-best-fit: malloc.c benchmark-worst.c
	$(CC) $(BENCHMARK_CFLAGS) -DSTRATEGY=2 -o $@ $^

benchmark-worst-system: benchmark-worst.c
	$(CC) $(BENCHMARK_CFLAGS) -DUSE_SYSTEM_MALLOC=1 -o $@ $^

benchmark-best-first-fit: malloc.c benchmark-best.c
	$(CC) $(BENCHMARK_CFLAGS) -DSTRATEGY=1 -o $@ $^

benchmark-best-best-fit: malloc.c benchmark-best.c
	$(CC) $(BENCHMARK_CFLAGS) -DSTRATEGY=2 -o $@ $^

benchmark-best-system: benchmark-best.c
	$(CC) $(BENCHMARK_CFLAGS) -DUSE_SYSTEM_MALLOC=1 -o $@ $^

clean:
	rm -f $(TESTS) $(BENCHMARKS) malloc.o
