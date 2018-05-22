#include "vi_math.h"
#include "vi_lua.h"
#ifdef VI3D_SYS_WIN
#include <malloc.h>
#endif





static const luaL_Reg __lib_math[] = {
    {NULL, NULL}
};

int ll_math_open(lua_State* L) {
    lua_pushglobaltable(L);
    luaL_setfuncs(L, __lib_math, 0);
    return 1;
}

