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
	if (ret == NULL){
		ret = lua_newuserdata(L, size);
		lua_pushvalue(L, lua_upvalueindex(1));
		lua_setmetatable(L, -2);
	}
	return ret;
}



static int _llfunc_vec2_new(lua_State *L) {
	vec2* arg = lua_touserdata(L, 1);
	vec2* ret = lua_newuserdata(L, sizeof(vec2));
	if (arg != NULL)
		*ret = *arg;

	lua_pushvalue(L, lua_upvalueindex(1));
	lua_setmetatable(L, -2);
	
	return 1;
}

static int _llfunc_vec2_get(lua_State *L) {
	vec2* v = userdata_get_or_die(L, 1);
	lua_pushnumber(L, v->x);
	lua_pushnumber(L, v->y);
	return 2;
}

static int _llfunc_vec2_set(lua_State *L) {
	vec2* v = userdata_get_or_die(L, 1);

	int isnum = 0;
	float f = (float)lua_tonumberx(L, 2, &isnum);
	if (isnum)
		v->x = f;
	f = (float)lua_tonumberx(L, 3, &isnum);
	if (isnum)
		v->y = f;

	return 0;
}

static int _llfunc_vec2_geti(lua_State *L) {
	vec2* v = userdata_get_or_die(L, 1);
	int i = (int)luaL_checkinteger(L, 2);
	lua_pushnumber(L, v->p[i-1]);
	return 1;
}

static int _llfunc_vec2_seti(lua_State *L) {
	vec2* v = userdata_get_or_die(L, 1);
	int i = (int)luaL_checkinteger(L, 2);
	float f = (float)luaL_checknumber(L, 3);
	v->p[i - 1] = f;
	return 0;
}

static int _llfunc_vec2_tostring(lua_State *L) {
	vec2 *v = userdata_get_or_die(L, 1);
	lua_pushfstring(L, "[%f, %f]", v->x, v->y);
	return 1;
}

static int _llfunc_vec2_metatable(lua_State *L) {
	lua_pushvalue(L, lua_upvalueindex(1));
	return 1;
}


static int _llfunc_vec2_add(lua_State *L) {
	vec2 *a = userdata_get_or_die(L, 1);
	vec2 *b = userdata_get_or_die(L, 2);
	vec2 *r = userdata_get_or_new(L, 3, sizeof(vec2));
	vec2_add(a, b, r);
	return 1;
}

static int _llfunc_vec2_sub(lua_State *L) {
	vec2 *a = userdata_get_or_die(L, 1);
	vec2 *b = userdata_get_or_die(L, 2);
	vec2 *r = userdata_get_or_new(L, 3, sizeof(vec2));
	vec2_sub(a, b, r);
	return 1;
}

static int _llfunc_vec2_mul(lua_State *L) {
	vec2 *v = userdata_get_or_die(L, 1);
	float k = (float)luaL_checknumber(L, 2);
	vec2 *r = userdata_get_or_new(L, 3, sizeof(vec2));
	vec2_mul(v, k, r);
	return 1;
}

static int _llfunc_vec2_dot(lua_State *L) {
	vec2 *a = userdata_get_or_die(L, 1);
	vec2 *b = userdata_get_or_die(L, 2);
	lua_pushnumber(L, vec2_dot(a, b));
	return 1;
}

static int _llfunc_vec2_len(lua_State *L) {
	vec2 *v = userdata_get_or_die(L, 1);
	lua_pushnumber(L, vec2_len(v));
	return 1;
}

static int _llfunc_vec2_normalize(lua_State *L) {
	vec2 *v = userdata_get_or_die(L, 1);
	vec2_normalize(v);
	return 0;
}

static int ll_vec2(lua_State* L) {
	luaL_newmetatable(L, "vec2");
	const luaL_Reg __reg[] = {
		{ "vec2_new", _llfunc_vec2_new },
		{ "vec2_get", _llfunc_vec2_get },
		{ "vec2_set", _llfunc_vec2_set },
		{ "vec2_geti", _llfunc_vec2_geti },
		{ "vec2_seti", _llfunc_vec2_seti },
		{ "vec2_tostring", _llfunc_vec2_tostring },
		{ "vec2_metatable", _llfunc_vec2_metatable },
		{ "vec2_add", _llfunc_vec2_add },
		{ "vec2_sub", _llfunc_vec2_sub },
		{ "vec2_mul", _llfunc_vec2_mul },
		{ "vec2_dot", _llfunc_vec2_dot },
		{ "vec2_len", _llfunc_vec2_len },
		{ "vec2_normalize", _llfunc_vec2_normalize },
		{ NULL, NULL }
	};
	luaL_setfuncs(L, __reg, 1);
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

