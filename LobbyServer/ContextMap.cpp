/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ContextMap.h"
#include "NameService.h"

ContextMap *ContextMap::m_ins = NULL;

ContextMgr* ContextMap::newContext(string scriptName)
{
    ContextMgr *ctx = new ContextMgr(scriptName);
    
    int ret = ctx->Init();
    if(0 != ret)
    {        
        __log(_ERROR, __FILE__, __LINE__,__FUNCTION__,  "context init fail!");
        return NULL;
        
    }
    m_CtxMap.insert(make_pair(ctx->getHandle(), ctx));
    return ctx;
    
    
}

ContextMap* ContextMap::getInstance()
{
    if(NULL == m_ins)
    {
        m_ins = new ContextMap();
    }
    return m_ins;
    
}

ContextMap::ContextMap()
{
    
}

ContextMgr* ContextMap::find(string scriptName)
{
    ContextMgr *ctx = NULL;
    int service = NameService::getInstance()->search(scriptName);
    if(service < 0 )
    {        
        __log(_ERROR, __FILE__, __LINE__,__FUNCTION__, "can not find service!");
        return ctx;
    }
    map<int, ContextMgr*>::iterator iter = m_CtxMap.find(service);
    if(iter != m_CtxMap.end())
    {
        ctx = iter->second;
    }
    return ctx;    
    
}
