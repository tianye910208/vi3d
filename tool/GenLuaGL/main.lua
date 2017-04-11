require("read")
require("work")

local srclist = {}

local deflist = read_func()
for i,v in ipairs(deflist) do
    local desc, func, text = work_func(v)
    table.insert(srclist, {name=v.name, desc=desc, func=func, text=text})
end

local fd = io.open("ll_gles.c", "w+")
fd:write("#include \"vi_sys.h\"\n")
fd:write("#include \"vi_lua.h\"\n")
fd:write("#ifdef VI3D_SYS_WIN\n#include <malloc.h>\n#endif\n")

for i,v in ipairs(srclist) do
    fd:write("\n")
    fd:write(v.desc)
    fd:write("\n")
    fd:write(v.text)
    fd:write("\n")
end


fd:write("\n\nstatic const luaL_Reg __lib_gles[] = {\n")
for i,v in ipairs(srclist) do
  fd:write("    {\""..v.name.."\", "..v.func.."},\n")
end
fd:write("    {NULL, NULL}\n};\n\n");

fd:write("int ll_gles_open(lua_State* L) {\n")
fd:write("    lua_pushglobaltable(L);\n")
fd:write("    luaL_setfuncs(L, __lib_gles, 0);\n")
fd:write("    return 1;\n")
fd:write("}\n\n")

fd:close()



















