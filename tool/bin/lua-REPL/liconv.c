#include <stdio.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "liconv.h"


#ifdef _WIN32
#include <windows.h>

static int iconv_utf8_gbk(lua_State* L) {
	const char *str_utf8 = luaL_checkstring(L, 1);

	int len_unicode = MultiByteToWideChar(CP_UTF8, 0, (char *)str_utf8, -1, NULL, 0);//获取转换到Unicode编码后所需要的字符空间长度
	WCHAR* str_unicode = (WCHAR*)malloc(sizeof(WCHAR)*(len_unicode + 1));
	if (!MultiByteToWideChar(CP_UTF8, 0, (char *)str_utf8, -1, str_unicode, len_unicode)){
		free(str_unicode);
		return 0;
	}

	int len_gbk = WideCharToMultiByte(CP_ACP, 0, str_unicode, len_unicode, NULL, 0, NULL, NULL);//获取转换到GBK编码后所需要的字符空间长度
	char* str_gbk = (char*)malloc(len_gbk + 1);
	if (!WideCharToMultiByte(CP_ACP, 0, str_unicode, len_unicode, str_gbk, len_gbk, NULL, NULL)){
		free(str_unicode);
		free(str_gbk);
		return 0;
	}

	free(str_unicode);
	lua_pushstring(L, str_gbk);
	free(str_gbk);


	return 1;
}

static int iconv_gbk_utf8(lua_State* L) {
	const char *str_gbk = luaL_checkstring(L, 1);

	int len_unicode = MultiByteToWideChar(CP_ACP, 0, (char *)str_gbk, -1, NULL, 0);//获取转换到Unicode编码后所需要的字符空间长度
	WCHAR* str_unicode = (WCHAR*)malloc(sizeof(WCHAR)*(len_unicode + 1));
	if (!MultiByteToWideChar(CP_ACP, 0, (char *)str_gbk, -1, str_unicode, len_unicode)){
		free(str_unicode);
		return 0;
	}
	
	int len_utf8 = WideCharToMultiByte(CP_UTF8, 0, str_unicode, len_unicode, NULL, 0, NULL, NULL);//获取转换到UTF8编码后所需要的字符空间长度
	char* str_utf8 = (char*)malloc(len_utf8 + 1);
	if (!WideCharToMultiByte(CP_UTF8, 0, str_unicode, len_unicode, str_utf8, len_utf8, NULL, NULL)){
		free(str_unicode);
		free(str_utf8);
		return 0;
	}
	
	free(str_unicode);
	lua_pushstring(L, str_utf8);
	free(str_utf8);

	return 1;
}

#else

#endif



static const struct luaL_Reg iclib[] = {
	{"utf8_gbk", iconv_utf8_gbk},
	{"gbk_utf8", iconv_gbk_utf8},
	{NULL, NULL},
};

int luaopen_liconv (lua_State *L) {
    lua_newtable(L);
    lua_pushvalue(L, -1);
    lua_setglobal(L, "iconv");
	luaL_setfuncs(L, iclib, 0);
	return 1;
}
