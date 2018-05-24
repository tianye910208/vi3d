print("----------math------------")

local rawget = rawget
local gen_get_func = function(desc, geti, vmap, fmap)
    return function(u, k)
        local f = rawget(fmap, k)
        if f then
            return f
        end
        local i = rawget(vmap, k)
        if i then
            return geti(u, i)
        end
        
        error(desc, k)
    end
end

local gen_set_func = function(desc, seti, vmap)
    return function(u, k, v)
        local i = rawget(vmap, k)
        if i then
            seti(u, i, v)
        else
            error(desc, k, v)
        end
    end
end



vec2 = vec2_new

local _vec2_f = {
    get = vec2_get,
    set = vec2_set,
    geti = vec2_geti,
    seti = vec2_seti,
    
    add = vec2_add,
    sub = vec2_sub,
    mul = vec2_mul,
    len = vec2_len,
    dot = vec2_dot,
    normalize = vec2_normalize,
}

local _vec2_v = {
    x = 1,
    y = 2,
    u = 1, 
    v = 2,
}

local _vec2 = vec2_meta()
_vec2.__add = vec2_add
_vec2.__sub = vec2_sub
_vec2.__mul = vec2_mul
_vec2.__len = vec2_len
_vec2.__tostring = vec2_tostring
_vec2.__index = gen_get_func("[vec2]get", vec2_geti, _vec2_v, _vec2_f)
_vec2.__newindex = gen_set_func("[vec2]set", vec2_seti, _vec2_v)



vec3 = vec3_new

local _vec3_f = {
    get = vec3_get,
    set = vec3_set,
    geti = vec3_geti,
    seti = vec3_seti,
    
    add = vec3_add,
    sub = vec3_sub,
    mul = vec3_mul,
    len = vec3_len,
    dot = vec3_dot,
    cross = vec3_cross,
    normalize = vec3_normalize,
}

local _vec3_v = {
    x = 1,
    y = 2,
    z = 3,
    r = 1,
    g = 2,
    b = 3,
}

local _vec3 = vec3_meta()
_vec3.__add = vec3_add
_vec3.__sub = vec3_sub
_vec3.__mul = vec3_mul
_vec3.__len = vec3_len
_vec3.__tostring = vec3_tostring
_vec3.__index = gen_get_func("[vec3]get", vec3_geti, _vec3_v, _vec3_f)
_vec3.__newindex = gen_set_func("[vec3]set", vec3_seti, _vec3_v)




vec4 = vec4_new

local _vec4_f = {
    get = vec4_get,
    set = vec4_set,
    geti = vec4_geti,
    seti = vec4_seti,
    
    add = vec4_add,
    sub = vec4_sub,
    mul = vec4_mul,
    len = vec4_len,
    normalize = vec4_normalize,
    
    quat_mul = vec4_quat_mul,
    quat_rot = vec4_quat_rot,
    quat_nlerp = vec4_quat_nlerp,
    quat_slerp = vec4_quat_slerp,
    quat_set_euler = vec4_quat_set_euler,
    quat_set_axis_angle = vec4_quat_set_axis_angle,
}

local _vec4_v = {
    x = 1,
    y = 2,
    z = 3,
    w = 4,
    r = 1,
    g = 2,
    b = 3,
    a = 4,
    u = 1,
    v = 2,
    s = 3,
    t = 4,
}

local _vec4 = vec4_meta()
_vec4.__add = vec4_add
_vec4.__sub = vec4_sub
_vec4.__mul = vec4_mul
_vec4.__len = vec4_len
_vec4.__tostring = vec4_tostring
_vec4.__index = gen_get_func("[vec4]get", vec4_geti, _vec4_v, _vec4_f)
_vec4.__newindex = gen_set_func("[vec4]set", vec4_seti, _vec4_v)



mat4 = mat4_new

local _mat4_f = {
    get = vec4_get,
    set = vec4_set,
    geti = vec4_geti,
    seti = vec4_seti,
    
    mul = vec4_mul,
    
    set_identity = mat4_set_identity,
	set_zero = mat4_set_zero,
	set_quat = mat4_set_quat,
    
	translate = mat4_translate,
	rotate = mat4_rotate,
	scale = mat4_scale,
    
	inverse = mat4_inverse,
	transpose = mat4_transpose,
}

local _mat4_v = {
    m00 = 1,
    m01 = 2,
    m02 = 3,
    m03 = 4,
    m10 = 5,
    m11 = 6,
    m12 = 7,
    m13 = 8,
    m20 = 9,
    m21 = 10,
    m22 = 11,
    m23 = 12,
    m30 = 13,
    m31 = 14,
    m32 = 15,
    m33 = 16,
}

local _mat4 = mat4_meta()
_mat4.__mul = mat4_mul
_mat4.__tostring = mat4_tostring
_mat4.__index = gen_get_func("[mat4]get", mat4_geti, _mat4_v, _mat4_f)
_mat4.__newindex = gen_set_func("[mat4]set", mat4_seti, _mat4_v)

















