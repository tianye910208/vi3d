#include "ll_all.h"


int ll_all_open(lua_State* L) {
	luaL_checkversion(L);
	ll_gles_open(L);
	ll_math_open(L);
	ll_vi3d_open(L);
	return 1;
}


















