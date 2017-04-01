require("util")
require("xml")


function finddef(node, list)
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

function loaddef()
    local tmplist = {}
    local xmllist = lsdir("xml")
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
            local arg = {type = v.value, name = v.parameter[1].value}
            if arg.type or arg.name ~= "void" then
                table.insert(args, arg)
            end
        end
        
        local def = {
            type = func.value, 
            name = func["function"][1].value,
            args = args
        }
        deflist[#deflist+1] = def
    end
    return deflist
end

function main()
    local deflist = loaddef()
    for i,v in ipairs(deflist) do
        local str = v.type.."\t"..v.name.."("
        local n = #v.args
        for i,arg in ipairs(v.args) do
            str = str .. arg.type.." "..arg.name
            if i < n then
                str = str .. ", "
            end
        end
        str = str .. ")"
        print(str)
    end
end

print("main-----------------------------")
main()






