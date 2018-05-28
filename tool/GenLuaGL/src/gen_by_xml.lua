

local function finddef(node, list)
    for k,v in pairs(node) do
        if type(v) == "table" then
            for i,v in ipairs(v) do
                if v.label == "funcprototype" then
                    list[#list+1] = v
                else
                    finddef(v, list)
                end
            end
        end
    end
end

local function loadstr(str)
    if str then
        str = string.gsub(str, "^[%s%c]*", "")
        str = string.gsub(str, "[%s%c]*$", "")
    end
    return str
end

local function loaddef()
    local tmplist = {}
    local xmllist = lsdir("xml", "%.xml$", nil, false)
    for i,v in ipairs(xmllist) do
        print("======"..v.."======")
        local fd = io.open(v, "r+")
        local str = fd:read("*a")
        local xml = loadxml(str)
        if not xml then
            error("xml load fail:", v)
        end
        
        finddef(xml, tmplist)
    end
    local deflist = {}
    for i,v in ipairs(tmplist) do
        local func = v.funcdef[1]
        local args = {}
        for i,v in ipairs(v.paramdef or {}) do
            local arg = {type = loadstr(v.value), name = loadstr(v.parameter[1].value)}
            if arg.type or arg.name ~= "void" then
                table.insert(args, arg)
            end
        end
        
        local def = {
            type = loadstr(func.value), 
            name = loadstr(func["function"][1].value),
            args = args
        }
        deflist[#deflist+1] = def
    end
    return deflist
end

local function load_def_list()
    local deflist = loaddef()
    table.sort(deflist, function(a, b) return a.name < b.name end)
    
    --[[
    local retTypes = {}
    local argTypes = {}
    for i,v in ipairs(deflist) do
        retTypes[v.type] = (retTypes[v.type] or 0) + 1
        local str = v.type.."\t"..v.name.."("
        local n = #v.args
        for i,arg in ipairs(v.args) do
            argTypes[arg.type] = (argTypes[arg.type] or 0) + 1
            str = str .. arg.type.." "..arg.name
            if i < n then
                str = str .. ", "
            end
        end
        str = str .. ")"
        print(str)
        work(v)
    end
    
    print("\ntype--------------------------")
    for k,v in pairs(retTypes) do
        print("ret", k,"",v)
    end
    for k,v in pairs(argTypes) do
        print("arg", k,"",v)
    end
    --]]
    return deflist
end







local load_b = function(name, tt, idx, def)
    return tt.." "..name.." = ("..tt..")lua_toboolean(L, "..idx..");\n", "<bool>"..name
end

local load_n = function(name, tt, idx, def)
    return tt.." "..name.." = ("..tt..")luaL_checkinteger(L, "..idx..");\n", "<int>"..name
end

local load_f = function(name, tt, idx, def)
    return tt.." "..name.." = ("..tt..")luaL_checknumber(L, "..idx..");\n", "<float>"..name
end

local load_s = function(name, tt, idx, def)
    return tt.." "..name.." = ("..tt..")luaL_checkstring(L, "..idx..");\n", "<string>"..name
end

local load_n_list = function(name, tt, idx, def)
    local ttt = string.gsub(string.gsub(tt, "%s*%*$", ""), "^const%s*", "")
    local src = tt.." "..name..";\n"..
                "    if(lua_istable(L, "..idx..")){\n"..
                "        int _ll_tabn = (int)lua_rawlen(L, "..idx..");\n"..
                "#ifdef VI3D_SYS_WIN\n"..
                "        "..ttt.." *_"..name.." = ("..ttt.." *)alloca(sizeof("..ttt..")*_ll_tabn);\n"..
                "#else\n"..
                "        "..ttt.." _"..name.."[_ll_tabn];\n"..
                "#endif\n"..
                "        for(int i = 0; i < _ll_tabn; i++) {\n"..
                "            lua_rawgeti(L, "..idx..", i+1);\n"..
                "            _"..name.."[i] = ("..ttt..")luaL_checkinteger(L, -1);\n"..
                "            lua_pop(L, 1);\n"..
                "        }\n"..
                "        "..name.." = ("..tt..")_"..name..";\n"..
                "    }else{\n"..
                "        "..name.." = ("..tt..")luaL_checkstring(L, "..idx..");\n"..
                "    }\n"

    return src, "{[int]}"..name.."/<string>"..name.."Packed", name
end

local load_f_list = function(name, tt, idx, def)
    local ttt = string.gsub(string.gsub(tt, "%s*%*$", ""), "^const%s*", "")
    local src = tt.." "..name..";\n"..
                "    if(lua_isuserdata(L, "..idx..")) {\n"..
                "        "..name.." = ("..tt..")lua_touserdata(L, "..idx..");\n"..
                "    }else if(lua_istable(L, "..idx..")) {\n"..
                "        int _ll_tabn = (int)lua_rawlen(L, "..idx..");\n"..
                "#ifdef VI3D_SYS_WIN\n"..
                "        "..ttt.." *_"..name.." = ("..ttt.." *)alloca(sizeof("..ttt..")*_ll_tabn);\n"..
                "#else\n"..
                "        "..ttt.." _"..name.."[_ll_tabn];\n"..
                "#endif\n"..
                "        for(int i = 0; i < _ll_tabn; i++) {\n"..
                "            lua_rawgeti(L, "..idx..", i+1);\n"..
                "            _"..name.."[i] = ("..ttt..")luaL_checknumber(L, -1);\n"..
                "            lua_pop(L, 1);\n"..
                "        }\n"..
                "        "..name.." = ("..tt..")_"..name..";\n"..
                "    }else{\n"..
                "        "..name.." = ("..tt..")luaL_checkstring(L, "..idx..");\n"..
                "    }\n"
    
    return src, "{[float]}"..name.."/<string>"..name.."Packed", name
end

local load_def_ptr = function(name, tt, idx, def)
    return string.gsub(tt, "%*$", name..";\n"), nil, "&"..name
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
    
    return src, nil, nil
end


local push_b = function(name, tt, idx, def)
    return "lua_pushboolean(L, (int)"..name..");\n", "<bool>"..name
end

local push_n = function(name, tt, idx, def)
    return "lua_pushinteger(L, (lua_Integer)"..name..");\n", "<int>"..name
end

local push_f = function(name, tt, idx, def)
    return "lua_pushnumber(L, (lua_Number)"..name..");\n", "<float>"..name
end

local push_s = function(name, tt, idx, def)
    return "lua_pushstring(L, (const char*)"..name..");\n", "<string>"..name
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
    return src, "{[int]}"..name
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



local function gen_src_auto(def, tag)
    local doc = {name = def.name, args = {}, rets = {}}
    local src = "static int "..tag.."(lua_State* L) {\n"
    
    for i,v in ipairs(def.args) do
        local f = arg_type[v.type]
        local text,desc,mark = f[1](v.name, v.type, i, def)
        v.mark = mark or v.name
        src = src .. "    " .. text
        
        if desc then
            table.insert(doc.args, desc)
        end
    end
    
    local retf = ret_type[def.type]
    
    src = src .. "\n    " .. (retf and (def.type.." _ll_ret = ") or (""))
    src = src .. def.name .. "("
    for i,v in ipairs(def.args) do
        if i > 1 then
            src = src .. ", "
        end
        src = src .. v.mark
    end
    src = src .. ");\n\n"
    
    local retn = 0
    if retf then
        local text, desc = retf("_ll_ret", def.type, 0, def)
        src = src .. "    " .. text
        retn = retn + 1
        
        if desc then
            table.insert(doc.rets, desc)
        end
    end
    for i,v in ipairs(def.args) do
        local f = arg_type[v.type]
        if f[2] then
            local text, desc = f[2](v.name, v.type, i, def)
            src = src .. "    " .. text
            retn = retn + 1
            
            if desc then
                table.insert(doc.rets, desc)
            end
        end
    end
    
    
    src = src .. "    return "..retn..";"
    src = src .. "\n}"
    return src, doc
end


local function gen_by_def(def)
    local str = def.type.." "..def.name.."("
    for i,arg in ipairs(def.args) do
        if i > 1 then
            str = str .. ", "
        end
        str = str .. arg.type.." "..arg.name
    end
    str = str .. ")"

    local tag = "_llfunc_"..def.name
    local src, doc = gen_src_auto(def, tag)
    
    local docs = ""
    for i,v in ipairs(doc.rets) do
        if i == 1 then
            docs = docs .. v
        else
            docs = docs .. ", " .. v
        end
    end
    if #doc.rets > 0 then
        docs = "local " .. docs .. " = "
    end
    
    docs = docs .. doc.name .. "("
    for i,v in ipairs(doc.args) do
        if i == 1 then
            docs = docs .. v
        else
            docs = docs .. ", " .. v
        end
    end
    docs = docs ..  ")"
    
    return str, tag, src, docs
end


function gen_by_xml(srclist)
    local deflist = load_def_list()
    for i,v in ipairs(deflist) do
        local desc, func, text, docs = gen_by_def(v)
        table.insert(srclist, {name=v.name, desc=desc, func=func, text=text, docs = docs})
    end
end








