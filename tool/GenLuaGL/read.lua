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

function loadstr(str)
    if str then
        str = string.gsub(str, "^[%s%c]*", "")
        str = string.gsub(str, "[%s%c]*$", "")
    end
    return str
end

function loaddef()
    local tmplist = {}
    local xmllist = lsdir("xml")
    for i,v in ipairs(xmllist) do
        --print("======"..v.."======")
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

function read_func()
    local deflist = loaddef()
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


