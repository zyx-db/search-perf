#ifndef BINSEARCH_H
#define BINSEARCH_H

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <vector>

namespace sets {
  template<typename T>
  class Base {
    public:
      T* data;
      unsigned size;
      Base() : data(nullptr), size(0) {}
      Base(std::vector<T> vals){
	size = vals.size();
	data = new T[size];
	for (int i = 0; i < size; i++) data[i] = vals[i];
      };
      virtual bool contains(T comp) = 0;
  };

  template <typename T>
  class Set: public sets::Base<T>{
    public:
      using Base<T>::Base;
      bool contains(T comp) override{
	T* base = this -> data;
	unsigned len = this -> size;
	while (len > 1) {
	  unsigned half = len / 2;
	  if (base[half - 1] < comp){
	    base += half;
	    len = len - half;
	  }
	  else {
	    len = half;
	  }
	}
	return *base == comp;
      }
  };

  template <typename T>
  class BL_Set: public Base<T> {
    public:
      using Base<T>::Base;
      bool contains(T comp) override{
	T* base = this -> data;
	unsigned len = this -> size;
	while (len > 1) {
	  unsigned half = len / 2;
	  base += (base[half-1] < comp) * half; // cmov instruction
	  len -= half;
	}
	return *base == comp;
      }
  };

  template<typename T>
  class BaseE: public sets::Base<T>{
    public:
      void eytzinger(unsigned k, std::vector<T> &vals, unsigned &init){
	if (k <= this -> size){
	  eytzinger(2 * k, vals, init);
	  this -> data[k] = vals[init++];	
	  eytzinger(2 * k + 1, vals, init);
	}
      };
      BaseE(): Base<T>{}{};
      BaseE(std::vector<T> vals): Base<T>{}{
	this -> size = vals.size();
	unsigned init = 0;
	this -> data = new T[this -> size + 1];
	this -> eytzinger(1, vals, init);
      };
  };

  template<typename T>
  class ESet: public sets::BaseE<T>{
    public:
      using BaseE<T>::BaseE;
      bool contains(T key) override{
	unsigned idx = 1;
	while (idx < this -> size){
	  if (this -> data[idx] > key){
	    idx = 2 * idx + 1; 
	  }
	  else {
	    idx = 2 * idx; 
	  }
	}
	idx >>= __builtin_ffs(~idx);
	return this -> data[idx] == key;
      }
  };

  template<typename T>
    class BL_ESet: public BaseE<T>{
      public:
	BL_ESet(std::vector<T> vals): BaseE<T>{}{
	  this -> size = vals.size();
	  unsigned init = 0;
	  int alloc_size = sizeof(T) * ((this -> size) + 1);
	  unsigned align_size = 64;
	  if (alloc_size % align_size != 0){alloc_size += (align_size - (alloc_size % align_size));}
	  this -> data = (T*) aligned_alloc(align_size, alloc_size);
	  if (this -> data == nullptr){
	    this -> data = new T[this -> size + 1];
	    std::cerr << "failed aligned_alloc" << std::endl;
	  }
	  this -> data[0] = NULL;
	  this -> eytzinger(1, vals, init);
	};

	bool contains(T key){
	  unsigned idx = 1;
	  while (idx < this -> size){
	    __builtin_prefetch(this -> data + idx * 16);
	    idx = 2 * idx + (this -> data[idx] < key);
	  }
	  idx >>= __builtin_ffs(~idx);
	  return this -> data[idx] == key;
      }
    };
}

#endif
