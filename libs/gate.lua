local svc = require("service")
local player = require("player")
local utils = require("utils")
local cjson = require("cjson")

local gate = {}
local handler = {}
local users = {}

gate._F = "gate"

-- �������û�, 
-- ����
-- argc.fd, �û�fd
-- argc.userid, �û� userid
function handler.accept(argc)
	p = player.new(argc.fd, argc.userid)
	users[p.userid] = p
end

-- ����û���Ϣ
-- argc.userid �û�userid
-- argc ע���û�������Ϣ
function handler.add(argc)
	p = users[argc.userid]	
	p:init(argc)
end

-- ��ʾ�û���Ϣ
-- argc.userid ��Ҫ��ʾ���û�id
function handler.show(argc)
	p = users[argc.userid]	
	p:show()
end

-- �����������Ļص�����
svc.register(function(typeid, msg, size)
	tmsg = cjson.decode(msg)
	f = handler[tmsg.cmd]
	if nil ~= f then
		f(tmsg)
	end
end)













return gate;
