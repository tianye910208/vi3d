#ifndef _VI3D_LUA_
#define _VI3D_LUA_

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

typedef struct _vi_lua_state {
	lua_State* L;
	
	int func_update;
	int func_render;
	int func_onerror;
} vi_lua_state;

int vi_lua_init();
void vi_lua_exit();
void vi_lua_update(float dt);
void vi_lua_render(float dt);

vi_lua_state* vi_lua_getstate();
int vi_lua_exec(const char* str);
int vi_lua_set_func(int func_update, int func_render, int func_onerror);




#endif 


