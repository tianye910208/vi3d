#include "vi_lua.h"
#include "vi_log.h"
#include "vi_mem.h"

static vi_lua_state* __lua_state = NULL;

int _vi_lua_print(lua_State* L)
{
	char str[4096];
	str[0] = '\0';

	int n = lua_gettop(L); 
	lua_getglobal(L, "tostring");
	for (int i = 1; i <= n; i++) 
	{
		const char *s;
		size_t l;
		lua_pushvalue(L, -1);  /* function to be called */
		lua_pushvalue(L, i);   /* value to print */
		lua_call(L, 1, 1);
		s = lua_tolstring(L, -1, &l);  /* get result */
		if (i > 1)
			strcat(str, "\t");
		if (s == NULL)
			strcat(str, "***");
		else
			strcat(str, s);
		lua_pop(L, 1);  /* pop result */
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
		{"print", _vi_lua_print},
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









