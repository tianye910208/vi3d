






local _llfunc_glShaderSource = [[
static int _llfunc_glShaderSource(lua_State* L) {
    GLuint shader = (GLuint)luaL_checkinteger(L, 1);
    const GLchar * source = (const GLchar *)luaL_checkstring(L, 2);

    glShaderSource(shader, 1, (const GLchar **)&source, NULL);

    return 0;
}
]]

local _llfunc_glGetAttachedShaders = [[
static int _llfunc_glGetAttachedShaders(lua_State* L) {
    GLuint program = (GLuint)luaL_checkinteger(L, 1);
    GLuint maxCount = (GLuint)luaL_checkinteger(L, 2);
    
    GLsizei count;
#ifdef VI3D_SYS_WIN
    GLuint *shaders = (GLuint *)alloca(sizeof(GLuint)*maxCount);
#else
    GLuint shaders[maxCount];
#endif

    glGetAttachedShaders(program, maxCount, &count, shaders);
    
    lua_newtable(L);
    for(int i = 0; i < count; i ++) {
        lua_pushinteger(L, (lua_Integer)shaders[i]);
        lua_rawseti(L, -2, i+1);
    }
    return 1;
}
]]

local _llfunc_glReadPixels = [[
static int _llfunc_glReadPixels(lua_State* L) {
    GLuint x = (GLuint)luaL_checkinteger(L, 1);
    GLuint y = (GLuint)luaL_checkinteger(L, 2);
    GLuint width = (GLuint)luaL_checkinteger(L, 3);
    GLuint height = (GLuint)luaL_checkinteger(L, 4);
    GLuint format = (GLuint)luaL_checkinteger(L, 5);
    GLuint type = (GLuint)luaL_checkinteger(L, 6);
    
    GLuint n = 4 * width * height;
#ifdef VI3D_SYS_WIN
    GLchar *data = (GLchar *)alloca(sizeof(GLchar)*n);
#else
    GLchar data[n];
#endif

    glReadPixels(x, y, width, height, format, type, (GLvoid *)data);
    
    lua_pushlstring(L, (const char*)data, n);
    return 1;
}
]]


function gen_manual()
    return {
        {
            name="glShaderSource", 
            desc="[Manual]void glShaderSource(GLuint shader, GLsizei count, const GLchar * const * string, const GLint * length)", 
            func="_llfunc_glShaderSource", 
            text=_llfunc_glShaderSource, 
            docs="[Manual]glShaderSource(<int>shader, <string>string)"
        },
        {
            name="glGetAttachedShaders", 
            desc="[Manual]void glGetAttachedShaders(GLuint program, GLsizei maxCount, GLsizei * count, GLuint * shaders)", 
            func="_llfunc_glGetAttachedShaders", 
            text=_llfunc_glGetAttachedShaders, 
            docs="[Manual]local <int> shaders = glGetAttachedShaders(<int>program, <int>maxCount)"
        },
        {
            name="glReadPixels", 
            desc="[Manual]void glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * data)", 
            func="_llfunc_glReadPixels", 
            text=_llfunc_glReadPixels, 
            docs="[Manual]local <string> data = glReadPixels(<int>x, <int>y, <int>width, <int>height, <int>format, <int>type)"
        },
    }
end