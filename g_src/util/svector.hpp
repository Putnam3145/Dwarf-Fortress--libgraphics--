#ifndef DF_GSRC_UTIL_SVECTOR_HPP
#define DF_GSRC_UTIL_SVECTOR_HPP

#include <vector>
#include <memory>

//pelican aka sam dennis wrote this

template <class T, class A = std::allocator<T> >
class svector : public std::vector<T, A> {
public:
  
  using std::vector<T, A>::erase;
  using std::vector<T,A>::begin;
  using std::vector<T,A>::insert;

  void erase(typename std::vector<T, A>::size_type i) {
    erase(begin() + i);
  }
  void insert(typename std::vector<T, A>::size_type i, const T &v) {
    insert(begin() + i, v);
  }
  void insert(typename std::vector<T,A>::size_type i,T &&v){
    insert(begin()+i,std::move(v));
  }

};

#endif
