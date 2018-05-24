print("============test_math============")
    
    
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


return function(dt) end



