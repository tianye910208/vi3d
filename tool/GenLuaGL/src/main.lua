require("src/util_lfs")
require("src/util_xml")

require("src/gen_by_xml")
require("src/gen_by_src")


local srclist = {}
gen_by_xml(srclist)
gen_by_src(srclist)


local fd = io.open("out/ll_gles.c", "w+")
fd:write("#include \"vi_sys.h\"\n")
fd:write("#include \"vi_lua.h\"\n")
fd:write("#ifdef VI3D_SYS_WIN\n#include <malloc.h>\n#endif\n")

fcopy("src/gles.lua", "out/gles.lua")
local fdoc = io.open("out/gles.lua", "a")

for i,v in ipairs(srclist) do
    fd:write("\n")
    fd:write("//"..v.desc)
    fd:write("\n")
    fd:write("//"..v.docs)
    fd:write("\n")
    fd:write(v.text)
    fd:write("\n")
    
    fdoc:write("\n")
    fdoc:write("--"..v.desc)
    fdoc:write("\n")
    fdoc:write("--"..v.docs)
    fdoc:write("\n\n")
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

fdoc:close()






















