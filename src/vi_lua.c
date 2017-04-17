#include "vi_lua.h"
#include "vi_log.h"
#include "vi_mem.h"
#include "ll_all.h"

static vi_lua_state* __lua_state = NULL;


static int _vi_lua_init(lua_State* L)
{
	luaL_openlibs(L);
	ll_all_open(L);

	lua_pushboolean(L, 1);  /* signal no errors */
	return 1;
}

int vi_lua_init()
{
	lua_State* L = luaL_newstate();
	if (L == NULL)
	{
		vi_log("[VI3D][E]create lua state failed");
		return 1;
	}
	lua_pushcfunction(L, &_vi_lua_init);
	int status = lua_pcall(L, 0, 1, 0);  /* do the call */
	int result = lua_toboolean(L, -1);  /* get result */
	lua_pop(L, 1);

	if (status != LUA_OK || result == 0)
	{
		vi_log("[VI3D][E]setup lua state failed");
		return 1;
	}

	__lua_state = vi_mem_alloc(sizeof(vi_lua_state));
	memset(__lua_state, 0, sizeof(vi_lua_state));
	__lua_state->L = L;
	return 0;
}

void vi_lua_exit()
{
	if (__lua_state)
	{
		lua_close(__lua_state->L);
		vi_mem_free(__lua_state);
		__lua_state = NULL;
	}
}

void vi_lua_update(float dt)
{
	if (__lua_state->func_update)
	{
		lua_State *L = __lua_state->L;
		lua_settop(L, 0);
		lua_rawgeti(L, LUA_REGISTRYINDEX, __lua_state->func_onerror);
		lua_rawgeti(L, LUA_REGISTRYINDEX, __lua_state->func_update);
		lua_pushnumber(L, dt);
		lua_pcall(L, 1, 0, 1);
	}
}

void vi_lua_render(float dt)
{
	if (__lua_state->func_render)
	{
		lua_State *L = __lua_state->L;
		lua_settop(L, 0);
		lua_rawgeti(L, LUA_REGISTRYINDEX, __lua_state->func_onerror);
		lua_rawgeti(L, LUA_REGISTRYINDEX, __lua_state->func_render);
		lua_pushnumber(L, dt);
		lua_pcall(L, 1, 0, 1);
	}
}

vi_lua_state* vi_lua_getstate()
{
	return __lua_state;
}

int vi_lua_exec(const char* str)
{
	if (__lua_state)
		return luaL_dostring(__lua_state->L, str);
	return 1;
}


int vi_lua_set_func(int func_update, int func_render, int func_onerror)
{
	if (__lua_state)
	{
		//vi_log("set_func %d %d %d", func_update, func_render, func_onerror);
		__lua_state->func_update = func_update;
		__lua_state->func_render = func_render;
		__lua_state->func_onerror = func_onerror;
		return 0;
	}
	return 1;
}














