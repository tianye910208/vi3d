print("----------math------------")

local _vec2 = vec2_metatable()
_vec2.__add = vec2_add
_vec2.__sub = vec2_sub
_vec2.__mul = vec2_mul
_vec2.__len = vec2_len
_vec2.__tostring = vec2_tostring

local _vec2_f = {
    get = vec2_get,
    set = vec2_set,
    geti = vec2_geti,
    seti = vec2_seti,
    
    add = vec2_add,
    sub = vec2_sub,
    mul = vec2_mul,
    dot = vec2_dot,
    len = vec2_len,
    normalize = vec2_normalize,
}

local _vec2_v = {
    x = 1,
    y = 2,
    u = 1, 
    v = 2,
}

_vec2.__index = function(u, k)
    local f = _vec2_f[k]
    if f then
        return f
    end
    local i = _vec2_v[k]
    if i then
        return vec2_geti(u, i)
    end
    
    error("[vec2]get", k)
end

_vec2.__newindex = function(u, k, v)
    local i = _vec2_v[k]
    if i then
        vec2_seti(u, i, v)
    else
        error("[vec2]set", k, v)
    end
end

vec2 = vec2_new


