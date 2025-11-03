#pragma once

#include <variant>

// std::inplace_vector is coming C++26, but that's C++26; this isn't actually used in many places, so we'll just fix it when we get there
template<typename T,size_t S>
struct array_vector {
    static constexpr size_t capacity=S;
    using iterator=typename std::array<T,capacity>::iterator;
    using value_type=T;
    using size_type=size_t;
    std::array<T,capacity> underlying;
    size_t cur_size;
    bool try_push_back(const T &p) {
        if (cur_size<capacity)
            {
            underlying[cur_size]=p;
            cur_size+=1;
            }
        return !full();
        }
    bool try_push_back(T &&p) {
        if (cur_size<capacity)
            {
            underlying[cur_size]=p;
            cur_size+=1;
            }
        return !full();
        }
    T pop_back() {
        cur_size-=1;
        return underlying[cur_size+1]; // Don't really need to deallocate it or anything, this is fine
        }
    T *data() {
        return underlying.data();
        }
    T &operator[](size_t idx) {
        return underlying[idx];
		}
    auto begin() {
        return underlying.begin();
        }
    auto end() {
        return underlying.begin()+cur_size;
        }
    auto begin() const {
        return underlying.cbegin();
        }
	auto end() const {
        return underlying.cbegin()+cur_size;
		}
    auto cbegin() const {
        return underlying.cbegin();
		}
    auto cend() const {
        return underlying.cbegin()+cur_size;
        }
    bool empty() {
        return cur_size==0;
		}
    bool full() {
        return cur_size>=capacity;
        }
    size_t size() {
        return cur_size;
        }
    iterator insert(iterator it,const T &p) {
        if (!full())
			{
            std::move_backward(it,end()-1,end());
            *it=p;
            cur_size+=1;
            return it;
            }
		return begin()-1; // Invalid
		}
    iterator insert(iterator it,T &&p) {
        if (!full())
            {
			std::move_backward(it,end()-1,end());
            *it=std::move(p);
            cur_size+=1;
            }
		return begin()-1; // Invalid
        }
	array_vector() : cur_size(0) {}
    array_vector(T init_value) : cur_size(0) {
        underlying.fill(init_value);
        }
    array_vector(std::initializer_list<T> &&list) {
        if (list.size() > capacity) throw "Initializer list too large for array_vector!";
        for (auto &p : list)
            {
            try_push_back(p);
            }
        }
    array_vector(std::initializer_list<T> &&list,std::vector<T> &overflow) {
        for (auto &p : list)
            {
            if (!try_push_back(p))
                {
                overflow.push_back(p);
                }
            }
        }
    };

template<typename T,size_t S>
struct stack_vector {
    static constexpr size_t capacity=S;
    using arr=array_vector<T,capacity>;
    using vec=std::vector<T>;
    std::variant<array_vector<T,capacity>,std::vector<T>> underlying;
    bool is_in_heap() {
        return std::holds_alternative<vec>(underlying);
        }
    void move_to_heap() {
        if (is_in_heap()) return;
        vec heap_space;
        arr &old=std::get<arr>(underlying);
        heap_space.resize(old.cur_size);
        std::copy(old.begin(),old.end(),heap_space.begin());
        underlying=std::move(heap_space);
        }
    void move_to_stack() {
        if (!is_in_heap()) return;
        vec &old=std::get<vec>(underlying);
        if (old.size() > capacity) return;
        arr stack_space;
        stack_space.cur_size=old.size();
        std::copy(old.begin(),old.end(),stack_space.begin());
        underlying=std::move(stack_space);
		}
    void push_back(const T &p) {
        if (is_in_heap())
            {
            std::get<vec>(underlying).push_back(p);
            }
        else
            {
            if (!std::get<arr>(underlying).try_push_back(p))
                {
                move_to_heap();
                push_back(p);
                }
            }
        }
    void push_back(T &&p) {
        if (is_in_heap())
            {
            std::get<vec>(underlying).push_back(p);
            }
        else
            {
            if (!std::get<arr>(underlying).try_push_back(p))
                {
                move_to_heap();
                push_back(p);
                }
            }
        }
    T pop_back() {
        if (is_in_heap())
            {
            auto &v=std::get<vec>(underlying);
            T ret=std::move(v.back());
            v.pop_back();
            if(v.size() <= capacity)
                {
                move_to_stack();
				}
            return ret;
            }
        else
            {
            return std::get<arr>(underlying).pop_back();
			}
        }
    size_t size() {
        if (is_in_heap())
            {
            return std::get<vec>(underlying).size();
            }
        else
            {
            return std::get<arr>(underlying).cur_size;
            }
        }
    void resize(size_t new_size) {
        if (!is_in_heap())
            {
            if (new_size > capacity)
                {
                move_to_heap();
                }
            else
                {
                std::get<arr>(underlying).cur_size=new_size;
                }
            }
        if(is_in_heap()) // might change since last check
            {
            std::get<vec>(underlying).resize(new_size);
            if(new_size <= capacity)
                {
                move_to_stack();
                }
            }
        }
    void shrink_to_fit() {
		if (is_in_heap()) // If we're not in heap, we're already as small as we can be
            {
            if (std::get<vec>(underlying).size() <= capacity)
                {
                move_to_stack();
                }
            else
                {
                std::get<vec>(underlying).shrink_to_fit();
                }
            }
        }
    void reserve(size_t new_cap) {
        if (new_cap > capacity && !is_in_heap())
            {
            move_to_heap();
            }
        if (is_in_heap())
            {
            std::get<vec>(underlying).reserve(new_cap);
            }
		}
    bool empty() {
        return size()==0;
        }
    void clear() {
        underlying=arr{};
        }
    T &operator[](size_t idx) {
        if (is_in_heap())
            {
            return std::get<vec>(underlying)[idx];
            }
        else
            {
            return std::get<arr>(underlying)[idx];
            }
		}
    stack_vector(const stack_vector &other) {
        if (other.is_in_heap())
            {
            underlying=std::get<vec>(other.underlying);
            }
        else
            {
            underlying=std::get<arr>(other.underlying);
            }
		}
	stack_vector() : underlying(arr{}) {}
    stack_vector(std::initializer_list<T> &&list) {
        if (list.size() > capacity)
            {
            auto v=vec{};
            v.resize(list.size());
			std::copy(list.begin(),list.end(),v.begin());
			underlying=std::move(v);
            }
        else
            {
			underlying=arr(list);
            }
        }
    };