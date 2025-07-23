#ifndef BIMAP_H
#define BIMAP_H

#include <map>
#include <unordered_map>
#include <optional>

template<typename A, typename B, typename map_a=std::map<A,B>, typename map_b=std::map<B,A>>
struct bimap {
  map_a left;
  map_b right;

  void insert(A a, B b) {
    left.insert(std::pair<A,B>(a,b));
    right.insert(std::pair<B,A>(b,a));
  }
  void insert(std::pair<A,B> p) {
      left.insert(p);
      right.insert(std::make_pair(p.second,p.first));
      }
  void insert(std::pair<B,A> p) {
      left.insert(std::make_pair(p.second,p.first));
      right.insert(p);
      }
  bimap() {};
  bimap(std::initializer_list < std::pair<const A, B> > l)
  {
      for (auto &p : l)
          {
          insert(p);
          }
  }
  bimap(std::initializer_list < std::pair<const B,A> > r)
  {
      for (auto &p : r)
          {
          insert(p);
          }
  }
  std::optional<A> maybe_get(const B &k) const {
      if (auto p=right.find(k); p!=right.end())
          {
          return p->second;
          }
      else
          {
          return std::nullopt;
          }
      }
  std::optional<B> maybe_get(const A &k) const {
      if (auto p=left.find(k); p!=left.end())
          {
          return p->second;
          }
      else
          {
          return std::nullopt;
          }
      }
    };

// New stuff introduced earlier will error if A and B are the same type, thus this template specialization
template<typename T>
struct bimap<T,T> {
    std::map<T,T> left;
    std::map<T,T> right;

    void insert(T a,T b) {
        left.insert(std::pair<T,T>(a,b));
        right.insert(std::pair<T,T>(b,a));
        }
    void insert(std::pair<T,T> p) {
        left.insert(p);
        right.insert(p.second,p.first);
        }
    bimap() {};
    bimap(std::initializer_list < std::pair<const T,T> > l) : left(l) {
        for (auto &p : left)
            {
            right[p.second]=p.first;
            }
        }
    };

template<typename A,typename B>
using unordered_bimap=bimap<A,B,std::unordered_map<A,B>,std::unordered_map<B,A>>;

#endif
