#include "vi_lua.h"
#include "vi_log.h"
#include "vi_mem.h"
#include "ll_all.h"

static vi_lua_state* __lua_state = NULL;

vi_lua_state* vi_lua_getstate() {
	return __lua_state;
}

static int _vi_lua_init(lua_State* L) {
	luaL_openlibs(L);
	ll_all_open(L);

	lua_pushboolean(L, 1);  /* signal no errors */
	return 1;
}

int vi_lua_init() {
	lua_State* L = luaL_newstate();
	if (L == NULL) {
		vi_log("[VI3D][E]create lua state failed");
		return 1;
	}
	lua_pushcfunction(L, &_vi_lua_init);
	int status = lua_pcall(L, 0, 1, 0);  /* do the call */
	int result = lua_toboolean(L, -1);  /* get result */
	lua_pop(L, 1);

	if (status != LUA_OK || result == 0) {
		vi_log("[VI3D][E]setup lua state failed");
		return 1;
	}

	__lua_state = vi_mem_alloc(sizeof(vi_lua_state));
	memset(__lua_state, 0, sizeof(vi_lua_state));
	__lua_state->L = L;
	return 0;
}

int vi_lua_exit() {
	if (__lua_state) {
		lua_close(__lua_state->L);
		vi_mem_free(__lua_state);
		__lua_state = NULL;
		return 0;
	}
	return 1;
}

int vi_lua_exec(const char* str, const char* chunkname) {
	if (__lua_state)
		return luaL_loadbuffer(__lua_state->L, str, strlen(str), chunkname) || lua_pcall(__lua_state->L, 0, LUA_MULTRET, 0);
	return 1;
}

int vi_lua_call(int func) {
	if (func) {
		lua_State *L = __lua_state->L;
		lua_settop(L, 0);
		lua_rawgeti(L, LUA_REGISTRYINDEX, __lua_state->func_fail);
		lua_rawgeti(L, LUA_REGISTRYINDEX, func);
		lua_pcall(L, 0, 0, 1);
		return 0;
	}
	return 1;
}

int vi_lua_loop(float dt) {
	if (__lua_state->func_loop) {
		lua_State *L = __lua_state->L;
		lua_settop(L, 0);
		lua_rawgeti(L, LUA_REGISTRYINDEX, __lua_state->func_fail);
		lua_rawgeti(L, LUA_REGISTRYINDEX, __lua_state->func_loop);
		lua_pushnumber(L, dt);
		lua_pcall(L, 1, 0, 1);
		return 0;
	}
	return 1;
}

int vi_lua_main(int func_init, int func_loop, int func_fail) {
	if (__lua_state) {
		__lua_state->func_loop = func_loop;
		__lua_state->func_fail = func_fail;

		vi_lua_call(func_init);
		return 0;
	}
	return 1;
}














