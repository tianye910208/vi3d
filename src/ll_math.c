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
	int argc = lua_gettop(L);
	vec2* ret = lua_newuserdata(L, sizeof(vec2));

	if (argc == 1) {
		vec2* arg = (vec2*)lua_touserdata(L, 1);
		if (arg != NULL)
			*ret = *arg;
	}
	else if (argc == 3) {
		ret->x = (float)luaL_checknumber(L, 1);
		ret->y = (float)luaL_checknumber(L, 2);
	}

	lua_pushvalue(L, lua_upvalueindex(1));
	lua_setmetatable(L, -2);
	
	return 1;
}

static int _llfunc_vec2_get(lua_State *L) {
	vec2* v = (vec2*)userdata_get_or_die(L, 1);
	lua_pushnumber(L, v->x);
	lua_pushnumber(L, v->y);
	return 2;
}

static int _llfunc_vec2_set(lua_State *L) {
	vec2* v = (vec2*)userdata_get_or_die(L, 1);

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
	vec2* v = (vec2*)userdata_get_or_die(L, 1);
	int i = (int)luaL_checkinteger(L, 2);
	lua_pushnumber(L, v->p[i-1]);
	return 1;
}

static int _llfunc_vec2_seti(lua_State *L) {
	vec2* v = (vec2*)userdata_get_or_die(L, 1);
	int i = (int)luaL_checkinteger(L, 2);
	float f = (float)luaL_checknumber(L, 3);
	v->p[i - 1] = f;
	return 0;
}

static int _llfunc_vec2_meta(lua_State *L) {
	lua_pushvalue(L, lua_upvalueindex(1));
	return 1;
}

static int _llfunc_vec2_tostring(lua_State *L) {
	vec2 *v = (vec2*)userdata_get_or_die(L, 1);
	lua_pushfstring(L, "[vec2][%f, %f]", v->x, v->y);
	return 1;
}

static int _llfunc_vec2_add(lua_State *L) {
	vec2 *a = (vec2*)userdata_get_or_die(L, 1);
	vec2 *b = (vec2*)userdata_get_or_die(L, 2);
	vec2 *r = (vec2*)userdata_get_or_new(L, 3, sizeof(vec2));
	vec2_add(a, b, r);
	return 1;
}

static int _llfunc_vec2_sub(lua_State *L) {
	vec2 *a = (vec2*)userdata_get_or_die(L, 1);
	vec2 *b = (vec2*)userdata_get_or_die(L, 2);
	vec2 *r = (vec2*)userdata_get_or_new(L, 3, sizeof(vec2));
	vec2_sub(a, b, r);
	return 1;
}

static int _llfunc_vec2_mul(lua_State *L) {
	vec2 *v = (vec2*)userdata_get_or_die(L, 1);
	float k = (float)luaL_checknumber(L, 2);
	vec2 *r = (vec2*)userdata_get_or_new(L, 3, sizeof(vec2));
	vec2_mul(v, k, r);
	return 1;
}

static int _llfunc_vec2_len(lua_State *L) {
	vec2 *v = (vec2*)userdata_get_or_die(L, 1);
	lua_pushnumber(L, vec2_len(v));
	return 1;
}

static int _llfunc_vec2_dot(lua_State *L) {
	vec2 *a = (vec2*)userdata_get_or_die(L, 1);
	vec2 *b = (vec2*)userdata_get_or_die(L, 2);
	lua_pushnumber(L, vec2_dot(a, b));
	return 1;
}

static int _llfunc_vec2_normalize(lua_State *L) {
	vec2 *v = (vec2*)userdata_get_or_die(L, 1);
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
		{ "vec2_meta", _llfunc_vec2_meta },
		{ "vec2_tostring", _llfunc_vec2_tostring },
		{ "vec2_add", _llfunc_vec2_add },
		{ "vec2_sub", _llfunc_vec2_sub },
		{ "vec2_mul", _llfunc_vec2_mul },
		{ "vec2_len", _llfunc_vec2_len },
		{ "vec2_dot", _llfunc_vec2_dot },
		{ "vec2_normalize", _llfunc_vec2_normalize },
		{ NULL, NULL }
	};
	luaL_setfuncs(L, __reg, 1);
	return 1;
}



static int _llfunc_vec3_new(lua_State *L) {
	int argc = lua_gettop(L);
	vec3* ret = lua_newuserdata(L, sizeof(vec3));

	if (argc == 1) {
		vec3* arg = (vec3*)lua_touserdata(L, 1);
		if (arg != NULL)
			*ret = *arg;
	}
	else if (argc == 3) {
		ret->x = (float)luaL_checknumber(L, 1);
		ret->y = (float)luaL_checknumber(L, 2);
		ret->z = (float)luaL_checknumber(L, 3);
	}

	lua_pushvalue(L, lua_upvalueindex(1));
	lua_setmetatable(L, -2);

	return 1;
}

static int _llfunc_vec3_get(lua_State *L) {
	vec3* v = (vec3*)userdata_get_or_die(L, 1);
	lua_pushnumber(L, v->x);
	lua_pushnumber(L, v->y);
	lua_pushnumber(L, v->z);
	return 3;
}

static int _llfunc_vec3_set(lua_State *L) {
	vec3* v = (vec3*)userdata_get_or_die(L, 1);

	int isnum = 0;
	float f = (float)lua_tonumberx(L, 2, &isnum);
	if (isnum)
		v->x = f;
	f = (float)lua_tonumberx(L, 3, &isnum);
	if (isnum)
		v->y = f;
	f = (float)lua_tonumberx(L, 4, &isnum);
	if (isnum)
		v->z = f;

	return 0;
}

static int _llfunc_vec3_geti(lua_State *L) {
	vec3* v = (vec3*)userdata_get_or_die(L, 1);
	int i = (int)luaL_checkinteger(L, 2);
	lua_pushnumber(L, v->p[i - 1]);
	return 1;
}

static int _llfunc_vec3_seti(lua_State *L) {
	vec3* v = (vec3*)userdata_get_or_die(L, 1);
	int i = (int)luaL_checkinteger(L, 2);
	float f = (float)luaL_checknumber(L, 3);
	v->p[i - 1] = f;
	return 0;
}

static int _llfunc_vec3_meta(lua_State *L) {
	lua_pushvalue(L, lua_upvalueindex(1));
	return 1;
}

static int _llfunc_vec3_tostring(lua_State *L) {
	vec3 *v = (vec3*)userdata_get_or_die(L, 1);
	lua_pushfstring(L, "[vec3][%f, %f, %f]", v->x, v->y, v->z);
	return 1;
}

static int _llfunc_vec3_add(lua_State *L) {
	vec3 *a = (vec3*)userdata_get_or_die(L, 1);
	vec3 *b = (vec3*)userdata_get_or_die(L, 2);
	vec3 *r = (vec3*)userdata_get_or_new(L, 3, sizeof(vec3));
	vec3_add(a, b, r);
	return 1;
}

static int _llfunc_vec3_sub(lua_State *L) {
	vec3 *a = (vec3*)userdata_get_or_die(L, 1);
	vec3 *b = (vec3*)userdata_get_or_die(L, 2);
	vec3 *r = (vec3*)userdata_get_or_new(L, 3, sizeof(vec3));
	vec3_sub(a, b, r);
	return 1;
}

static int _llfunc_vec3_mul(lua_State *L) {
	vec3 *v = (vec3*)userdata_get_or_die(L, 1);
	if (lua_isuserdata(L, 2)) {
		mat4 *m = (mat4*)lua_touserdata(L, 2);
		vec3 *r = (vec3*)userdata_get_or_new(L, 3, sizeof(vec3));
		vec3_mul_mat4(v, m, r);
	}
	else {
		float k = (float)luaL_checknumber(L, 2);
		vec3 *r = (vec3*)userdata_get_or_new(L, 3, sizeof(vec3));
		vec3_mul(v, k, r);
	}
	return 1;
}

static int _llfunc_vec3_len(lua_State *L) {
	vec3 *v = (vec3*)userdata_get_or_die(L, 1);
	lua_pushnumber(L, vec3_len(v));
	return 1;
}

static int _llfunc_vec3_dot(lua_State *L) {
	vec3 *a = (vec3*)userdata_get_or_die(L, 1);
	vec3 *b = (vec3*)userdata_get_or_die(L, 2);
	lua_pushnumber(L, vec3_dot(a, b));
	return 1;
}

static int _llfunc_vec3_cross(lua_State *L) {
	vec3 *a = (vec3*)userdata_get_or_die(L, 1);
	vec3 *b = (vec3*)userdata_get_or_die(L, 2);
	vec3 *r = (vec3*)userdata_get_or_new(L, 3, sizeof(vec3));
	vec3_cross(a, b, r);
	return 1;
}

static int _llfunc_vec3_normalize(lua_State *L) {
	vec3 *v = (vec3*)userdata_get_or_die(L, 1);
	vec3_normalize(v);
	return 0;
}

static int ll_vec3(lua_State* L) {
	luaL_newmetatable(L, "vec3");
	const luaL_Reg __reg[] = {
		{ "vec3_new", _llfunc_vec3_new },
		{ "vec3_get", _llfunc_vec3_get },
		{ "vec3_set", _llfunc_vec3_set },
		{ "vec3_geti", _llfunc_vec3_geti },
		{ "vec3_seti", _llfunc_vec3_seti },
		{ "vec3_meta", _llfunc_vec3_meta },
		{ "vec3_tostring", _llfunc_vec3_tostring },
		{ "vec3_add", _llfunc_vec3_add },
		{ "vec3_sub", _llfunc_vec3_sub },
		{ "vec3_mul", _llfunc_vec3_mul },
		{ "vec3_len", _llfunc_vec3_len },
		{ "vec3_dot", _llfunc_vec3_dot },
		{ "vec3_cross", _llfunc_vec3_cross },
		{ "vec3_normalize", _llfunc_vec3_normalize },
		{ NULL, NULL }
	};
	luaL_setfuncs(L, __reg, 1);
	return 1;
}



static int _llfunc_vec4_new(lua_State *L) {
	int argc = lua_gettop(L);
	vec4* ret = lua_newuserdata(L, sizeof(vec4));

	if (argc == 1) {
		vec4* arg = (vec4*)lua_touserdata(L, 1);
		if (arg != NULL)
			*ret = *arg;
	}
	else if (argc == 4) {
		ret->x = (float)luaL_checknumber(L, 1);
		ret->y = (float)luaL_checknumber(L, 2);
		ret->z = (float)luaL_checknumber(L, 3);
		ret->w = (float)luaL_checknumber(L, 4);
	}

	lua_pushvalue(L, lua_upvalueindex(1));
	lua_setmetatable(L, -2);

	return 1;
}

static int _llfunc_vec4_get(lua_State *L) {
	vec4* v = (vec4*)userdata_get_or_die(L, 1);
	lua_pushnumber(L, v->x);
	lua_pushnumber(L, v->y);
	lua_pushnumber(L, v->z);
	lua_pushnumber(L, v->w);
	return 4;
}

static int _llfunc_vec4_set(lua_State *L) {
	vec4* v = (vec4*)userdata_get_or_die(L, 1);

	int isnum = 0;
	float f = (float)lua_tonumberx(L, 2, &isnum);
	if (isnum)
		v->x = f;
	f = (float)lua_tonumberx(L, 3, &isnum);
	if (isnum)
		v->y = f;
	f = (float)lua_tonumberx(L, 4, &isnum);
	if (isnum)
		v->z = f;
	f = (float)lua_tonumberx(L, 5, &isnum);
	if (isnum)
		v->w = f;
	return 0;
}

static int _llfunc_vec4_geti(lua_State *L) {
	vec4* v = (vec4*)userdata_get_or_die(L, 1);
	int i = (int)luaL_checkinteger(L, 2);
	lua_pushnumber(L, v->p[i - 1]);
	return 1;
}

static int _llfunc_vec4_seti(lua_State *L) {
	vec4* v = (vec4*)userdata_get_or_die(L, 1);
	int i = (int)luaL_checkinteger(L, 2);
	float f = (float)luaL_checknumber(L, 3);
	v->p[i - 1] = f;
	return 0;
}

static int _llfunc_vec4_meta(lua_State *L) {
	lua_pushvalue(L, lua_upvalueindex(1));
	return 1;
}

static int _llfunc_vec4_tostring(lua_State *L) {
	vec4 *v = (vec4*)userdata_get_or_die(L, 1);
	lua_pushfstring(L, "[vec4][%f, %f, %f, %f]", v->x, v->y, v->z, v->w);
	return 1;
}

static int _llfunc_vec4_add(lua_State *L) {
	vec4 *a = (vec4*)userdata_get_or_die(L, 1);
	vec4 *b = (vec4*)userdata_get_or_die(L, 2);
	vec4 *r = (vec4*)userdata_get_or_new(L, 3, sizeof(vec4));
	vec4_add(a, b, r);
	return 1;
}

static int _llfunc_vec4_sub(lua_State *L) {
	vec4 *a = (vec4*)userdata_get_or_die(L, 1);
	vec4 *b = (vec4*)userdata_get_or_die(L, 2);
	vec4 *r = (vec4*)userdata_get_or_new(L, 3, sizeof(vec4));
	vec4_sub(a, b, r);
	return 1;
}

static int _llfunc_vec4_mul(lua_State *L) {
	vec4 *v = (vec4*)userdata_get_or_die(L, 1);
	if (lua_isuserdata(L, 2)) {
		mat4 *m = (mat4*)lua_touserdata(L, 2);
		vec4 *r = (vec4*)userdata_get_or_new(L, 3, sizeof(vec4));
		vec4_mul_mat4(v, m, r);
	}
	else {
		float k = (float)luaL_checknumber(L, 2);
		vec4 *r = (vec4*)userdata_get_or_new(L, 3, sizeof(vec4));
		vec4_mul(v, k, r);
	}
	return 1;
}

static int _llfunc_vec4_len(lua_State *L) {
	vec4 *v = (vec4*)userdata_get_or_die(L, 1);
	lua_pushnumber(L, vec4_len(v));
	return 1;
}

static int _llfunc_vec4_normalize(lua_State *L) {
	vec4 *v = (vec4*)userdata_get_or_die(L, 1);
	vec4_normalize(v);
	return 0;
}

static int _llfunc_vec4_quat_mul(lua_State *L) {
	vec4 *a = (vec4*)userdata_get_or_die(L, 1);
	vec4 *b = (vec4*)userdata_get_or_die(L, 2);
	vec4 *r = (vec4*)userdata_get_or_new(L, 3, sizeof(vec4));
	vec4_quat_mul(a, b, r);
	return 1;
}

static int _llfunc_vec4_quat_rot(lua_State *L) {
	vec4 *q = (vec4*)userdata_get_or_die(L, 1);
	vec3 *v = (vec3*)userdata_get_or_die(L, 2);
	vec3 *r = (vec3*)userdata_get_or_new(L, 3, sizeof(vec3));
	vec4_quat_rot(q, v, r);
	return 1;
}

static int _llfunc_vec4_quat_nlerp(lua_State *L) {
	vec4 *a = (vec4*)userdata_get_or_die(L, 1);
	vec4 *b = (vec4*)userdata_get_or_die(L, 2);
	float k = (float)luaL_checknumber(L, 3);
	vec4 *r = (vec4*)userdata_get_or_new(L, 4, sizeof(vec4));
	vec4_quat_nlerp(a, b, k, r);
	return 1;
}

static int _llfunc_vec4_quat_slerp(lua_State *L) {
	vec4 *a = (vec4*)userdata_get_or_die(L, 1);
	vec4 *b = (vec4*)userdata_get_or_die(L, 2);
	float k = (float)luaL_checknumber(L, 3);
	vec4 *r = (vec4*)userdata_get_or_new(L, 4, sizeof(vec4));
	vec4_quat_slerp(a, b, k, r);
	return 1;
}

static int _llfunc_vec4_quat_set_euler(lua_State *L) {
	vec4 *q = (vec4*)userdata_get_or_die(L, 1);
	vec3 *euler = (vec3*)userdata_get_or_die(L, 2);
	vec4_quat_set_euler(q, euler);
	return 0;
}

static int _llfunc_vec4_quat_set_axis_angle(lua_State *L) {
	vec4 *q = (vec4*)userdata_get_or_die(L, 1);
	vec3 *axis = (vec3*)userdata_get_or_die(L, 2);
	float angle = (float)luaL_checknumber(L, 3);
	vec4_quat_set_axis_angle(q, axis, angle);
	return 0;
}

static int ll_vec4(lua_State* L) {
	luaL_newmetatable(L, "vec4");
	const luaL_Reg __reg[] = {
		{ "vec4_new", _llfunc_vec4_new },
		{ "vec4_get", _llfunc_vec4_get },
		{ "vec4_set", _llfunc_vec4_set },
		{ "vec4_geti", _llfunc_vec4_geti },
		{ "vec4_seti", _llfunc_vec4_seti },
		{ "vec4_meta", _llfunc_vec4_meta },
		{ "vec4_tostring", _llfunc_vec4_tostring },
		{ "vec4_add", _llfunc_vec4_add },
		{ "vec4_sub", _llfunc_vec4_sub },
		{ "vec4_mul", _llfunc_vec4_mul },
		{ "vec4_len", _llfunc_vec4_len },
		{ "vec4_normalize", _llfunc_vec4_normalize },
		{ "vec4_quat_mul", _llfunc_vec4_quat_mul },
		{ "vec4_quat_rot", _llfunc_vec4_quat_rot },
		{ "vec4_quat_nlerp", _llfunc_vec4_quat_nlerp },
		{ "vec4_quat_slerp", _llfunc_vec4_quat_slerp },
		{ "vec4_quat_set_euler", _llfunc_vec4_quat_set_euler },
		{ "vec4_quat_set_axis_angle", _llfunc_vec4_quat_set_axis_angle },
		{ NULL, NULL }
	};
	luaL_setfuncs(L, __reg, 1);
	return 1;
}





static int _llfunc_mat4_new(lua_State *L) {
	mat4* arg = (mat4*)lua_touserdata(L, 1);
	mat4* ret = lua_newuserdata(L, sizeof(mat4));
	if (arg != NULL)
		*ret = *arg;

	lua_pushvalue(L, lua_upvalueindex(1));
	lua_setmetatable(L, -2);

	return 1;
}

static int _llfunc_mat4_get(lua_State *L) {
	mat4* m = (mat4*)userdata_get_or_die(L, 1);
	float* p = m->p;
	for (int i = 0; i < 16; i++)
		lua_pushnumber(L, p[i]);
	return 16;
}

static int _llfunc_mat4_set(lua_State *L) {
	mat4* m = (mat4*)userdata_get_or_die(L, 1);
	float* p = m->p;
	for (int i = 0; i < 16; i++) {
		int isnum = 0;
		float f = (float)lua_tonumberx(L, 2 + i, &isnum);
		if (isnum)
			p[i] = f;
	}
	return 0;
}

static int _llfunc_mat4_geti(lua_State *L) {
	mat4* m = (mat4*)userdata_get_or_die(L, 1);
	int i = (int)luaL_checkinteger(L, 2);
	lua_pushnumber(L, m->p[i - 1]);
	return 1;
}

static int _llfunc_mat4_seti(lua_State *L) {
	mat4* m = (mat4*)userdata_get_or_die(L, 1);
	int i = (int)luaL_checkinteger(L, 2);
	float f = (float)luaL_checknumber(L, 3);
	m->p[i - 1] = f;
	return 0;
}

static int _llfunc_mat4_meta(lua_State *L) {
	lua_pushvalue(L, lua_upvalueindex(1));
	return 1;
}

static int _llfunc_mat4_tostring(lua_State *L) {
	mat4 *m = (mat4*)userdata_get_or_die(L, 1);
	lua_pushfstring(L, "[mat4][%f, %f, %f, %f]\n      [%f, %f, %f, %f]\n      [%f, %f, %f, %f]\n      [%f, %f, %f, %f]", 
		m->m00, m->m01, m->m02, m->m03,
		m->m10, m->m11, m->m12, m->m13, 
		m->m20, m->m21, m->m22, m->m23, 
		m->m30, m->m31, m->m32, m->m33);

	return 1;
}

static int _llfunc_mat4_mul(lua_State *L) {
	mat4 *a = (mat4*)userdata_get_or_die(L, 1);
	mat4 *b = (mat4*)userdata_get_or_die(L, 2);
	mat4 *r = (mat4*)userdata_get_or_new(L, 3, sizeof(mat4));
	mat4_mul(a, b, r);
	return 1;
}

static int _llfunc_mat4_set_identity(lua_State *L) {
	mat4 *m = (mat4*)userdata_get_or_die(L, 1);
	mat4_set_identity(m);
	return 0;
}

static int _llfunc_mat4_set_zero(lua_State *L) {
	mat4 *m = (mat4*)userdata_get_or_die(L, 1);
	mat4_set_zero(m);
	return 0;
}

static int _llfunc_mat4_set_quat(lua_State *L) {
	mat4 *m = (mat4*)userdata_get_or_die(L, 1);
	vec4 *q = (vec4*)userdata_get_or_die(L, 2);
	mat4_set_quat(m, q);
	return 0;
}

static int _llfunc_mat4_translate(lua_State *L) {
	mat4 *m = (mat4*)userdata_get_or_die(L, 1);
	vec3 *v = (vec3*)userdata_get_or_die(L, 2);
	mat4_translate(m, v);
	return 0;
}

static int _llfunc_mat4_rotate(lua_State *L) {
	mat4 *m = (mat4*)userdata_get_or_die(L, 1);
	vec4 *q = (vec4*)userdata_get_or_die(L, 2);
	mat4_rotate(m, q);
	return 0;
}

static int _llfunc_mat4_scale(lua_State *L) {
	mat4 *m = (mat4*)userdata_get_or_die(L, 1);
	vec3 *v = (vec3*)userdata_get_or_die(L, 2);
	mat4_scale(m, v);
	return 0;
}

static int _llfunc_mat4_inverse(lua_State *L) {
	mat4 *m = (mat4*)userdata_get_or_die(L, 1);
	mat4_inverse(m);
	return 0;
}

static int _llfunc_mat4_transpose(lua_State *L) {
	mat4 *m = (mat4*)userdata_get_or_die(L, 1);
	mat4_transpose(m);
	return 0;
}

static int ll_mat4(lua_State* L) {
	luaL_newmetatable(L, "mat4");
	const luaL_Reg __reg[] = {
		{ "mat4_new", _llfunc_mat4_new },
		{ "mat4_get", _llfunc_mat4_get },
		{ "mat4_set", _llfunc_mat4_set },
		{ "mat4_geti", _llfunc_mat4_geti },
		{ "mat4_seti", _llfunc_mat4_seti },
		{ "mat4_meta", _llfunc_mat4_meta },
		{ "mat4_tostring", _llfunc_mat4_tostring },
		{ "mat4_mul", _llfunc_mat4_mul },
		{ "mat4_set_identity", _llfunc_mat4_set_identity },
		{ "mat4_set_zero", _llfunc_mat4_set_zero },
		{ "mat4_set_quat", _llfunc_mat4_set_quat },
		{ "mat4_translate", _llfunc_mat4_translate },
		{ "mat4_rotate", _llfunc_mat4_rotate },
		{ "mat4_scale", _llfunc_mat4_scale },
		{ "mat4_inverse", _llfunc_mat4_inverse },
		{ "mat4_transpose", _llfunc_mat4_transpose },
		{ NULL, NULL }
	};
	luaL_setfuncs(L, __reg, 1);
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

