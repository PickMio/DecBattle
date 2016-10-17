/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "LuaService.h"


int32_t
llaunch(lua_State *L)
{
    const char *file = luaL_checkstring(L, 1);
    const char *name = luaL_checkstring(L, 2);
    if(NULL == file)
    {        
        __log(_ERROR, __FILE__, __LINE__, __FUNCTION__, "get script file name error");
        lua_pushinteger(L, -1);
        return 1;
    }
    if(NULL == name)
    {        
        __log(_ERROR, __FILE__, __LINE__, __FUNCTION__, "get script file reg name error");
        lua_pushinteger(L, -1);
        return 1;
    }
    ContextMgr *mgr = ContextMap::getInstance()->newContext(static_cast<string>(file), static_cast<string>(name));
    if(NULL == mgr)
    {        
        __log(_ERROR, __FILE__, __LINE__, __FUNCTION__, "load script ctx fail");
        lua_pushinteger(L, -1);
        return 1;        
    }
    
    lua_pushinteger(L, 0);
    return 1;  
}


int32_t
lcall(lua_State *L)
{
    const char *svc = luaL_checkstring(L, 1);
    if(NULL == svc)
    {        
        __log(_ERROR, __FILE__, __LINE__, __FUNCTION__, "get script file name error");
        lua_pushinteger(L, -1);
        return 1;
    }
    ContextMgr *mgr = ContextMap::getInstance()->find(static_cast<string>(svc));
    if(NULL == mgr)
    {        
        __log(_ERROR, __FILE__, __LINE__, __FUNCTION__, "get script file name error");
        lua_pushinteger(L, -1);
        return 1;        
    }
    
    int32_t type = luaL_checkinteger(L, -3);
    const char *msg = luaL_checkstring(L, -2);
    int32_t sz = luaL_checkinteger(L, -1);
    
    //添加上\0
    char *smsg = (char*)malloc(sz + 1);
    
    memcpy(smsg, msg, sz + 1);
    
    int32_t ret = mgr->call(type, smsg, sz);
    
    free(smsg);
    
    
    
    lua_pushinteger(L, ret);
    return 1;  
}

int lsend(lua_State *L)
{
 
    const char *service = luaL_checkstring(L, 1);
    const char *source = luaL_checkstring(L, 2);
    int32_t type = luaL_checkinteger(L, 3);
    int32_t sz = luaL_checkinteger(L, 4);
    const char *msg = luaL_checkstring(L, 5);
    //reader.parse(str, root);
    
    InerMsg *m = new InerMsg;
    if(NULL == m)
    {
        _LOG(_ERROR, "malloc InerMsg fail");
        lua_pushinteger(L, -1);
        return 1;
        
    }
    
    m->sz = sz;
    m->type = type;
    m->service = service;
    m->source = source;
    m->msg = msg;
     
    _LOGX(_DEBUG,  "send msg sz[%d] type[%d] service[%s] source[%s]",
                    sz, type, m->service.c_str(), m->source.c_str());
     
    ContextMgr* ctx = ContextMap::getInstance()->find(m->service);
    if(NULL == ctx)
    {
        _LOGX(_ERROR,  "can not find service name[%s]!", m->service.c_str());
        return 1;
    }
    
    ctx->putMsg(m);
    
    return 1;
}


int lstart(lua_State *L)
{
    
    const char *ip = luaL_checkstring(L, 1);    
    int32_t port = luaL_checkinteger(L, 2);
    int fd = SockServer::getInstance()->getSendFD();
    RequestMsg request;
    int len;
    len = sizeof(request_start) + strlen(ip) + 1;
    request.u.start.port = port;
    memcpy(request.u.start.ip, ip, strlen(ip) + 1);
    request.header[6] = (uint8_t)'S';
    request.header[7] = (uint8_t)len;
    
    
    
	for (;;) {
        int n = write(fd, &request.header[6], len + 2);
        if (n < 0) {
            if (errno != EINTR) {
                fprintf(stderr, "socket-server : send ctrl command error %s.\n", strerror(errno));
            }
            continue;
        }
        
        return 0;
    }
    
    
    
    
    
    return 1;
}


int lexit(lua_State *L)
{
    exit(0);
    return 0;
}
extern "C" int32_t 
luaopen_lobbylib_core(lua_State *L)
{
    luaL_Reg l[] = {
        { "launch", llaunch }, 
        { "call", lcall }, 
        { "send", lsend }, 
        { "exit", lexit },
        { "start", lstart },
        
        { NULL, NULL },
    };
      
    luaL_newlibtable(L, l);         
    luaL_setfuncs(L, l, 0);
    return 1;
}
