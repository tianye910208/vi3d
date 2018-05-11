

local escape_tbl_ctl = { ["\\"] = "\\\\" , ["\n"] = "\\n", ["\r"] = "\\r" }
local escape_tbl = setmetatable({
	['&amp;'] = '&',
	['&quot;'] = '"',
    ['&apos;'] = "'",
	['&lt;'] = '<',
	['&gt;'] = '>',
}, 
{ 
    __index = function(_,k) 
        local c = string.match(k, "&#(%d+);")
        return c and string.char(c) or k
    end 
})
local function xml_value(s)
    if s == nil then
        return ""
    end
	s = s:gsub("(&[^;]+;)", escape_tbl)
	s = s:gsub("[\\\r\n]", escape_tbl_ctl)
	return s
end

local function xml_child(p, node)
    local list = p[node.label]
    if not list then
        list = {}
        p[node.label] = list
    end
    list[#list+1] = node
end

local function xml_args(node, s)
    string.gsub(s, "(%w+) *= *([\"'])(.-)%2", function(w, _, a)
        node["@"..w] = xml_value(a)
    end)
    return node
end

function loadxml(xmlText)
    local stack = {}
    local top = {}
    table.insert(stack, top)
    local ni, c, label, xarg, empty
    local i, j = 1, 1
    while true do
        ni, j, c, label, xarg, empty = string.find(xmlText, "<(%/?)([%w_:]+)(.-)(%/?)>", i)
        if not ni then break end
        local text = string.sub(xmlText, i, ni - 1);
        if not string.find(text, "^%s*$") then
            top.value = (top.value or "") .. xml_value(text)
        end

        if empty == "/" then -- empty element tag
            xml_child(top, xml_args({label = label}, xarg))
        elseif c == "" then -- start tag
            top = xml_args({label = label}, xarg)
            table.insert(stack, top)
        else -- end tag
            local toclose = table.remove(stack) -- remove top

            top = stack[#stack]
            if #stack < 1 then
                error("XmlParser: nothing to close with " .. label)
            end
            if toclose.label ~= label then
                error("XmlParser: trying to close " .. toclose.label .. " with " .. label)
            end
            xml_child(top, toclose)
        end
        i = j + 1
    end
    local text = string.sub(xmlText, i);
    if #stack > 1 then
        error("XmlParser: unclosed " .. stack[#stack]:name())
    end
    return top
end


