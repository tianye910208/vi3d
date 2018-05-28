print("============test_gles============")


local vShaderStr = [[
    attribute vec4 a_position;
    void main()                    
    {            
       gl_Position = a_position;       
    }   
]]

local fShaderStr = [[
    void main() 
    {               
       gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
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


local vs = LoadShader(GL_VERTEX_SHADER, vShaderStr)
local fs = LoadShader(GL_FRAGMENT_SHADER, fShaderStr)

local programObject = glCreateProgram()
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

local posLocation = glGetAttribLocation(programObject, "a_position")


local vVertices = {
    0.0, 0.5, 0.0,
    -0.5, -0.5, 0.0,
    0.5, -0.5, 0.0
}
local verticeObject = ""
for i,v in ipairs(vVertices) do
    verticeObject = verticeObject .. string.pack("<f", v)
end

local vIndexes = {
    0,1,2
}
local indexesObject = ""
for i,v in ipairs(vIndexes) do
    indexesObject = indexesObject .. string.pack("<I2", v)
end

local buffer = glGenBuffers(2)

local vbo = buffer[1]
glBindBuffer(GL_ARRAY_BUFFER, vbo)
glBufferData(GL_ARRAY_BUFFER, #vVertices*4, verticeObject, GL_STATIC_DRAW)
glBindBuffer(GL_ARRAY_BUFFER, 0)


local ibo = buffer[2]
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo)
glBufferData(GL_ELEMENT_ARRAY_BUFFER, #vIndexes*2, indexesObject, GL_STATIC_DRAW)
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)
  



glClearColor(1.0, 1.0, 1.0, 0.0)

local app = vi_app_info()
return function(dt)
    
	glViewport(app.viewport_x, app.viewport_y, app.viewport_w, app.viewport_h)
    
    glClear(GL_COLOR_BUFFER_BIT)

    glUseProgram(programObject)

    glBindBuffer(GL_ARRAY_BUFFER, vbo)
    
    glVertexAttribPointer(posLocation, 3, GL_FLOAT, false, 12, 0)
    glEnableVertexAttribArray(posLocation)

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo)
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0)
    
    glBindBuffer(GL_ARRAY_BUFFER, 0)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)
end
    



