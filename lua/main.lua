

local app_func = {}
local function app_init()
	vi_app_set_design_size(800, 480)
    
    local app = vi_app_info()
    
    package.searchers[4] = nil
    package.searchers[3] = nil
    package.searchers[2] = function(filename)
        local p = vi_file_open(app.data_path.."lua/"..filename..".lua", "r")
        local s = vi_file_read(p, vi_file_size(p))
        vi_file_close(p)
        
        local f,err = load(s, filename)
        return f or err
    end

    require("vi3d/gles")
    require("vi3d/math")
    
    
    --table.insert(app_func, require("test/gles"))
    --table.insert(app_func, require("test/math"))
    table.insert(app_func, require("test/cube"))
    
    return true
end

local function app_loop(dt)
    for i,f in ipairs(app_func) do
        if f then
            f(dt)
        end
    end
end

local function app_fail(msg)
    print(debug.traceback(msg))
end

vi_lua_main(app_init, app_loop, app_fail)







