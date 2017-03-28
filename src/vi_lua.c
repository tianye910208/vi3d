#include "vi_lua.h"
#include "vi_log.h"
#include "vi_mem.h"

static vi_lua_state* __lua_state = NULL;

int _vi_lua_print(lua_State* L)
{
	int nargs = lua_gettop(L);
	char str[4096];
	int i;
	for (i = 1; i <= nargs; i++)
	{
		if (lua_istable(L, i))
			strcat(str, "table");
		else if (lua_isnone(L, i))
			strcat(str, "none");
		else if (lua_isnil(L, i))
			strcat(str, "nil");
		else if (lua_isboolean(L, i))
		{
			if (lua_toboolean(L, i) != 0)
				strcat(str, "true");
			else
				strcat(str, "false");
		}
		else if (lua_isfunction(L, i))
			strcat(str, "function");
		else if (lua_islightuserdata(L, i))
			strcat(str, "lightuserdata");
		else if (lua_isthread(L, i))
			strcat(str, "thread");
		else
		{
			const char * s = lua_tostring(L, i);
			if (s)
				strcat(str, s);
			else
				strcat(str, lua_typename(L, lua_type(L, i)));
		}
		if (i != nargs)
			strcat(str, "\t");
	}
	vi_log_print(str);
	return 0;
}

int vi_lua_init()
{
	lua_State* L = luaL_newstate();
	if (L == NULL)
	{
		vi_log("[VI3D][E]create lua state failed");
		return 1;
	}
	luaL_openlibs(L);

	const luaL_Reg funcs[] = {
		//{"print", _vi_lua_print},
		{NULL, NULL}
	};
	lua_getglobal(L, "_G");
	luaL_setfuncs(L, funcs, 0);

	__lua_state = vi_mem_alloc(sizeof(vi_lua_state));
	__lua_state->L = L;
	return 0;
}

int vi_lua_exit()
{
	if (__lua_state)
	{
		lua_close(__lua_state->L);
		vi_mem_free(__lua_state);
		__lua_state = NULL;
	}
	return 0;
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

int vi_lua_call(const char* funcname)
{
	return 0;
}









