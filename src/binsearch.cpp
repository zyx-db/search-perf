#ifndef BINSEARCH_H
#define BINSEARCH_H

#include <vector>
namespace Binsearch {
  template<typename T>
  class Base {
    public:
      T* data;
      unsigned size;
      Base(std::vector<T> vals){
	size = vals.size();
	data = new T[size];
	for (int i = 0; i < size; i++) data[i] = vals[i];
      };
      virtual bool contains(T comp) = 0;
  };

  template <typename T>
  class Set: public Binsearch::Base<T>{
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

  template <typename T>
  class Map {};

  template <typename T>
  class BL_Map {};
}

#endif
