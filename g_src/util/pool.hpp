#pragma once

#include <array>
#include <vector>
#include <memory>
#include <set>
#include <unordered_set>

template<class T, size_t N>
class object_pool
{
  using pool_array=std::array<T,N>;
  std::vector<std::unique_ptr<pool_array>> pool;
  std::set<size_t> unused_slots;
  size_t get_next_slot()
  {
    if (unused_slots.empty())
    {
      pool.push_back(std::unique_ptr<pool_array>(new pool_array()));
      const auto end=pool.size()*N;
      auto it=unused_slots.begin();
      for (auto i=end-N; i<end; i++)
      {
        it = unused_slots.emplace_hint(it, i);
      }
    }
    auto smallest=unused_slots.begin();
    size_t slot=*smallest;
    unused_slots.erase(smallest);
    return slot;
  }

public:
  T *get(size_t n)
  {
    const size_t v=n/N;
    const size_t a=n%N;
    return &(pool[v]->data()[a]);
  }
  template<class... Args>
  size_t make_new(Args&&... args)
  {
    auto next_slot=get_next_slot();
    void *b=get(next_slot);
    new (b) T(args...);
    return next_slot;
  }
  void clear()
  {
    for (auto i=0; i<pool.size()*N; i++)
    {
      erase(i);
    }
  }
  size_t push(T obj)
  {
    auto next_slot=get_next_slot();
    *get(next_slot)=obj;
    return next_slot;
  }
  void erase(size_t idx)
  {
    if (!unused_slots.contains(idx))
    {
      unused_slots.insert(idx);
      get(idx)->T::~T();
    }
  }
  void remove(T *ptr)
  {
    // hell yeah we're doing a linear search
    for (int i=0; i<pool.size()*N; i++)
    {
      if (get(i)==ptr)
      {
        erase(i);
        return;
      }
    }
  }
  ~object_pool()
  {
    pool.clear();
    unused_slots.clear();
  }
};

template<class T,size_t N>
void garbage_collect(object_pool<T,N> &pool,std::vector<T *> &glob)
{
  std::set<size_t> still_extant;
  for (auto &obj:glob)
  {
    still_extant.insert(obj->get_pool_id());
  }
  if (still_extant.empty()) return;
  const auto m=*still_extant.rbegin();
  for (auto i=0; i<=m; i++)
  {
    if (!still_extant.contains(i))
      pool.erase(i);
  }
}