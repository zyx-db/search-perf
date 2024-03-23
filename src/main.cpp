#include <algorithm>
#include <set>
#include <cstdlib>
#include <vector>
#include <chrono>
#include <iostream>

#include "set.cpp"

int QUERIES = 1e6;

struct BenchData{
  public:

    std::chrono::nanoseconds set_bin_search;
    std::chrono::nanoseconds bl_set_bin_search;
    std::chrono::nanoseconds set_eytzinger;
    std::chrono::nanoseconds bl_set_eytzinger;

    std::chrono::nanoseconds map_bin_search;
    std::chrono::nanoseconds map_eytzinger;

    std::chrono::nanoseconds packed_bin_search;
    std::chrono::nanoseconds packed_eytzinger;
  
    void headers() {
      using namespace std;
      cout << "size,binary set,eytzinger set,bl binary set,bl eytzinger set" << endl;
    }

    void print(unsigned long size) {
      using namespace std;
      cout << size;
      cout << "," << set_bin_search.count() / QUERIES;
      cout << "," << set_eytzinger.count() / QUERIES;
      cout << "," << bl_set_bin_search.count() / QUERIES;
      cout << "," << bl_set_eytzinger.count() / QUERIES;
      cout << endl;
    }
};

template <typename T>
void time_set(
    sets::Base<T> &s,
    std::vector<T> &q,
    std::chrono::nanoseconds &res
    ){
  std::chrono::steady_clock::time_point begin;
  std::chrono::steady_clock::time_point end; 

  volatile int checksum = 0;
  begin = std::chrono::steady_clock::now();
  for (auto x: q){
    checksum ^= s.contains(x);
  }
  end = std::chrono::steady_clock::now(); 
  res = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
};

void benchmark(unsigned size, BenchData &result){
  std::vector<int> data;
  data.reserve(size);
  std::set <int> values;
  std::vector<int> queries;
  while (values.size() < size){
    values.insert(rand());
  }
  for (auto x: values){
    data.push_back(x);
  }
  std::sort(data.begin(), data.end());
  for (int i = 0; i < QUERIES; i++){
    queries.push_back(rand());
  }

  sets::ESet<int> eset(data);
  time_set(eset, queries, result.set_eytzinger);

  sets::Set<int> set(data);
  time_set(set, queries, result.set_bin_search);

  sets::BL_ESet<int> bl_eset(data);
  time_set(bl_eset, queries, result.bl_set_eytzinger);

  sets::BL_Set<int> bl_set(data);
  time_set(bl_set, queries, result.bl_set_bin_search);
}

int main(){
  using namespace std;

  vector<unsigned long> benchmark_sizes;
  for (int i = 9; i <= 24; i++){
    benchmark_sizes.push_back(1 << i);
  }
  {
    unsigned long q = 1000;
    while (q <= 17000000){
      benchmark_sizes.push_back(q); 
      q += 1000;
    }
  }
  sort(benchmark_sizes.begin(), benchmark_sizes.end());
  BenchData x; 

  x.headers();
  for (auto size: benchmark_sizes){
    cerr << "size: " << size << endl;
    benchmark(size, x);
    x.print(size);
  }
}
