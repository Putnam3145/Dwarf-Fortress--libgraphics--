#ifndef DF_GSRC_UTIL_BIMAP_HPP
#define DF_GSRC_UTIL_BIMAP_HPP

#include <map>

template<typename A, typename B>
struct bimap {

  std::map<A,B> left;
  std::map<B,A> right;

  bimap() {};
  bimap(std::initializer_list<std::pair<const A, B>> list):left(list){
    for (auto& p : left){
      right[p.second] = p.first;
    }
  }

  void insert(A a, B b) {
    left.insert(std::pair<A,B>(a,b));
    right.insert(std::pair<B,A>(b,a));
  }

};

#endif // DF_GSRC_UTIL_BIMAP_HPP