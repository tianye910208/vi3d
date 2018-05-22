#include "vi_math.h"
#include "vi_lua.h"
#ifdef VI3D_SYS_WIN
#include <malloc.h>
#endif


static void* userdata_get_or_die(lua_State *L, int idx) {
	void* ret = lua_touserdata(L, idx);
	luaL_argcheck(L, ret != NULL, idx, "Userdata should not be NULL");
	return ret;
}

static void* userdata_get_or_new(lua_State* L, int idx, int size) {
	void* ret = lua_touserdata(L, idx);
	if (ret == NULL)
		ret = lua_newuserdata(L, size);
	return ret;
}

static int _llfunc_vec2(lua_State *L) {
	vec2* arg = lua_touserdata(L, 1);
	vec2* ret = lua_newuserdata(L, sizeof(vec2));
	if (arg != NULL)
		*ret = *arg;
	return 1;
}

static int _llfunc_vec2_tostring(lua_State *L) {
	vec3 *v = userdata_get_or_die(L, 1);
	lua_pushfstring(L, "[%f, %f, %f]", v->x, v->y, v->z);
	return 1;
}

static int ll_vec2(lua_State* L) {
	const luaL_Reg __reg[] = {
		{ "__tostring", _llfunc_vec2_tostring },
		{ NULL, NULL }
	};

	lua_newtable(L);
	luaL_newlib(L, __reg);
	lua_setfield(L, -2, "__index");
	lua_pushstring(L, "vec2");
	lua_setfield(L, -2, "__metatable");
	lua_pushcfunction(L, _llfunc_vec2_tostring);
	lua_setfield(L, -2, "__tostring");
	
	lua_pushvalue(L, -1);
	lua_pushcclosure(L, _llfunc_vec2, 1);
	lua_setfield(L, -4, "vec2");

	lua_setfield(L, -4, "_vec2");
	return 1;
}

static int ll_vec3(lua_State* L) {
	return 1;
}

static int ll_vec4(lua_State* L) {
	return 1;
}

static int ll_mat4(lua_State* L) {
	return 1;
}


int ll_math_open(lua_State* L) {
	lua_pushglobaltable(L);
	ll_vec2(L);
	ll_vec3(L);
	ll_vec4(L);
	ll_mat4(L);
    return 1;
}

