#ifndef BRANCHLESS_H
#define BRANCHLESS_H

#include <vector>
#include <stdlib.h>
#include <iostream>
namespace Branchless {
  template<typename T>
  class ESet{
    private:
      T *data;
      unsigned size;
      void eytzinger(unsigned k, std::vector<T> &vals, unsigned &init){
	if (k <= size){
	  eytzinger(2 * k, vals, init);
	  data[k] = vals[init++];	
	  data[k] = vals[init++];	
	  eytzinger(2 * k + 1, vals, init);
	}
      };

    public:
      ESet(std::vector<T> vals){
	size = vals.size();
	unsigned init = 0;
	data = (T*) std::aligned_alloc(32, sizeof(T) * (size + 1));
	if (data == nullptr){
	  data = new T[size + 1];
	  std::cerr << "failed aligned_alloc" << std::endl;
	}
	data[0] = NULL;
	eytzinger(1, vals, init);
      };
      bool contains(T key){
	unsigned idx = 1;
	while (idx < size){
	  if (data[idx] == key){ return true; }
	  if (data[idx] > key){
	    idx = 2 * idx + 1; 
	  }
	  else {
	    idx = 2 * idx; 
	  }
	}
	return false;
      }
  };

  template<typename T>
  class EMap{
    
  };

  template<typename T>
  class EMapDense{
    
  };
}

#endif
