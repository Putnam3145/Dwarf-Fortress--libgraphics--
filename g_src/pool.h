#pragma once

#include <span>
#include <vector>
#include <memory>
#include <map>
#include <unordered_set>
#include <algorithm>

template<class T, size_t N>
class object_pool
{
	using pool_array=std::span<T,std::dynamic_extent>;
	std::vector<pool_array> pool;
	std::map<size_t,bool> unused_slots; // False is uninit, true is just unused
	std::vector<T*> *glob;
	std::pair<size_t,bool> get_next_slot()
	{
		if (no_empty_slots())
			{
			if (glob)
				{
				garbage_collect(*this,*glob);
				}
			if (no_empty_slots())
				{
				const auto current_capacity=capacity();
				add_capacity(current_capacity>N?current_capacity:N);
				}
			}
		auto smallest=unused_slots.begin();
		std::pair<size_t, bool> slot=*smallest;
		unused_slots.erase(smallest->first);
		return slot;
	}

public:
	T *get(size_t n)
		{
		for (auto &p : pool)
			{
			if (n<p.size())
				{
				return &(p[n]);
				}
			n-=p.size();
			}
		return NULL; // really should not happen, but necessary
		}
	template<class... Args>
	size_t make_new(Args&&... args)
		{
		auto next_slot=get_next_slot();
		T *b=get(next_slot.first);
		if (next_slot.second)
			{
			b->T::~T();
			}
		new (b) T(args...);
		return next_slot.first;
		}
	void clear()
		{
		for(auto i=0; i<capacity(); i++)
			{
			if (const auto &p=unused_slots.find(i); p!=unused_slots.end())
				{
				if (p->second)
					{
					get(i)->T::~T();
					}
				}
			else
				{
				get(i)->T::~T();
				}
			}
		unused_slots.clear();
		for (auto &p : pool)
			{
			free(p.data());
			}
		pool.clear();
		}
	void erase(size_t idx)
		{
		if (!unused_slots.contains(idx))
			{
			unused_slots[idx]=true;
			}
		}
	void remove(T *ptr)
		{
		// hell yeah we're doing a linear search
		for (int i=0; i<pool.capacity(); i++)
			{
			if (get(i)==ptr)
				{
				erase(i);
				return;
				}
			}
		}
	bool no_empty_slots()
		{
		return unused_slots.empty();
		}
	size_t capacity() const {
		size_t total=0;
		for (auto &p : pool)
			{
			total+=p.size();
			}
		return total;
		}
	void add_capacity(size_t n) {
		T *arr=static_cast<T*>(calloc(n,sizeof(T)));
		pool.emplace_back(arr,n);
		auto it=unused_slots.begin();
		const auto end=capacity();
		for (auto i=end-n; i<end; i++)
			{
			it=unused_slots.emplace_hint(it,std::pair(i,false));
			}
		}
	void reserve(size_t n) {
		if (n>capacity())
			{
			const size_t min_cap_add=capacity()*2;
			if (n<min_cap_add)
				{
				n=min_cap_add;
				}
			add_capacity(n-capacity());
			}
		}
	object_pool(std::vector<T *> *g=nullptr) : glob(g) {}
	~object_pool()
		{
		/*
		* If you make one of these without a static lifetime at any point
		* it will cause a leak, so don't do that please
		*/
		}
};

template<class T,size_t N>
void garbage_collect(object_pool<T,N> &pool,std::vector<T *> &glob)
{
		std::unordered_set<size_t> still_extant;
		for (auto &obj:glob)
			{
			still_extant.insert(obj->get_pool_id());
			}
		for (auto i=0; i<pool.capacity(); i++)
			{
			if (!still_extant.contains(i))
				pool.erase(i);
			}
}