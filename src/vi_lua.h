#ifndef _VI3D_LUA_
#define _VI3D_LUA_

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

typedef struct _vi_lua_state {
	lua_State* L;
	
	int func_loop;
	int func_fail;
} vi_lua_state;

vi_lua_state* vi_lua_getstate();

int vi_lua_init();
int vi_lua_exit();
int vi_lua_exec(const char* str, const char* chunkname);
int vi_lua_call(int func);
int vi_lua_loop(float dt);
int vi_lua_main(int func_init, int func_loop, int func_fail);




#endif 


