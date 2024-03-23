# Performance Benchmarking for sorted search

The goal of this project is to measure the performance of searching for an item within a static set. The factors being compared are the data layout and branchless vs branching code.

## Measured Structures / Algorithms

There are 2 data structures I am measuring performance of, a sorted set and a sorted map. For each data structure I am implementing 4 versions:

 - backed by sorted array and binary search
 - backed by sorted array and branchless binary search
 - backed by eytzinger layout
 - backed by eytzinger layout and a branchless search

In addition for the Map, I am comparing performance when the <Key, Value> is stored together versus in separate sections.


## Benchmarking strategy

I will benchmark the structures with varying sizes, to account for performance when memory does and doesnt fit in cache lines. The sizes I plan are:
 - powers of 2 from 2^1 to 2^24
 - multiples of 1000, from 1000 to 100,000

(i am including the powers of 2 due to potential issues with cache coherence)

### Tools
 - perf: record cpu cycles and cache misses

### Setup
 1. generate n random integers
 2. populate all structures
 3. generate 1 million queries

### Runs
 1. start with a dry run, not measured in benchmarks
 3. start timer
 2. run the 1 million queries
 4. end timer

### Interpretation

I will be looking
## Results

WIP.

## Inspiration

I recently came across [Algorithms for Modern Hardware by Sergey Slotin](https://en.algorithmica.org/hpc/). I found it to be a great read! He does a great job of gathering lots of useful information in one place. This inspired me to give performance benchmarking in cpp a try, specifically for the eytzinger layout and branch predication.

I have also came across [multiple](https://gamesfromwithin.com/data-oriented-design) [blogs](https://gameprogrammingpatterns.com/data-locality.html) on data oriented design, and wanted to test the performance impact on a structure like a map.
