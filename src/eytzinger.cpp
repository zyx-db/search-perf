#ifndef EYTZINGER_H
#define EYTZINGER_H

#include <vector>
#include <iostream>
namespace Eytzinger {
  template<typename T>
  class ESet{
    private:
      T *data;
      unsigned size;
      void eytzinger(unsigned k, std::vector<T> &vals, unsigned &init){
	if (k <= size){
	  eytzinger(2 * k, vals, init);
	  data[k] = vals[init++];	
	  eytzinger(2 * k + 1, vals, init);
	}
      };

    public:
      ESet(std::vector<T> vals){
	size = vals.size();
	unsigned init = 0;
	data = new T[size + 1];
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
