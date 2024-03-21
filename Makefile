CC = g++
DEBUG_FLAGS = -Wall -Wextra -std=c++20
BENCH_FLAGS = -O3 -march=native
FILES = ./src/main.cpp ./src/eytzinger.cpp
BRANCHLESS_FILES = ./src/main.cpp ./src/eytzinger_branchless.cpp

all: debug bench

debug: debug_version debug_branchless_version

bench: bench_version bench_branchless_version

debug_version: $(FILES)
	$(CC) $(DEBUG_FLAGS) $^ -o debug

debug_branchless_version: $(BRANCHLESS_FILES)
	$(CC) $(DEBUG_FLAGS) $^ -o debug_branchless

bench_version: $(FILES) 
	$(CC) $(BENCH_FLAGS) $^ -o bench

bench_branchless_version: $(BRANCHLESS_FILES)
	$(CC) $(BENCH_FLAGS) $^ -o bench_branchless

clean:
	rm -f debug debug_branchless bench bench_branchless

