


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

local _llfunc_glDrawElements = [[
static int _llfunc_glDrawElements(lua_State* L) {
    GLenum mode = (GLenum)luaL_checkinteger(L, 1);
    GLsizei count = (GLsizei)luaL_checkinteger(L, 2);
    GLenum type = (GLenum)luaL_checkinteger(L, 3);
    
    if(lua_isinteger(L, 4)){
        GLintptr offset = (GLintptr)lua_tointeger(L, 4);
        glDrawElements(mode, count, type, (const GLvoid *)offset);
    }else if (lua_isuserdata(L, 4)){
        const GLvoid * indices = (const GLvoid *)lua_touserdata(L, 4);
        glDrawElements(mode, count, type, indices);
    }else {
        const GLvoid * indices = (const GLvoid *)luaL_checkstring(L, 4);
        glDrawElements(mode, count, type, indices);
    }

    return 0;
}
]]

local _llfunc_glVertexAttribPointer = [[
static int _llfunc_glVertexAttribPointer(lua_State* L) {
    GLuint index = (GLuint)luaL_checkinteger(L, 1);
    GLint size = (GLint)luaL_checkinteger(L, 2);
    GLenum type = (GLenum)luaL_checkinteger(L, 3);
    GLboolean normalized = (GLboolean)lua_toboolean(L, 4);
    GLsizei stride = (GLsizei)luaL_checkinteger(L, 5);
    
    if(lua_isinteger(L, 6)){
        GLintptr offset = (GLintptr)lua_tointeger(L, 6);
        glVertexAttribPointer(index, size, type, normalized, stride, (const GLvoid *)offset);
    }else if (lua_isuserdata(L, 6)){
        const GLvoid * pointer = (const GLvoid *)lua_touserdata(L, 6);
        glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    }else{
        const GLvoid * pointer = (const GLvoid *)luaL_checkstring(L, 6);
        glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    }
    return 0;
}
]]

local _src_list = {
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
    {
        name="glDrawElements", 
        desc="[Manual]void glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices/int offset)", 
        func="_llfunc_glDrawElements", 
        text=_llfunc_glDrawElements, 
        docs="[Manual]glDrawElements(<int>mode, <int>count, <int>type, <int>offset/<userdata>indices/<string>indices)"
    },
    {
        name="glVertexAttribPointer", 
        desc="[Manual]void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer/int offset)", 
        func="_llfunc_glVertexAttribPointer", 
        text=_llfunc_glVertexAttribPointer, 
        docs="[Manual]glVertexAttribPointer(<int>index, <int>size, <int>type, <bool>normalized, <int>stride, <int>offset/<userdata>pointer/<string>pointer)"
    },
}


function gen_by_src(srclist)
    for i,v in ipairs(_src_list) do
        table.insert(srclist, v)
    end
end



