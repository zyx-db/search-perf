#include <algorithm>
#include <set>
#include <cstdlib>
#include <vector>
#include <chrono>
#include <iostream>

#include "set.cpp"

struct BenchData{
  public:

    std::chrono::microseconds set_bin_search;
    std::chrono::microseconds bl_set_bin_search;
    std::chrono::microseconds set_eytzinger;
    std::chrono::microseconds bl_set_eytzinger;

    std::chrono::microseconds map_bin_search;
    std::chrono::microseconds map_eytzinger;

    std::chrono::microseconds packed_bin_search;
    std::chrono::microseconds packed_eytzinger;
  
    void headers() {
      using namespace std;
      cout << "size, binary set, eytzinger set, bl binary set, bl eytzinger set" << endl;
    }

    void print(unsigned long size) {
      using namespace std;
      cout << size;
      cout << "," << set_bin_search.count();
      cout << "," << set_eytzinger.count();
      cout << "," << bl_set_bin_search.count();
      cout << "," << bl_set_eytzinger.count();
      cout << endl;
    }
};

template <typename T>
void time_set(
    sets::Base<T> &s,
    std::vector<T> &f_q,
    std::vector<T> &t_q,
    std::chrono::microseconds &res
    ){
  std::chrono::steady_clock::time_point begin;
  std::chrono::steady_clock::time_point end; 

  volatile int checksum = 0;
  begin = std::chrono::steady_clock::now();
  for (auto x: f_q){
    checksum ^= s.contains(x);
  }
  for (auto x: t_q){
    checksum ^= s.contains(x);
  }
  end = std::chrono::steady_clock::now(); 
  res = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
};

void benchmark(unsigned size, BenchData &result){
  int QUERIES = 1e6;

  std::vector<int> data;
  data.reserve(size);
  std::set <int> values;
  std::vector<int> false_queries;
  std::vector<int> true_queries;
  while (values.size() < size){
    values.insert(rand());
  }
  for (auto x: values){
    data.push_back(x);
  }
  std::sort(data.begin(), data.end());
  int i = 0;
  while (i < QUERIES){
    int cur = rand();
    if (!values.contains(cur)){
      false_queries.push_back(cur);
      i++;
    }
  }
  for (int i = 0; i < QUERIES; i++){
    int idx = rand() % size;
    true_queries.push_back(data[idx]);
  } 

  sets::ESet<int> eset(data);
  time_set(eset, false_queries, true_queries, result.set_eytzinger);

  sets::Set<int> set(data);
  time_set(set, false_queries, true_queries, result.set_bin_search);

  sets::BL_ESet<int> bl_eset(data);
  time_set(bl_eset, false_queries, true_queries, result.bl_set_eytzinger);

  sets::BL_Set<int> bl_set(data);
  time_set(bl_set, false_queries, true_queries, result.bl_set_bin_search);
}

int main(){
  using namespace std;

  vector<unsigned long> benchmark_sizes;
  for (int i = 1; i <= 24; i++){
    benchmark_sizes.push_back(1 << i);
  }
  {
    unsigned long q = 1000;
    while (q <= 1000000){
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
