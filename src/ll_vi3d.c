#include "vi3d.h"
#ifdef VI3D_SYS_WIN
#include <malloc.h>
#endif



static int _llfunc_vi_log_print(lua_State* L)
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
		if (s == NULL)
			strcat(str, "***");
		else
			strcat(str, s);

		if (i == n)
			strcat(str, "\n");
		else
			strcat(str, "\t");

		lua_pop(L, 1);  /* pop result */
	}

	vi_log_print("%s", str);
	return 0;
}

static int _llfunc_vi_lua_set_func(lua_State* L)
{
	int f1 = 0;
	if (lua_isfunction(L, 1))
	{
		lua_pushvalue(L, 1);
		f1 = luaL_ref(L, LUA_REGISTRYINDEX);
	}
	int f2 = 0;
	if (lua_isfunction(L, 2))
	{
		lua_pushvalue(L, 2);
		f2 = luaL_ref(L, LUA_REGISTRYINDEX);
	}
	int f3 = 0;
	if (lua_isfunction(L, 3))
	{
		lua_pushvalue(L, 3);
		f3 = luaL_ref(L, LUA_REGISTRYINDEX);
	}
	vi_lua_set_func(f1, f2, f3);
	return 0;
}

static int _llfunc_vi_app_info(lua_State* L)
{
	vi_app* app = vi_app_info();

	lua_createtable(L, 0, 16);
	lua_pushnumber(L, app->time);
	lua_setfield(L, -2, "time");
	lua_pushinteger(L, app->screen_w);
	lua_setfield(L, -2, "screen_w");
	lua_pushinteger(L, app->screen_h);
	lua_setfield(L, -2, "screen_h");
	lua_pushinteger(L, app->design_w);
	lua_setfield(L, -2, "design_w");
	lua_pushinteger(L, app->design_h);
	lua_setfield(L, -2, "design_h");
	lua_pushinteger(L, app->viewport_x);
	lua_setfield(L, -2, "viewport_x");
	lua_pushinteger(L, app->viewport_y);
	lua_setfield(L, -2, "viewport_y");
	lua_pushinteger(L, app->viewport_w);
	lua_setfield(L, -2, "viewport_w");
	lua_pushinteger(L, app->viewport_h);
	lua_setfield(L, -2, "viewport_h");
	lua_pushstring(L, app->data_path);
	lua_setfield(L, -2, "data_path");
	lua_pushstring(L, app->save_path);
	lua_setfield(L, -2, "save_path");
	return 1;
}

static int _llfunc_vi_app_time(lua_State* L)
{
	lua_pushnumber(L, vi_app_time());
	return 1;
}

static int _llfunc_vi_app_set_design_size(lua_State* L)
{
	int w = (int)luaL_checkinteger(L, 1);
	int h = (int)luaL_checkinteger(L, 2);
	vi_app_set_design_size(w, h);
	return 0;
}

static int _llfunc_vi_file_open(lua_State* L)
{
	const char* filepath = luaL_checkstring(L, 1);
	const char* mode = luaL_checkstring(L, 2);
	vi_file* f = vi_file_open(filepath, mode);
	lua_pushlightuserdata(L, (void*)f);
	return 1;
}

static int _llfunc_vi_file_close(lua_State* L)
{
	if (!lua_islightuserdata(L, 1))
		luaL_argerror(L, 1, "#[vi_file* f] lightuserdata expected");

	vi_file* f = (vi_file*)lua_topointer(L, 1);
	vi_file_close(f);
	return 1;
}

static int _llfunc_vi_file_read(lua_State* L)
{
	if (!lua_islightuserdata(L, 1))
		luaL_argerror(L, 1, "#[vi_file* f] lightuserdata expected");

	vi_file* f = (vi_file*)lua_topointer(L, 1);
	int n = (int)luaL_checkinteger(L, 2);

#ifdef VI3D_SYS_WIN
	char *data = (char *)alloca(sizeof(char)*n);
#else
	char shaders[n];
#endif
	int read = vi_file_read(f, data, n); 
	lua_pushlstring(L, data, n);
	lua_pushinteger(L, read);
	return 2;
}

static int _llfunc_vi_file_seek(lua_State* L)
{
	if (!lua_islightuserdata(L, 1))
		luaL_argerror(L, 1, "#[vi_file* f] lightuserdata expected");

	vi_file* f = (vi_file*)lua_topointer(L, 1);
	int offset = (int)luaL_checkinteger(L, 2);
	int origin = (int)luaL_checkinteger(L, 3);

	int ret = vi_file_seek(f, offset, origin);
	lua_pushinteger(L, ret);
	return 1;
}

static int _llfunc_vi_file_size(lua_State* L)
{
	if (!lua_islightuserdata(L, 1))
		luaL_argerror(L, 1, "#[vi_file* f] lightuserdata expected");

	vi_file* f = (vi_file*)lua_topointer(L, 1);

	int ret = vi_file_size(f);
	lua_pushinteger(L, ret);
	return 1;
}

static const luaL_Reg __lib_vi3d[] = {
	{ "print", _llfunc_vi_log_print },
	{ "vi_log_print", _llfunc_vi_log_print },
	{ "vi_lua_set_func", _llfunc_vi_lua_set_func },
	{ "vi_app_info", _llfunc_vi_app_info },
	{ "vi_app_time", _llfunc_vi_app_time },
	{ "vi_app_set_design_size", _llfunc_vi_app_set_design_size },
	{ "vi_file_open", _llfunc_vi_file_open },
	{ "vi_file_close", _llfunc_vi_file_close },
	{ "vi_file_read", _llfunc_vi_file_read },
	{ "vi_file_seek", _llfunc_vi_file_seek },
	{ "vi_file_size", _llfunc_vi_file_size }, 
    {NULL, NULL}
};

int ll_vi3d_open(lua_State* L) {
    lua_pushglobaltable(L);
    luaL_setfuncs(L, __lib_vi3d, 0);
    return 1;
}

