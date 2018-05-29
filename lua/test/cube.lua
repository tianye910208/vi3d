print("============test_cube============")


local vShaderStr = [[
    precision mediump float;
    attribute vec4 a_position;
    attribute vec4 a_color;
    varying vec4 v_color;
    uniform mat4 mvp;
    void main()                    
    {          
        v_color = a_color;
        gl_Position = mvp * a_position;
    }   
]]

local fShaderStr = [[
    precision mediump float;
    varying vec4 v_color;
    void main() 
    {               
        gl_FragColor = v_color;
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
local colorLocation = glGetAttribLocation(programObject, "a_color")
local mvpLocation = glGetUniformLocation(programObject, "mvp");

local vVertices = {
    -1.0,   -1.0,   1.0,    0.0,    1.0,    1.0,    0.0,    0.0,    1.0,  
    1.0,    -1.0,   1.0,    0.0,    1.0,    0.0,    1.0,    0.0,    1.0,  
    1.0,    1.0,    1.0,    1.0,    1.0,    0.0,    0.0,    1.0,    1.0,  
    -1.0,   1.0,    1.0,    1.0,    1.0,    1.0,    1.0,    0.0,    1.0,  
    -1.0,   -1.0,   -1.0,   0.0,    1.0,    0.0,    1.0,    1.0,    1.0,  
    -1.0,   1.0,    -1.0,   1.0,    1.0,    1.0,    0.0,    1.0,    1.0,  
    1.0,    1.0,    -1.0,   1.0,    1.0,    1.0,    1.0,    1.0,    1.0,  
    1.0,    -1.0,   -1.0,   1.0,    1.0,    0.0,    0.0,    0.0,    1.0,  
}

local vIndexes = {
    0, 1, 2, 0, 2, 3, 
    4, 5, 6, 4, 6, 7,
    5, 3, 2, 5, 2, 6, 
    4, 7, 1, 4, 1, 0, 
    7, 6, 2, 7, 2, 1, 
    4, 0, 3, 4, 3, 5  
}

local verticeObject = ""
for i,v in ipairs(vVertices) do
    verticeObject = verticeObject .. string.pack("<f", v)
end

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
  



local app = vi_app_info()

local m = mat4()
m:set_identity()

local q = vec4()
local r = 0
q:quat_set_euler(vec3(0, 0.1, 0))


--m:scale(vec3(0.1, 0.1, 0.1))
--m:translate(vec3(1,0,-10))


local v = mat4()
v:set_identity()

local s = vec4()
s:quat_set_euler(vec3(-math.pi/6, 0, 0))
v:rotate(s)
v:translate(vec3(0, 0, -20))

local p = mat4()
p:set_projection(45, app.viewport_w/app.viewport_h, 0.1, 100)


glClearColor(1.0, 1.0, 1.0, 0.0)
return function(dt)
    m:rotate(q)
    
	glViewport(app.viewport_x, app.viewport_y, app.viewport_w, app.viewport_h)
    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)

    glUseProgram(programObject)
    
    local mvp = p * v * m
    glUniformMatrix4fv(mvpLocation, 1, false, mvp)
    
    
    glEnable(GL_DEPTH_TEST)
    

    glBindBuffer(GL_ARRAY_BUFFER, vbo)
    glVertexAttribPointer(posLocation, 3, GL_FLOAT, false, 36, 0)
    glEnableVertexAttribArray(posLocation)
    glVertexAttribPointer(colorLocation, 4, GL_FLOAT, false, 36, 20)
    glEnableVertexAttribArray(colorLocation)


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo)
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0)
    
    glBindBuffer(GL_ARRAY_BUFFER, 0)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)
end
    



