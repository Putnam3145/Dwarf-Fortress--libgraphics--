//pelican aka sam dennis wrote this
#ifndef SVECTOR_H
#define SVECTOR_H

/**
 * svector is essentially a clone of std::vector,
 * forwarding the allocator template parameter
 * (note: identical default to std::vector),
 * but implementing an insert-by-index instead of
 * iterator method for convenience.
 * 
 * this templated container appears to be used primarily for
 * c-strings, but has a number of helper-method implemented
 * to operate on index-types.
*/

#include <vector>
#include <memory>

template <class T, class A = std::allocator<T> >
class svector : public std::vector<T, A> {
public:
        using std::vector<T,A>::erase;
        using std::vector<T,A>::begin;
        using std::vector<T,A>::insert;

        void erase(typename std::vector<T, A>::size_type i) {
                erase(begin() + i);
        }
        void insert(typename std::vector<T, A>::size_type i, const T &v) {
                insert(begin() + i, v);
        }
        void insert(typename std::vector<T,A>::size_type i, T &&v){
                insert(begin() + i, std::move(v));
        }

};

#endif