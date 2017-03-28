#ifndef _VI3D_LUA_
#define _VI3D_LUA_

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

typedef struct _vi_lua_state
{
	lua_State* L;
} vi_lua_state;

int vi_lua_init();
int vi_lua_exit();

vi_lua_state* vi_lua_getstate();

int vi_lua_exec(const char* str);
int vi_lua_call(const char* funcname);

#endif 


