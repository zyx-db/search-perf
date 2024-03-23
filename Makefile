CC = g++
DEBUG_FLAGS = -Wall -Wextra -std=c++20 -fsanitize=address -fsanitize=undefined -fsanitize=leak
BENCH_FLAGS = -O3 -march=native -std=c++20
FILES = src/main.cpp src/set.cpp

all: debug bench

debug: debug_version

bench: bench_version

debug_version: $(FILES)
	$(CC) $(DEBUG_FLAGS) $^ -o debug

bench_version: $(FILES) 
	$(CC) $(BENCH_FLAGS) $^ -o bench

clean:
	rm -f debug debug_branchless bench bench_branchless

