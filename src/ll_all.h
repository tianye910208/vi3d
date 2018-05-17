#ifndef _VI3D_LUALIB_ALL_
#define _VI3D_LUALIB_ALL_

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

int ll_all_open(lua_State* L);


int ll_gles_open(lua_State* L);
int ll_math_open(lua_State* L);
int ll_vi3d_open(lua_State* L);

#endif 


