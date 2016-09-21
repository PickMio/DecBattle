/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "Dispatch.h"

Dispatch::Dispatch()
{
    
}
Dispatch::~Dispatch()
{
    
    
}

int32_t 
Dispatch::dispatch()
{
    MQueue *q = NULL;
    for(;;)
    {
        q = dealMsg(q);
        if(NULL == q)
        {
            GlobalQueue::getInstance()->lockQ();
            //没消息来时, 会阻塞在这
            GlobalQueue::getInstance()->waitQ();            
            GlobalQueue::getInstance()->unlockQ();
        }        
        
    }
    return 0;
}
MQueue* 
Dispatch::dealMsg(MQueue* q)
{
    if(NULL == q)
    {
         q = GlobalQueue::getInstance()->pop();
    }
    if(NULL == q)
    {
        return q;
    }
    
    InerMsg* msg = q->pop();
    ContextMgr *ctxMgr = ContextMap::getInstance()->find("test.lua");
    __log(_WARN, __FILE__, __LINE__, __FUNCTION__, "I am get msg");
    
    ctxMgr->call(msg->type, msg->msg, msg->sz);    
    SAFEDEL(msg);
    return GlobalQueue::getInstance()->pop();
}