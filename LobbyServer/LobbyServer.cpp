/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "LobbyServer.h"
#include "utils.h"
#include "SockServer.h"
#include "GlobalQueue.h"
#include "Dispatch.h"

void 
LobbyServer::loadConfig()
{
    config = string("bootstrap.lua");
}
void 
LobbyServer::start()
{    
    __log(_WARN, __FILE__, __LINE__, __FUNCTION__, "------------- main LobbyServer started!-----");
     for(int c = 0; c < 1; c++)
        m_threads.spawn(dispatchMessage, NULL);
     
    sleep(3);
    this->loadConfig();
    ContextMap::getInstance()->newContext(this->config);
    this->runSockServer();
    
    //多线程处理逻辑
   
    
   m_threads.join();
    
}



void 
LobbyServer::runSockServer()
{
    Addr *addr = (Addr*) malloc(sizeof(Addr));
    memset(addr, 0, sizeof(Addr));
    const char *ip = "0.0.0.0";
    strncpy(addr->ip, ip, strlen(ip) + 1);
    addr->port = 10077;
    m_threads.spawn(sockServer, (void*)addr);
    
    
}

void 
LobbyServer::sockServer(void *argc)
{
    if(NULL == argc)
    {        
        __log(_ERROR, __FILE__, __LINE__, __FUNCTION__, "the argc is null, can not run sock server");
        
    }
    Addr *addr = (Addr *)argc;
    SockServer *svr = new SockServer();
    if(0 != svr->initServer(addr->ip, addr->port))
    {
        free(addr);        
        __log(_ERROR, __FILE__, __LINE__, __FUNCTION__, "Init Sock Server failed");
        return;
    }
    free(addr);
    
    __log(_DEBUG, __FILE__, __LINE__, __FUNCTION__, "Init sock server success");
    if(0 != svr->run())
    {       
        __log(_ERROR, __FILE__, __LINE__, __FUNCTION__, "run Server failed");
        return;
    }
    
}

void 
LobbyServer::dispatchMessage(void *)
{
    Dispatch dis;

    dis.dispatch();

    
}
