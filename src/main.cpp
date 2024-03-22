#include <algorithm>
#include <set>
#include <cstdlib>
#include <vector>
#include <chrono>
#include <iostream>

#include "binsearch.cpp"
#include "eytzinger.cpp"

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

    void print() {
      std::cout << "\t\tbin\teytzinger" << std::endl;
      std::cout << "b set:\t" << set_bin_search.count() << "\t" << set_eytzinger.count() << std::endl;
      std::cout << "bl set:\t" << bl_set_bin_search.count() << "\t" << bl_set_eytzinger.count() << std::endl;
      /* std::cout << "map:\t" << map_bin_search.count() << "\t" << map_eytzinger.count() << std::endl; */
      /* std::cout << "pmap:\t" << packed_bin_search.count() << "\t" << packed_eytzinger.count() << std::endl; */
      std::cout << std::endl;
    }
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
  std::chrono::steady_clock::time_point begin;
  std::chrono::steady_clock::time_point end; 

  {
    Eytzinger::ESet<int> s(data);
    begin = std::chrono::steady_clock::now();
    int checksum = 0;
    for (auto x: false_queries){
      checksum ^= s.contains(x); 
    }
    for (auto x: true_queries){
      checksum ^= s.contains(x); 
    }
    end = std::chrono::steady_clock::now(); 
    result.set_eytzinger = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);

    std::cerr << checksum << std::endl;
  }
  {
    Eytzinger::BL_ESet<int> s(data);
    begin = std::chrono::steady_clock::now();
    int checksum = 0;
    for (auto x: false_queries){
      checksum ^= s.contains(x); 
    }
    for (auto x: true_queries){
      checksum ^= s.contains(x); 
    }
    end = std::chrono::steady_clock::now(); 
    result.bl_set_eytzinger = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);

    std::cerr << checksum << std::endl;
  }
  {
    Binsearch::Set<int> s(data);
    begin = std::chrono::steady_clock::now();
    int checksum = 0;
    for (auto x: false_queries){
      checksum ^= s.contains(x); 
    }
    for (auto x: true_queries){
      checksum ^= s.contains(x); 
    }
    end = std::chrono::steady_clock::now(); 
    result.set_bin_search = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);

    std::cerr << checksum << std::endl;
  }
  {
    Binsearch::BL_Set<int> s(data);
    begin = std::chrono::steady_clock::now();
    int checksum = 0;
    for (auto x: false_queries){
      checksum ^= s.contains(x); 
    }
    for (auto x: true_queries){
      checksum ^= s.contains(x); 
    }
    end = std::chrono::steady_clock::now(); 
    result.bl_set_bin_search = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);

    std::cerr << checksum << std::endl;
  }
}

int main(){
  using namespace std;

  vector<unsigned long> benchmark_sizes = {1000, 1<<13, 10000, 1 << 16, 100000, 1 << 18, 1 << 20, 1 << 22};
  BenchData x; 

  for (auto size: benchmark_sizes){
    cout << "size: " << size << endl;
    benchmark(size, x);
    x.print();
  }
}
