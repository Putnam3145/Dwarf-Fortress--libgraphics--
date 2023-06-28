#ifndef BIMAP_H
#define BIMAP_H

#include <map>

template<typename A, typename B>
struct bimap {
  std::map<A,B> left;
  std::map<B,A> right;

  void insert(A a, B b) {
    left.insert(std::pair<A,B>(a,b));
    right.insert(std::pair<B,A>(b,a));
  }
  bimap() {};
  bimap(std::initializer_list < std::pair<const A, B> > l) : left(l)
  {
      for (auto& p : left)
      {
          right[p.second] = p.first;
      }
  }
};

#endif
