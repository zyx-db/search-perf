#ifndef EYTZINGER_H
#define EYTZINGER_H

#include <vector>
#include <iostream>
namespace Eytzinger {
  template<typename T>
  class BaseSet{
    public:
      T* data;
      unsigned size;
      void eytzinger(unsigned k, std::vector<T> &vals, unsigned &init){
	if (k <= size){
	  eytzinger(2 * k, vals, init);
	  data[k] = vals[init++];	
	  eytzinger(2 * k + 1, vals, init);
	}
      };
      BaseSet() : data(nullptr), size(0) {}
      BaseSet(std::vector<T> vals){
	size = vals.size();
	unsigned init = 0;
	data = new T[size + 1];
	eytzinger(1, vals, init);
      };
      virtual bool contains(T key) = 0;
  };

  template<typename T>
  class ESet: public Eytzinger::BaseSet<T>{
    public:
      using BaseSet<T>::BaseSet;
      bool contains(T key) override{
	unsigned idx = 1;
	while (idx < this -> size){
	  if (this -> data[idx] == key){ return true; }
	  if (this -> data[idx] > key){
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
    class BL_ESet: public BaseSet<T>{
      public:
	BL_ESet(std::vector<T> vals): BaseSet<T>{}{
	  this -> size = vals.size();
	  unsigned init = 0;
	  this -> data = (T*) aligned_alloc(32, sizeof(T) * (this -> size + 1));
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
	    if (this -> data[idx] == key){ return true; }
	    if (this -> data[idx] > key){
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
