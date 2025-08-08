#include "lua.h"
#include "platform.h"
#include "enabler.h"
#include "random.h"

std::mutex lualog_mutex;

void lualog_string(std::string str) {
	if (str.empty()) return;
	std::scoped_lock lk(lualog_mutex);
	std::ofstream fseed("lualog.txt",std::ios::out | std::ios::app);
	if (fseed.is_open())
		{
		fseed<<str<<std::endl;
		}
	fseed.close();
	}

void lualog_string(std::string str,int err,lua_State *L) {
	//SAVE AN ERROR TO THE LOG FILE
	str+=": ";
	switch (err)
		{
		case LUA_ERRRUN:
			str+="Runtime error: ";
			str+=string(luaL_checklstring(L,-1,NULL));
			lua_pop(L,1);
			break;
		case LUA_ERRMEM:
			str+="Memory allocation error: ";
			str+=string(luaL_checklstring(L,-1,NULL));
			break;
		case LUA_ERRERR:
			str+="Error error (sic)";
			break;
		case LUA_ERRSYNTAX:
			{
			str+="Syntax error: ";
			str+=string(luaL_checklstring(L,-1,NULL));
			lua_pop(L,1);
			break;
			}
		case LUA_YIELD:
			str+="Yielded";
			break;
		case LUA_ERRFILE:
			{
			str+="Could not find file: ";
			str+=string(luaL_checklstring(L,-1,NULL));
			lua_pop(L,1);
			break;
			}
		default:
			str="";
			break;
		}
	lualog_string(str);
	}

void lua_print_whole_stack(std::string str,lua_State *L) {
	lualog_string(str);
	int top=lua_gettop(L);
	for (int i=1; i <= top; i++)
		{
		string str;
		add_long_to_string(i,str);
		str+=" ";
		str+=luaL_typename(L,i);
		lualog_string(str);
		}
	}

int lua_logged_pcall(std::string str,lua_State *L,int n,int r,int f) {
	int load_err=LUA_OK;
	if (load_err=lua_pcall(L,n,r,f); load_err)
		{
		lualog_string(str,load_err,L);
		}
	return load_err;
	}

int lua_run_file(lua_State *L,filest file) {
	if (auto maybe_f=file.any_location(); maybe_f)
		{
		auto f=maybe_f.value().string();
		auto load_err=luaL_loadfile(L,f.c_str());
		if (load_err)
			{
			lualog_string(file.path.string(),load_err,L);
			return load_err;
			}
		return lua_logged_pcall(f,L,0,LUA_MULTRET,0);
		}
	else
		{
		lualog_string("Could not find file: " + file.path.string());
		return LUA_ERRFILE;
		}
	}

int lua_load_directory(lua_State *L,filest path) {
	lua_getglobal(L,"package");
	lua_getfield(L,-1,"path");
	std::string cur_path=lua_tostring(L,-1);
	std::string orig_path=cur_path;
	std::filesystem::path path_obj=path.any_location_unchecked();
	cur_path+=";"+path_obj.string()+"/?.lua";
	lua_pop(L,1);
	lua_pushstring(L,cur_path.c_str());
	lua_setfield(L,-2,"path");
	lua_pop(L,1);
	std::string f=(path_obj/"init.lua").string();
	auto load_err=luaL_loadfile(L,f.c_str());
	if (load_err)
		{
		if (load_err != LUA_ERRFILE)
			{
			lualog_string(f,load_err,L);
			}
		else
			{
			// failing this way puts a string on the stack; get rid of it
			lua_pop(L,1);
			}
		}
	else
		{
		load_err=lua_logged_pcall(f,L,0,LUA_MULTRET,0);
		}
	lua_getglobal(L,"package");
	lua_pushstring(L,orig_path.c_str());
	lua_setfield(L,-2,"path");
	lua_pop(L,1);
	return load_err;
	}

void lua_string_table_to_stringvect(lua_State *L,stringvectst &vect) {
	lua_pushnil(L);
	while (lua_next(L,-2) != 0)
		{
		vect.add_string(lua_tostring(L,-1));
		lua_pop(L,1);
		}
	}

void lua_pushstringvect(lua_State *L,stringvectst &vect) {
	lua_createtable(L,(int)vect.str.size(),0);
	for (auto i=0; i<vect.str.size(); ++i)
		{
		lua_pushstring(L,vect.str[i]->dat.c_str());
		lua_seti(L,-2,i+1);
		}
	}

void lua_pairs(lua_State *L,std::function<void(lua_State *L,const char *k,int v)> f,int tbl_idx) {
	if (lua_istable(L,tbl_idx))
		{
		lua_pushnil(L);
		while (lua_next(L,tbl_idx-1) != 0)
			{
			lua_pushvalue(L,tbl_idx-1);
			const char *k=lua_tostring(L,-1);
			lua_pop(L,1);
			f(L,k,-1);
			lua_pop(L,1);
			}
		}
	else
		{
		lualog_string("Tried to iterate over a non-table!");
		}
	}
void lua_getfield_or_createtable(lua_State *L,int n,const char *k,int narr,int nrec) {
	auto t=lua_getfield(L,n,k);
	if (t!=LUA_TTABLE)
		{
		lua_pop(L,1);
		lua_createtable(L,narr,nrec);
		}
	}

static int lua_trandom(lua_State *L) {
	int n=lua_gettop(L);
	if (n<1)
		{
		lua_pushinteger(L,trandom());
		}
	else 
		{
		int m=luaL_checkinteger(L,1);
		lua_pushinteger(L,trandom(m));
		}
	return 1;
	}

static int lua_lualog(lua_State *L) {
	int n=lua_gettop(L);
	string s="";
	for (auto i=1; i<=n; ++i)
		{
		s+=luaL_checklstring(L,i,NULL);
		s+=" ";
		}
	lualog_string(s);
	return 0;
	}

void generate_utterance(string &str);

int lua_generate_utterance(lua_State *L) {
	string s;
	generate_utterance(s);
	lua_pushstring(L,s.c_str());
	return 1;
	}

int lua_capitalize_string_words(lua_State *L) {
	int n=lua_gettop(L);
	if (!n) return 0;
	string s="";
	s+=luaL_checklstring(L,1,NULL);
	capitalize_string_words(s);
	lua_pushstring(L,s.c_str());
	return 1;
	}

int lua_capitalize_string_first_word(lua_State *L) {
	int n=lua_gettop(L);
	if (!n) return 0;
	string s="";
	s+=luaL_checklstring(L,1,NULL);
	capitalize_string_first_word(s);
	lua_pushstring(L,s.c_str());
	return 1;
	}

void add_lua_df_globals(lua_State *L) {
	lua_register(L,"trandom",lua_trandom);
	lua_register(L,"lua_log",lua_lualog);
	lua_register(L,"utterance",lua_generate_utterance);
	lua_register(L,"capitalize_string_words",lua_capitalize_string_words);
	lua_register(L,"capitalize_string_first_word",lua_capitalize_string_first_word);
	lua_run_file(L,filest("data/init/globals.lua").with_flags(FILE_FLAG_ALWAYS_BASE_FIRST));
	}