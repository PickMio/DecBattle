/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "LobbyServer.h"
#include "utils.h"
#include "SockServer.h"

void 
LobbyServer::loadConfig()
{
    config = string("bootstrap.lua");
}
void 
LobbyServer::start()
{
    //_LOG("------------- main LobbyServer started!-----", _WARN);
    __log(_WARN, __FILE__, __LINE__, __FUNCTION__, "------------- main LobbyServer started!-----");
    
    this->loadConfig();
    ContextMap::getInstance()->newContext(this->config);
    this->runSockServer();
    
    //多线程处理逻辑
   // for(int c = 0; c < 5; c++)
   //     m_threads.spawn(dispatchMessage, NULL);
     
      
    
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
        //_LOG("the argc is null, can not run sock server", _ERROR);
        __log(_ERROR, __FILE__, __LINE__, __FUNCTION__, "the argc is null, can not run sock server");
        
    }
    Addr *addr = (Addr *)argc;
    SockServer *svr = new SockServer();
    if(0 != svr->initServer(addr->ip, addr->port))
    {
        free(addr);
        //_LOG("Init Server failed", _ERROR);
        __log(_ERROR, __FILE__, __LINE__, __FUNCTION__, "Init Sock Server failed");
        return;
    }
    free(addr);
    //_LOG("Init sock server success", _DEBUG);
    __log(_DEBUG, __FILE__, __LINE__, __FUNCTION__, "Init sock server success");
    if(0 != svr->run())
    {
       // _LOG("run Server failed", _ERROR);
        __log(_ERROR, __FILE__, __LINE__, __FUNCTION__, "run Server failed");
        return;
    }
    
}

void 
LobbyServer::dispatchMessage(void *)
{
    for(;;)
    {
        sleep(2);
    }
    
}
