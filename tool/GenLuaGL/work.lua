local push_b = function(name)
    return "lua_pushboolen(L, (int)"..name..");\n"
end

local push_n = function(name)
    return "lua_pushinteger(L, (lua_Integer)"..name..");\n"
end

local push_f = function(name)
    return "lua_pushnumber(L, (lua_Number)"..name..");\n"
end

local push_s = function(name, vlen)
    if vlen then
        return "lua_pushlstring(L, (const char*)"..name..", "..vlen..");\n"
    else
        return "lua_pushstring(L, (const char*)"..name..");\n"
    end
end

local push_n_list = function(name)
    return "lua_pushinteger(L, (lua_Integer)"..name..");\n"
end


local load_b = function(name, tt, idx)
    return tt.." "..name.." = ("..tt..")lua_toboolean(L, "..idx..");\n"
end

local load_n = function(name, tt, idx)
    return tt.." "..name.." = ("..tt..")lua_tointeger(L, "..idx..");\n"
end

local load_f = function(name, tt, idx)
    return tt.." "..name.." = ("..tt..")lua_tonumber(L, "..idx..");\n"
end

local load_s = function(name, tt, idx)
    return tt.." "..name.." = ("..tt..")lua_tostring(L, "..idx..");\n"
end

local load_n_list = function(name, tt, idx)
    return tt.." "..name.." = ("..tt..")lua_tointeger(L, "..idx..");\n"
end

local load_f_list = function(name, tt, idx)
    return tt.." "..name.." = ("..tt..")lua_tonumber(L, "..idx..");\n"
end

local load_ptr_var = function(name, tt, idx)
    return string.gsub(tt, "%*$", name..";"), "&"..name
end

local load_def_var = function(name, tt, idx)
    return string.gsub(tt, "%*$", name.."[n];")
end

local load_def_str = function(name, tt, idx)
    return string.gsub(tt, "%*$", name.."[256];")
end


local ret_type = {
    ["GLboolean"] = push_b,
    ["const GLubyte*"] = push_s,
    ["GLuint"] = push_n,
    ["GLint"] = push_n,
    ["GLenum"] = push_n,
}

local arg_type = {
    ["GLintptr"] = {load_n},
    ["GLclampf"] = {load_f},
    ["GLfloat"] = {load_f},
    ["GLuint"] = {load_n},
    ["GLsizeiptr"] = {load_n},
    ["GLsizei"] = {load_n},
    ["GLbitfield"] = {load_n},
    ["GLenum"] = {load_n},
    ["GLint"] = {load_n},
    ["GLboolean"] = {load_b},

    ["const GLchar *"] = {load_s},
    ["const GLvoid *"] = {load_s},
    ["const GLint *"] = {load_n_list},
    ["const GLuint *"] = {load_n_list},
    ["const GLfloat *"] = {load_f_list},
    ["const void *"] = {load_s},


    ["GLboolean *"] = {load_ptr_var, push_b},
    ["GLfloat *"] = {load_ptr_var, push_f},
    ["GLsizei *"] = {load_ptr_var, push_n},
    ["GLint *"] = {load_ptr_var, push_n},
    ["GLenum *"] = {load_ptr_var, push_n},
    ["GLuint *"] = {load_def_var, push_n_list},
    ["GLchar *"] = {load_def_str, push_s},
}




function gen_src(def, tag)
    local src = "static int "..tag.."(lua_State* L) {\n"
    src = src .. "    return 0;"
    src = src .. "\n}"
    return src
end


function work_func(def)
    local str = "//"..def.type.." "..def.name.."("
    local n = #def.args
    for i,arg in ipairs(def.args) do
        str = str .. arg.type.." "..arg.name
        if i < n then
            str = str .. ", "
        else 
            str = str .. ")"
        end
    end


    local tag = "_llfunc_"..def.name
    return str, tag, gen_src(def, tag)
end











