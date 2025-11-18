#include <unordered_set>

template <class T> T get_vector_sum(svector<T> &vec)
{
	T total=0;
	auto ii_s=vec.begin(),ii_e=vec.end();
	for(;ii_s<ii_e;++ii_s)
		{
		total+=(*ii_s);
		}
	return total;
}

template <class T> VIndex get_random_biased_index(svector<T> &chance)
{
	if(chance.size()==0)
		{
		errorlog_string("Empty biased index vector");
		return -1;
		}

	T roll=trandom(get_vector_sum(chance));

	auto ii_s=chance.begin(),ii_e=chance.end();
	auto ii_b=ii_s;
	for(;ii_s<ii_e;++ii_s)
		{
		if(roll<(*ii_s))return (VIndex)(ii_s-ii_b);
		roll-=(*ii_s);
		}

	errorlog_string("Biased index vector computation error");
	return 0;//MORE FUNCTIONS WILL BE HAPPIER WITH 0 THAN -1 HERE
}

template <class T> void zero_vector(svector<T> &vc)
{
	std::fill(vc.begin(),vc.end(),0);
}

template <class T> bool positive_vector(svector<T> &vc)
{
	return std::any_of(vc.begin(),vc.end(),[](T v) {return v>0; });
}

template <class T> void add_unique_to_vector(T nl,svector<T> &vc)
{
	auto ii_s=vc.begin(),ii_e=vc.end();
	for(;ii_s<ii_e;++ii_s)
		{
		if((*ii_s)==nl)return;
		}
	vc.push_back(nl);
}

template <class T,class T2> void add_dual_unique_to_vectors(T nl,T2 nl2,svector<T> &vc,svector<T2> &vc2)
{
	auto ii_s=vc.begin(),ii_e=vc.end();
	auto ii_s2=vc2.begin(),ii_e2=vc2.end();
	for(;ii_s<ii_e;++ii_s,++ii_s2)
		{
		if((*ii_s)==nl&&
			(*ii_s2)==nl2)return;
		}
	vc.push_back(nl);
	vc2.push_back(nl2);
}

template <class T,class T2,class T3,class T4> void add_quad_unique_to_vectors(T nl,T2 nl2,T3 nl3,T4 nl4,
																				svector<T> &vc,svector<T2> &vc2,svector<T3> &vc3,svector<T4> &vc4)
{
	auto ii_s=vc.begin(),ii_e=vc.end();
	auto ii_s2=vc2.begin(),ii_e2=vc2.end();
	auto ii_s3=vc3.begin(),ii_e3=vc3.end();
	auto ii_s4=vc4.begin(),ii_e4=vc4.end();
	for(;ii_s<ii_e;++ii_s,++ii_s2,++ii_s3,++ii_s4)
		{
		if((*ii_s)==nl&&
			(*ii_s2)==nl2&&
			(*ii_s3)==nl3&&
			(*ii_s4)==nl4)return;
		}
	vc.push_back(nl);
	vc2.push_back(nl2);
	vc3.push_back(nl3);
	vc4.push_back(nl4);
}

template <class T> void remove_all_from_vector(T nl,svector<T> &vc)
{
	VIndex i;
	for(i=(VIndex)vc.size()-1;i>=0;i--)
		{
		if(vc[i]==nl)vc.erase(i);
		}
}

template <class T,class T2> void remove_all_from_dual_vectors(T nl,T2 nl2,svector<T> &vc,svector<T2> &vc2)
{
	VIndex i;
	for(i=(VIndex)vc.size()-1;i>=0;i--)
		{
		if(vc[i]==nl&&
			vc2[i]==nl2)
			{
			vc.erase(i);
			vc2.erase(i);
			}
		}
}

template <class T> VIndex get_vector_index(T a,svector<T> &v)
{
	auto ii_s=v.begin(),ii_e=v.end();
	auto ii_b=ii_s;
	for(;ii_s<ii_e;++ii_s)
		{
		if((*ii_s)==a)return (VIndex)(ii_s-ii_b);
		}
	return -1;
}

template <class T,class T2> VIndex get_dual_vector_index(T a1,T2 a2,svector<T> &vc,svector<T2> &vc2)
{
	auto ii_s=vc.begin(),ii_e=vc.end();
	auto ii_s2=vc2.begin(),ii_e2=vc2.end();
	auto ii_b=ii_s;
	for(;ii_s<ii_e;++ii_s,++ii_s2)
		{
		if((*ii_s)==a1&&
			(*ii_s2)==a2)return (VIndex)(ii_s-ii_b);
		}
	return -1;
}

template <class T,class T2,class T3,class T4> VIndex get_quad_vector_index(T a1,T2 a2,T3 a3,T4 a4,
																		svector<T> &vc,svector<T2> &vc2,
																		svector<T3> &vc3,svector<T4> &vc4)
{
	auto ii_s=vc.begin(),ii_e=vc.end();
	auto ii_s2=vc2.begin(),ii_e2=vc2.end();
	auto ii_s3=vc3.begin(),ii_e3=vc3.end();
	auto ii_s4=vc4.begin(),ii_e4=vc4.end();
	auto ii_b=ii_s;
	for(;ii_s<ii_e;++ii_s,++ii_s2,++ii_s3,++ii_s4)
		{
		if((*ii_s)==a1&&
			(*ii_s2)==a2&&
			(*ii_s3)==a3&&
			(*ii_s4)==a4)return (VIndex)(ii_s-ii_b);
		}
	return -1;
}

template <class T> void merge_vectors(std::vector<T> &master, std::vector<T> &merger)
{
	std::unordered_set<T> master_set;
	for (auto &c : master)
		{
		master_set.insert(c);
		}
	for(auto &thing : merger)
		{
		if(!master_set.contains(thing))
			{
			master.push_back(thing);
			master_set.insert(thing);
			}
		}
}

template <class T> VIndex get_common_element_vector_index(T &master, T &merger)
{
	auto ii_s=merger.begin(),ii_e=merger.end();
	auto ii_s2=master.begin(),ii_e2=master.end();
	auto ii_b2=ii_s2;

	for(;ii_s<ii_e;++ii_s)
		{
		for(;ii_s2<ii_e2;++ii_s2)
			{
			if((*ii_s)==(*ii_s2))return (int32_t)(ii_s2-ii_b2);
			}
		ii_s2=ii_b2;
		}

	return -1;
}

template <class T,class T2> void merge_dual_vectors(T &master, T2 &master2, T &merger, T2 &merger2)
{
	auto ii_s=merger.begin(),ii_e=merger.end();
	auto ii_s2=merger2.begin(),ii_e2=merger2.end();
	for(;ii_s<ii_e;++ii_s,++ii_s2)
		{
		auto ii_s3=master.begin(),ii_e3=master.end();
		auto ii_s4=master2.begin(),ii_e4=master2.end();
		for(;ii_s3<ii_e3;++ii_s3,++ii_s4)
			{
			if((*ii_s)==(*ii_s3)&&
				(*ii_s2)==(*ii_s4))break;
			}
		if(ii_s3>=ii_e3)
			{
			master.push_back((*ii_s));
			master2.push_back((*ii_s2));
			}
		}
}

template <class T,class T2,class T3,class T4> void merge_quad_vectors(T &master, T2 &master2, T3 &master3, T4 &master4,
																		T &merger, T2 &merger2, T3 &merger3, T4 &merger4)
{
	auto ii_s=merger.begin(),ii_e=merger.end();
	auto ii_s2=merger2.begin(),ii_e2=merger2.end();
	auto ii_s3=merger3.begin(),ii_e3=merger3.end();
	auto ii_s4=merger4.begin(),ii_e4=merger4.end();
	for(;ii_s<ii_e;++ii_s,++ii_s2,++ii_s3,++ii_s4)
		{
		auto ii_s5=master.begin(),ii_e5=master.end();
		auto ii_s6=master2.begin(),ii_e6=master2.end();
		auto ii_s7=master3.begin(),ii_e7=master3.end();
		auto ii_s8=master4.begin(),ii_e8=master4.end();
		for(;ii_s5<ii_e5;++ii_s5,++ii_s6,++ii_s7,++ii_s8)
			{
			if((*ii_s)==(*ii_s5)&&
				(*ii_s2)==(*ii_s6)&&
				(*ii_s3)==(*ii_s7)&&
				(*ii_s4)==(*ii_s8))break;
			}
		if(ii_s5>=ii_e5)
			{
			master.push_back((*ii_s));
			master2.push_back((*ii_s2));
			master3.push_back((*ii_s3));
			master4.push_back((*ii_s4));
			}
		}
}

template <class T> void cull_vectors(std::vector<T> &master,std::vector<T> &cull)
{
	std::unordered_set<T> cull_set;
	for(auto &c : cull)
		{
		cull_set.insert(c);
		}
	std::erase_if(master,[&cull_set](const T &m) { return cull_set.contains(m); });
}

template <class T> void push_on_vector(T &master,T &new_stuff) {
	std::copy(new_stuff.begin(),new_stuff.end(),std::back_inserter(master));
	}

constexpr inline VIndex LINEAR_SEARCH_THRESHOLD=64;

template<class Container> std::pair<VIndex,VIndex> narrow_to_range(const Container &span, typename Container::value_type id) {
	VIndex size=(VIndex)span.size();
	if (size==0) return std::make_pair(-1,-1);
	VIndex start=0;
	while (size>LINEAR_SEARCH_THRESHOLD)
		{
		const auto half=size/2;
		const auto mid=start+half;
		start=id<span[mid]?start:mid;
		size-=half;
		}
	return std::make_pair(start,start+size);
	}

template<class Container,typename F> std::pair<VIndex,VIndex> narrow_to_range(const Container &span, int64_t id, F trans_func) {
	VIndex size=(VIndex)span.size();
	if (size==0) return std::make_pair(-1,-1);
	VIndex start=0;
	while (size>LINEAR_SEARCH_THRESHOLD)
		{
		const auto half=size/2;
		const auto mid=start+half;
		start=id<trans_func(span[mid])?start:mid;
		size-=half;
		}
	return std::make_pair(start,start+size);
	}

template<class Container> std::optional<std::pair<VIndex,typename Container::value_type>> strategy_search(const Container &span,typename Container::value_type id) {
	const auto [start,end]=narrow_to_range(span,id);
	if (end==-1) return std::nullopt;
	for (VIndex i=start; i<end; ++i)
		{
		if (id==span[i])
			{
			return std::make_pair(i,span[i]);
			}
		}
	return std::nullopt;
	}


template<class Container,typename F> std::optional<std::pair<VIndex,typename Container::value_type>> strategy_search(const Container &span,int64_t id,F trans_func) {
	const auto [start,end]=narrow_to_range(span,id,trans_func);
	if (end==-1) return std::nullopt;
	for (VIndex i=start; i<end; ++i)
		{
		if (id==trans_func(span[i]))
			{
			return std::make_pair(i,span[i]);
			}
		}
	return std::nullopt;
	}

template<class Container> VIndex add_to_global_id_vector(const typename Container::value_type ptr,Container &vect)
{
	using T=typename Container::value_type;
	const auto it=std::lower_bound(vect.begin(),vect.end(),ptr,[](const T &a,const T &b) { return a->global_id<b->global_id; });
	if (it==vect.end() || (*it)->global_id != ptr->global_id)
		{
		vect.insert(it,std::move(ptr));
		return std::distance(vect.begin(),it);
		}
	return -1;
}

template<class Container> void remove_from_global_id_vector_by_id(const int32_t id,Container &vect)
{
	using T=typename Container::value_type;
	const auto found=strategy_search(vect,id,[](const T &thing) { return thing->global_id;});
	if (found)
		{
		vect.erase(vect.begin()+found.value().first);
		}
}

template<class Container> void remove_from_global_id_vector(const typename Container::value_type ptr,Container &vect) {
	remove_from_global_id_vector_by_id(ptr->global_id,vect);
	}

template<class Container> Container::value_type get_from_global_id_vector(const int32_t id,const Container &vect)
{
	using T=typename Container::value_type;
	const auto found=strategy_search(vect,id,[](const T &thing) { return thing->global_id; });
	return found.value_or(std::make_pair(-1,static_cast<T>(NULL))).second;
}

template<class Container> VIndex get_index_from_global_id_vector(const int32_t id,const Container &vect)
{
	using T=typename Container::value_type;
	const auto found=strategy_search(vect,id,[](const T &thing) { return thing->global_id; });
	return found.value_or(std::make_pair(-1,static_cast<T>(NULL))).first;
}

template<class Container> VIndex add_possible_duplicate_to_binary_vector(const typename Container::value_type ptr,Container &vect)
{
	const auto it=std::lower_bound(vect.begin(),vect.end(),ptr);
	vect.insert(it,ptr);
	return std::distance(vect.begin(),it);
}

//NOTE: RETURNS -1 IF ALREADY PRESENT, NOT THE INDEX
template<class Container> VIndex add_unique_to_binary_vector(const typename Container::value_type ptr,Container &vect)
{
	const auto it=std::lower_bound(vect.begin(),vect.end(),ptr);
	if (it>=vect.begin() && it==vect.end() || *it!=ptr)
		{
		const auto inserted_it=vect.insert(it,ptr);
		return std::distance(vect.begin(),inserted_it);
		}
	return -1; // ALREADY IN VECTOR
}

//NOTE: RETURNS INDEX IF ALREADY PRESENT
template<class Container> VIndex add_unique_to_binary_vector_always_index(const typename Container::value_type ptr,Container &vect,bool &was_present)
{
	was_present=false;

	const auto it=std::lower_bound(vect.begin(),vect.end(),ptr);
	const auto ret=std::distance(vect.begin(),it);
	if (it>=vect.begin() && it==vect.end() || *it!=ptr)
		{
		vect.insert(it,ptr);
		}
	else
		{
		was_present=true;
		}
	return ret;
}

template<class Container> void remove_from_binary_vector(const typename Container::value_type ptr,Container &vect)
{
	using T=typename Container::value_type;
	const auto found=strategy_search(vect,ptr);
	if (found)
		{
		vect.erase(vect.begin()+found.value().first);
		return;
		}
}

template<class Container> VIndex get_index_from_binary_vector(const typename Container::value_type id,const Container &vect)
{
	using T=typename Container::value_type;
	const auto found=strategy_search(vect,id);
	return found.value_or(std::make_pair(-1,static_cast<T>(NULL))).first;
}

template<class Container> VIndex get_floor_index_from_binary_vector(const typename Container::value_type ptr,const Container &vect)
{
	return std::distance(vect.begin(),std::lower_bound(vect.begin(),vect.end(),ptr));
}

template<class Container> void add_to_local_id_vector(const typename Container::value_type ptr,Container &vect)
{
	using T=typename Container::value_type;
	const auto it=std::lower_bound(vect.begin(),vect.end(),ptr,[](const T &a,const T &b) { return a->local_id<b->local_id; });
	if (it==vect.end() || (*it)->local_id!=ptr->local_id)
		{
		vect.insert(it,std::move(ptr));
		}
}

template<class Container> void remove_from_local_id_vector(const typename Container::value_type ptr,Container &vect)
{
	using T=typename Container::value_type;
	const auto id=ptr->local_id;
	const auto found=strategy_search(vect,id,[](const T &thing) { return thing->local_id; });
	if (found) vect.erase(vect.begin()+found.value().first);
}

template<class Container> typename Container::value_type get_from_local_id_vector(int32_t id,const Container &vect)
{
	using T=typename Container::value_type;
	const auto found=strategy_search(vect,id,[](const T &thing) { return thing->local_id; });
	return found.value_or(std::make_pair(-1,static_cast<T>(NULL))).second;
}

template<class Container> VIndex get_index_from_local_id_vector(int32_t id,const Container &vect)
{
	using T=typename Container::value_type;
	const auto found=strategy_search(vect,id,[](const T &thing) { return thing->local_id; });
	return found.value_or(std::make_pair(-1,static_cast<T>(NULL))).first;
}

template<class Container> void add_to_short_id_vector(typename Container::value_type ptr,Container &vect)
{
	using T=typename Container::value_type;
	const auto it=std::lower_bound(vect.begin(),vect.end(),ptr,[](const T &a,const T &b) { return a->short_id<b->short_id; });
	if (it==vect.end() || (*it)->short_id != ptr->short_id)
		{
		vect.insert(it,std::move(ptr));
		}
}

template<class Container> void remove_from_short_id_vector(const typename Container::value_type ptr,Container &vect)
{
	using T=typename Container::value_type;
	const auto id=ptr->short_id;
	const auto found=strategy_search(vect,id,[](const T &thing) { return thing->short_id; });
	if (found) vect.erase(found.value().first);
}

template<class Container> typename Container::value_type get_from_short_id_vector(int16_t id,const Container &vect)
{
	using T=typename Container::value_type;
	const auto found=strategy_search(vect,id,[](const T &thing) { return thing->short_id; });
	return found.value_or(std::make_pair(-1,static_cast<T>(NULL))).second;
}

template<class Container> int16_t get_index_from_short_id_vector(int16_t id,const Container &vect)
{
	using T=typename Container::value_type;
	const auto found=strategy_search(vect,id,[](const T &thing) { return thing->short_id; });
	return found.value_or(std::make_pair(-1,static_cast<T>(NULL))).first;
}

template<class Container> void add_to_local_id_64_vector(const typename Container::value_type ptr,Container &vect) {
	add_to_local_id_vector(ptr,vect); // templates! good to use
	}

template<class Container> void remove_from_local_id_64_vector(const typename Container::value_type ptr,Container &vect) {
	remove_from_local_id_vector(ptr,vect);
	}

template<class Container> typename Container::value_type get_from_local_id_64_vector(int64_t id,const Container &vect) {
	using T=typename Container::value_type;
	const auto found=strategy_search(vect,id,[](const T &thing) { return thing->local_id; });
	return found.value_or(std::make_pair(-1,static_cast<T>(NULL))).second;
	}

template<class Container> int64_t get_index_from_local_id_64_vector(int64_t id,const Container &vect) {
	using T=typename Container::value_type;
	const auto found=strategy_search(vect,id,[](const T &thing) { return thing->local_id; });
	return found.value_or(std::make_pair(-1,static_cast<T>(NULL))).first;
	}

template<class T> std::optional<T> wrap_optional(T input,T null_val) {
	return input==null_val ? std::nullopt : std::make_optional(input);
	}

// No unwrap_optional because C++ has std::optional<T>::value_or

template<class Container> std::optional<typename Container::value_type> maybe_get(const typename Container::key_type &key,const Container &map) {
	const auto found=map.find(key);
	if (found!=map.end()) return found->second;
	return std::nullopt;
	}

template<class Container> std::optional<typename Container::value_type> maybe_get(typename Container::key_type &&key,const Container &map) {
	const auto found=map.find(key);
	if (found!=map.end()) return found->second;
	return std::nullopt;
	}

// Above are mostly useful for e.g. some maybe_get(map,key).and_then([](auto &val){...}); or maybe_get(map,key).value_or(default_val); or whatever, lots of good utility from std::optional

template<typename T>
T *flatten(T **ptr_ptr) {
	if constexpr (std::is_pointer<T>::value)
		{
		return ptr_ptr?flatten(*ptr_ptr):nullptr;
		}
	else
		{
		return *ptr_ptr;
		}
	}

template<typename T,typename... Types>
std::optional<T> better_get_if(std::variant<Types...> &v) {
	if (auto p=std::get_if<T>(&v);p)
		{
		return *p;
		}
	return std::nullopt;
	}