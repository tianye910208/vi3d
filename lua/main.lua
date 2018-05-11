print("Hello Lua World!!!")


function string.split(input, delimiter)
    input = tostring(input)
    delimiter = tostring(delimiter)
    if (delimiter=='') then return false end
    local pos,arr = 0, {}
    -- for each divider found
    for st,sp in function() return string.find(input, delimiter, pos, true) end do
        table.insert(arr, string.sub(input, pos, st - 1))
        pos = sp + 1
    end
    table.insert(arr, string.sub(input, pos))
    return arr
end



local vShaderStr = [[
    #version 300 es
    layout(location = 0) in vec4 vPosition;
    void main()                    
    {            
       gl_Position = vPosition;       
    }   
]]

local fShaderStr = [[
    #version 300 es   
    precision mediump float;  
    out vec4 fragColor; 
    void main() 
    {               
       fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }  
]]


function LoadShader(shaderType, shaderSrc)
    local shader = glCreateShader(shaderType)
    if shader == 0 then
        return 0
    end
    
    glShaderSource(shader, shaderSrc)
    glCompileShader(shader)

    if glGetShaderiv(shader, GL_COMPILE_STATUS) == 0  then
        local infoLen = glGetShaderiv(shader, GL_INFO_LOG_LENGTH)
        if infoLen > 1 then
            local len, log = glGetShaderInfoLog(shader, infoLen)
            print("[Error] compiling shader:", log)
        end
        glDeleteShader(shader)
        return 0
    else
        return shader
    end
end

local programObject = 0
local verticeObject = ""
local indexesObject = ""


function app_init()
	vi_app_set_design_size(1280, 720)
    
    local app = vi_app_info()
    
    package.searchers[4] = nil
    package.searchers[3] = nil
    package.searchers[2] = function(filename)
        local p = vi_file_open(app.data_path.."lua/"..filename..".lua", "r")
        local s = vi_file_read(p, vi_file_size(p))
        vi_file_close(p)
        
        print("file_read:", s, p)
        
        local f,err = load(s, filename)
        return f or err
    end

    require("gles")
    
    
    local extstr = glGetString(GL_EXTENSIONS)
    for i,v in ipairs(string.split(extstr, " ")) do
        print(i, v) 
    end

    local vs = LoadShader(GL_VERTEX_SHADER, vShaderStr)
    local fs = LoadShader(GL_FRAGMENT_SHADER, fShaderStr)

    programObject = glCreateProgram()
    if programObject == 0 then
        print("Error glCreateProgram")
        return false
    end
    
    
    glAttachShader(programObject, vs)
    glAttachShader(programObject, fs)
    glLinkProgram(programObject)

    if glGetProgramiv(programObject, GL_LINK_STATUS) == 0 then
        local infoLen = glGetProgramiv(programObject, GL_INFO_LOG_LENGTH)
        if infoLen > 1 then
            local len, log = glGetProgramInfoLog(programObject, infoLen)
            print("Error linking program:", log)
        end
        glDeleteProgram(programObject)
        return false
    end

    glClearColor(1.0, 1.0, 1.0, 0.0)
    
    
    local vVertices = {
        0.0, 0.5, 0.0,
        -0.5, -0.5, 0.0,
        0.5, -0.5, 0.0
    }
    for i,v in ipairs(vVertices) do
        verticeObject = verticeObject .. string.pack("<f", v)
    end
    
    local vIndexes = {
        0,1,2
    }
    for i,v in ipairs(vIndexes) do
        indexesObject = indexesObject .. string.pack("<I2", v)
    end
    
    --test table input and output
    local n = 10
    local t = glGenBuffers(n)
    local p = ""
    for i,v in ipairs(t) do
        print("buffer", i, v)
        p = p..string.pack("<I4", v)
    end
    
    glDeleteBuffers(n, p)
    
    return true
end

function app_update(dt)
  
end

function app_render(dt)
    local app = vi_app_info()
	glViewport(app.viewport_x, app.viewport_y, app.viewport_w, app.viewport_h)
    
    glClear(GL_COLOR_BUFFER_BIT)

    glUseProgram(programObject)

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, verticeObject)
    glEnableVertexAttribArray(0)

    --glDrawArrays(GL_TRIANGLES, 0, 3) 
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, indexesObject)
end

function app_onerror(msg)
    print(debug.traceback(msg))

end


xpcall(function()
    if app_init() then
        print("[app_init]ok")
        vi_lua_set_func(app_update, app_render, app_onerror)
    end
end, app_onerror)



