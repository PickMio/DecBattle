/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ContextMgr.h"
#include "utils.h"
#include "NameService.h"
#include <assert.h>


int ContextMgr::HANDLEID = 0; 

static int 
traceback (lua_State *L) {
	const char *msg = lua_tostring(L, 1);
	if (msg)
		luaL_traceback(L, L, msg, 1);
	else {
		lua_pushliteral(L, "(no error message)");
	}
	return 1;
}

ContextMgr::ContextMgr(string sname)
{    
   this->scriptName = sname;    
   
    
}

ContextMgr::~ContextMgr()
{    
    delete m_Ctx->queue;
    delete this->m_Ctx;
    
}
int 
ContextMgr::Init()
{
    m_Ctx = new Context;
    m_Ctx->state = luaL_newstate();    
    m_Ctx->handle = ++HANDLEID;
    m_Ctx->queue = new MQueue(m_Ctx->handle);
    
    int ret = NameService::getInstance()->reg(m_Ctx->handle, this->scriptName);
    if( 0 != ret)
    {
        delete this;
        return ret;  //名字服务器发现重复, 不能使用
    }
    return this->loadScript();
    
}

int ContextMgr::loadScript()
{
    lua_State *L = this->getLuaState();
    lua_gc(L, LUA_GCSTOP, 0);
    luaL_openlibs(L);
    lua_pushlightuserdata(L, this->m_Ctx);
    lua_setfield(L, LUA_REGISTRYINDEX, "Context");
    //lua_pushcfunction(L, traceback);
    
    assert(lua_gettop(L) == 0);
    
    //可以在这加个loader, 用lua来控制读取的文件以及其他信息
    int r = luaL_dofile(L, this->scriptName.c_str());    
  
    
    if (r != LUA_OK) 
    {        
        const char *buff;
        buff = lua_tostring(L, -1);
        __log(_ERROR, __FILE__, __LINE__, __FUNCTION__, buff);
        //_LOG(buff, _ERROR);
        return -1;
    }

    __log(_DEBUG, __FILE__, __LINE__, __FUNCTION__, "Load file %s success!", this->scriptName.c_str());
    lua_gc(L, LUA_GCRESTART, 0);   
    
    return 0;
    
}
int
ContextMgr::getHandle()
{
    return this->m_Ctx->handle;   
    
}
lua_State*
ContextMgr::getLuaState()
{
    return this->m_Ctx->state;
    
}

int 
ContextMgr::call(int type, void *msg, int sz)
{
    if(NULL == this->m_Ctx->cb)
    {
        //_LOG("function not registered!", _ERROR);
        __log(_ERROR, __FILE__, __LINE__, __FUNCTION__, "function not registered!");
        return -1;
    }
    this->m_Ctx->cb(this->m_Ctx, type, msg, sz);
    
   
    __log(_DEBUG, __FILE__, __LINE__, __FUNCTION__, "Handle[%d] called!", this->getHandle());
    
    return 0;
    
}
