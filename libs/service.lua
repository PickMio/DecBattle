local load = load
local ipairs = ipairs
local table = table
local utils = require("utils")
local snlua = require("lobbylib.snlua")
local core = require("lobbylib.core")
local cjson = require("cjson")


local utils = {}
local service = {}
service._F = "service.lua"

-- ע��ص�����
-- fun �ص�����
-- ������������Ҫ���� integer type, string msg, integer size
function service.register(fun)
	snlua.callback(fun)
end 

-- ���ûص�
-- sname service name
-- type ����
-- msg ����table, ����������json
function service.call(sname, typeid, msg)
	encodeMsg = cjson.encode(msg)
	ret = core.call(sname, typeid, encodeMsg, #encodeMsg)
	return ret
end 


return service
