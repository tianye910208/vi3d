


local load_b = function(name, tt, idx, def)
    return tt.." "..name.." = ("..tt..")lua_toboolean(L, "..idx..");\n"
end

local load_n = function(name, tt, idx, def)
    return tt.." "..name.." = ("..tt..")luaL_checkinteger(L, "..idx..");\n"
end

local load_f = function(name, tt, idx, def)
    return tt.." "..name.." = ("..tt..")luaL_checknumber(L, "..idx..");\n"
end

local load_s = function(name, tt, idx, def)
    return tt.." "..name.." = ("..tt..")luaL_checkstring(L, "..idx..");\n"
end

local load_n_list = function(name, tt, idx, def)
    local ttt = string.gsub(string.gsub(tt, "%s*%*$", ""), "^const%s*", "")
    local src = "int _ll_tabn = (int)luaL_len(L, "..idx..");"
    src = src .."\n#ifdef VI3D_SYS_WIN\n    "..ttt.." *"..name.." = ("..ttt.." *)alloca(sizeof("..ttt..")*_ll_tabn);"..
                "\n#else\n    "..ttt.." "..name.."[_ll_tabn];"..
                "\n#endif\n"
    
    src = src .."    for(int i = 0; i < _ll_tabn; i++) {\n"..
                "        lua_rawgeti(L, "..idx..", i+1);\n"..
                "        "..name.."[i] = ("..ttt..")luaL_checkinteger(L, -1);\n"..
                "        lua_pop(L, 1);\n"..
                "    }\n"

    return src, "("..tt..")"..name
end

local load_f_list = function(name, tt, idx, def)
    local ttt = string.gsub(string.gsub(tt, "%s*%*$", ""), "^const%s*", "")
    local src = "int _ll_tabn = (int)luaL_len(L, "..idx..");"
    src = src .."\n#ifdef VI3D_SYS_WIN\n    "..ttt.." *"..name.." = ("..ttt.." *)alloca(sizeof("..ttt..")*_ll_tabn);"..
                "\n#else\n    "..ttt.." "..name.."[_ll_tabn];"..
                "\n#endif\n"
    
    src = src .."    for(int i = 0; i < _ll_tabn; i++) {\n"..
                "        lua_rawgeti(L, "..idx..", i+1);\n"..
                "        "..name.."[i] = ("..ttt..")luaL_checknumber(L, -1);\n"..
                "        lua_pop(L, 1);\n"..
                "    }\n"

    return src, "("..tt..")"..name
end

local load_def_ptr = function(name, tt, idx, def)
    return string.gsub(tt, "%*$", name..";\n"), "&"..name
end

local load_def_vec = function(name, tt, idx, def)
    local arg_name = {
        ["GLsizei bufSize"] = true,
        ["GLsizei maxLength"] = true,
        ["GLsizei n"] = true,
    }
    local arg_maxn = nil
    for i,v in ipairs(def.args) do
        if arg_name[v.type .. " " .. v.name] then
            arg_maxn = v.name
            break
        end
    end
    
    if not arg_maxn then
        return load_def_ptr(name, tt, idx, def)
    end
    
    
    local ttt = string.gsub(tt, "%s%*$", "")
    local src = "\n#ifdef VI3D_SYS_WIN\n    "..ttt.." *"..name.." = ("..ttt.." *)alloca(sizeof("..ttt..")*"..arg_maxn..");"..
                "\n#else\n    "..ttt.." "..name.."["..arg_maxn.."];"..
                "\n#endif\n"
    
    return src
end


local push_b = function(name, tt, idx, def)
    return "lua_pushboolean(L, (int)"..name..");\n"
end

local push_n = function(name, tt, idx, def)
    return "lua_pushinteger(L, (lua_Integer)"..name..");\n"
end

local push_f = function(name, tt, idx, def)
    return "lua_pushnumber(L, (lua_Number)"..name..");\n"
end

local push_s = function(name, tt, idx, def)
    return "lua_pushstring(L, (const char*)"..name..");\n"
end

local push_n_list = function(name, tt, idx, def)
    local arg_name = {
        ["GLsizei n"] = true,
    }
    local arg_maxn = nil
    for i,v in ipairs(def.args) do
        if arg_name[v.type .. " " .. v.name] then
            arg_maxn = v.name
            break
        end
    end
    if not arg_maxn then
        return push_n(name, tt, idx, def)
    end
    
    local src = "lua_newtable(L);\n"
    src = src .."    for(int i = 0; i < "..arg_maxn.."; i ++) {\n"..
                "        lua_pushinteger(L, (lua_Integer)"..name.."[i]);\n"..
                "        lua_rawseti(L, -2, i+1);\n"..
                "    }\n"
    return src
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


    ["GLboolean *"] = {load_def_ptr, push_b},
    ["GLfloat *"] = {load_def_ptr, push_f},
    ["GLsizei *"] = {load_def_ptr, push_n},
    ["GLint *"] = {load_def_ptr, push_n},
    ["GLenum *"] = {load_def_ptr, push_n},
    ["GLuint *"] = {load_def_vec, push_n_list},
    ["GLchar *"] = {load_def_vec, push_s},
}



function gen_src(def, tag)
    local retn = 0
    local argn = #def.args

    local src = "static int "..tag.."(lua_State* L) {\n"
    
    for i,v in ipairs(def.args) do
        local f = arg_type[v.type]
        local ss,vv = f[1](v.name, v.type, i, def)
        v.mark = vv
        src = src .. "    " .. ss
    end
    
    local retf = ret_type[def.type]
    
    src = src .. "\n    " .. (retf and (def.type.." _ll_ret = ") or (""))
    src = src .. def.name .. "("
    for i,v in ipairs(def.args) do
        src = src .. (v.mark or v.name)
        if i < argn then
            src = src .. ", "
        end
    end
    src = src .. ");\n\n"
    
    
    if retf then
        src = src .. "    " .. retf("_ll_ret", def.type, 0, def)
        retn = retn + 1
    end
    for i,v in ipairs(def.args) do
        local f = arg_type[v.type]
        if f[2] then
            src = src .. "    " .. f[2](v.name, v.type, i, def)
            retn = retn + 1
        end
    end
    
    
    src = src .. "    return "..retn..";"
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











