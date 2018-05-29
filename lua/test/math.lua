print("============test_math============")
    
--[[
local a = vec2()
a:set(1,2)
print(a, a:get(), a.x, a.y)
local b = vec2()
b:set(3,4)
print(b)
local c = a + b
print(c)
local d = a * 10
print(d, a)
print(#b)
print(a:dot(b))

a.x = 3
a.y = 4
a:normalize()
print(a)


local m = mat4()
m:set_identity()
m.m01 = 0.5
m.m20 = 0.5
print(m)
m:transpose()
local s = mat4(m)
print(m)
m:inverse()
print(m)

print(m*s)

m:set_identity()
print(m)
local v = vec3()
v:set(100,200,300)
m:translate(v)
print(m)
local pos = vec3()
pos:set(10,20,30)
print(pos*m)

--]]

local app = vi_app_info()

local p = mat4()
p:set_identity()
p:set_projection(0, app.viewport_w/app.viewport_h, 0.1, 100)
print(p)



local m = mat4()
m:set_identity()

local q = vec4()
q:quat_set_euler(vec3(0,45,0))
--m:rotate(q)

--m:scale(vec3(0.1, 0.1, 0.1))
m:translate(vec3(0,0,-10))

p = p * m

local v1 = vec4(-1, 0, -50, 1) * p
local v2 = vec4(1, 0, -50, 1) * p
local v3 = vec4(0, -1, -50, 1) * p
local v4 = vec4(0, 1, -50, 1) * p

print(v1 * (1/v1.w))
print(v2 * (1/v2.w))
print(v3 * (1/v3.w))
print(v4 * (1/v4.w))

return function(dt) end



