--util.lua--------------------------
--@tianye112197
--@2016-12-07
------------------------------------
local lfs = lfs or require("lfs")

function lsdir(path, ext, ret)
    ret = ret or {}
    for file in lfs.dir(path) do
        if file ~= "." and file ~= ".." then
            local f = path.."/"..file
            local a = lfs.attributes(f)
            if a and a.mode == "directory" then
                lsdir(f, ext, ret)
            else
                if ext then
                    if string.match(f, ext) then
                        table.insert(ret, f)
                    end
                else
                    table.insert(ret, f)
                end
            end
        end
    end
    return ret
end

function mkdir(path)
    local dir = string.match(path, "(.+)[/\\].-$")
    if dir and lfs.attributes(dir) == nil then
        mkdir(dir)
    end
    lfs.mkdir(path)
end

function fcopy(from, dest)
    local ff = io.open(from, "rb")
    assert(ff, from)
    local fd = io.open(dest, "w+b")
    assert(fd, dest)
    
    local data = ff:read("*a")
    ff:close()
    
    fd:write(data)
    fd:close()
end


function tostr(var)
    local vt = type(var)
    if vt == "string" then
        return "\""..var.."\""
    elseif vt == "number" then
        return tostring(var)
    elseif vt == "boolean" then
        return var and "true" or "false"
    elseif vt == "table" then
        local idx = 0
        local str = "{"
        for i,v in ipairs(var) do
            idx = i
            str = str..tostr(v)..","
        end
        for k,v in pairs(var) do
            local kt = type(k)
            if kt == "string" then
                str = str..k.."="..tostr(v)..","
            elseif kt == "number" then
                if k < 1 or k > idx then
                    str = str.."["..k.."]="..tostr(v)..","
                end
            else
                error("[UTIL]not support k "..kt.." "..k)
            end
        end
        return str.."}"
    else
        error("[UTIL]not support v "..vt.." "..tostring(var))
    end
end












