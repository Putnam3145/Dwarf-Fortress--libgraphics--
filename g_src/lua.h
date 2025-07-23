#pragma once

#include "lua/src/lua.hpp"

#include "svector.h"

#include "files.h"

#include <string>

#include <functional>

#include <algorithm>

void add_lua_df_globals(lua_State *L);

void lualog_string(std::string str,int err,lua_State *L);

void lualog_string(std::string str);

void lua_print_whole_stack(std::string str,lua_State *L);

int lua_logged_pcall(std::string str,lua_State *L,int num_args,int num_return,int f);

int lua_run_file(lua_State *L,filest f);

int lua_load_directory(lua_State *L,filest f);

class stringvectst;

void lua_string_table_to_stringvect(lua_State *L,stringvectst &vect);

void lua_pushstringvect(lua_State *L,stringvectst &vect);

void lua_pairs(lua_State *L,std::function<void(lua_State *L,const char *k,int v)>,int tbl_idx=-1);

void lua_getfield_or_createtable(lua_State *L,int n,const char *k,int narr=0,int nrec=0);

#define lua_push_member(L,lua_push_type,field) lua_push##lua_push_type(L,field); lua_setfield(L,-2,#field)

#define lua_push_string_member(L,field) lua_pushstring(L,field.c_str()); lua_setfield(L,-2,#field)

#define lua_push_vec_member(L,lua_push_type,field) lua_createtable(L,(int)field.size(),0); for(auto i=0;i<field.size(); ++i) { lua_push##lua_push_type(L,field[i]); lua_seti(L,-2,(int)i+1); } lua_setfield(L,-2,#field)

#define lua_push_map_member(L,map,key) lua_pushstring(L,map.maybe_get(key).value_or("NONE").c_str()); lua_setfield(L,-2,#key)

#define lua_push_object_member(L,field) field.push_to_lua(L); lua_setfield(L,-2,#field)

#define lua_push_pointer_member(L,field) if (field) { field->push_to_lua(L); } else { lua_pushnil(L); } lua_setfield(L,-2,#field)

template<typename T>
void push_all_to_lua_no_tok(lua_State *L,svector<T> &vec) {
	lua_createtable(L,0,0);
	auto __index=[](lua_State *L) -> int {
		svector<T> *v_p=static_cast<svector<T>*>(lua_touserdata(L,lua_upvalueindex(1)));
		if (!v_p) return 0;
		auto &vec=*v_p;
		if (lua_isnumber(L,2))
			{
			int i=lua_tonumber(L,2);
			if (i>=1 && i<=vec.size())
				{
				vec[i-1]->push_to_lua(L);
				return 1;
				}
			}
		return 0;
		};
	auto __len=[](lua_State *L) -> int {
		svector<T> *v_p=static_cast<svector<T>*>(lua_touserdata(L,lua_upvalueindex(1)));
		if (!v_p) return 0;
		lua_pushinteger(L,v_p->size());
		return 1;
		};
	lua_createtable(L,0,2);
	lua_pushlightuserdata(L,&vec);
	lua_pushcclosure(L,__index,1);
	lua_setfield(L,-2,"__index");
	lua_pushlightuserdata(L,&vec);
	lua_pushcclosure(L,__len,1);
	lua_setfield(L,-2,"__len");
	lua_setmetatable(L,-2);
	// leaves the table on top of the stack!
	}

template<typename T>
void push_all_to_lua(lua_State *L,std::vector<T> &vec) {
	lua_createtable(L,0,0);
	auto __index=[](lua_State *L) -> int {
		std::vector<T> *v_p=static_cast<std::vector<T>*>(lua_touserdata(L,lua_upvalueindex(1)));
		if (!v_p) return 0;
		auto &vec=*v_p;
		if (lua_isnumber(L,2))
			{
			int i=lua_tonumber(L,2);
			if (i>=1 && i<=vec.size())
				{
				vec[i-1]->push_to_lua(L);
				return 1;
				}
			}
		else if (lua_isstring(L,2))
			{
			auto str=lua_tostring(L,2);
			for (auto e : vec)
				{
				if (e->token==str)
					{
					e->push_to_lua(L);
					return 1;
					}
				}
			}
		return 0;
		};
	auto __len=[](lua_State *L) -> int {
		std::vector<T> *v_p=static_cast<std::vector<T>*>(lua_touserdata(L,lua_upvalueindex(1)));
		if (!v_p) return 0;
		lua_pushinteger(L,v_p->size());
		return 1;
		};
	lua_createtable(L,0,2);
	lua_pushlightuserdata(L,&vec);
	lua_pushcclosure(L,__index,1);
	lua_setfield(L,-2,"__index");
	lua_pushlightuserdata(L,&vec);
	lua_pushcclosure(L,__len,1);
	lua_setfield(L,-2,"__len");
	lua_setmetatable(L,-2);
	// leaves the table on top of the stack!
	}

#define lua_push_lazy_member(L, vec) push_all_to_lua(L,vec); lua_setfield(L,-2,#vec)

#define lua_push_lazy_member_no_tok(L, vec) push_all_to_lua_no_tok(L,vec); lua_setfield(L,-2,#vec)

// Deeply abusing templates; U is always going to be flagarrayst,
// which is not yet defined. I guess if something else has `has_flag` this is fine
template<typename T,typename U>
void lua_push_flags_member(lua_State *L,const T &flag_string_map, const U &flags) {
	auto to_lower=[](const auto &str) {
		std::string copy=str;
		std::transform(str.cbegin(),str.cend(),copy.begin(),
					   [](auto c) {return std::tolower(c); });
		return copy;
		};
	for (auto &[str,flag] : flag_string_map)
		{
		lua_pushboolean(L,flags.has_flag(flag));
		lua_setfield(L,-2,to_lower(str).c_str());
		}
	}

template<typename T, typename U>
void lua_push_vec_to_map(lua_State *L,const std::vector<T> &vec,const U &map) {
	lua_createtable(L,0,(int)vec.size());
	for (auto i=0; i<vec.size(); ++i)
		{
		if (auto e=map.maybe_get(vec[i]); e)
			{
			lua_pushboolean(L,true);
			lua_setfield(L,-2,e.value().c_str());
			}
		}
}

#define lua_push_vec_member_to_map(L,vec,map) lua_push_vec_to_map(L,vec,map); lua_setfield(L,-2,#vec)

// An owning lua state...
struct LuaState {

	lua_State *inner;

	operator lua_State *() {
		return inner;
		}

	/*
	* You don't wanna copy these, since that would cause
	* double-closing of the lua state, so,
	* we delete copy constructors/assignment
	*/
	LuaState(const LuaState &other)=delete;
	LuaState &operator=(const LuaState &other)=delete;

	LuaState(LuaState &&other) noexcept {
		inner=other.inner;
		other.inner=NULL;
		}
	// Moving it is fine, though
	LuaState& operator=(LuaState &&other) noexcept {
		inner=other.inner;
		other.inner=NULL;
		return *this;
		}


	LuaState() {
		inner=luaL_newstate();
		luaL_openlibs(inner);
		add_lua_df_globals(inner);
		}
	~LuaState() {
		if (inner)
			{
			lua_close(inner);
			}
		}
	};