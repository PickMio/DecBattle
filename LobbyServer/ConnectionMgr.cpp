/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "ConnectionMgr.h"

uint32_t ConnectionMgr::HANDLER = 0;

ConnectionMgr::ConnectionMgr() 
{
    m_connCount = 0;
    
    // 构造函数中会调用初始化, 不用在这里处理 
//    for(uint32_t c = 0; c < MAX_SOCKET_COUNT; ++c)
//    {
//        m_connMap[c].reset();
//    }
}


ConnectionMgr::~ConnectionMgr() 
{
    for(int32_t c = 0;  c < MAX_SOCKET_COUNT; c++)
    {
        m_connMap[c].closeHandle();
    }
    m_connCount = 0;
    HANDLER = 0;
}

int32_t
ConnectionMgr::addConnection(Socket *s) 
{
    ++m_connCount;

    return 0;

}

/**
 * 
 * @param s
 *  表示服务器监听的端口
 * @return 
 */
Socket*
ConnectionMgr::acceptPeer(Socket* s) 
{
    if(NULL == s )
    {        
        __log(_ERROR , __FILE__, __LINE__, __FUNCTION__, "accept sock is null");
        return NULL;
    }
     if (m_connCount >= MAX_CLIENT_CONNECTION) 
    {       
        __log(_ERROR, __FILE__, __LINE__, __FUNCTION__, "connection is more than [%d]", MAX_CLIENT_CONNECTION);
        exit(0); //just for test to exit program pg
        return NULL;
    }
    int connfd = accept(s->getFD(), NULL, NULL);
    if (connfd <= 0) {        
        __log(_ERROR, __FILE__, __LINE__, __FUNCTION__, "accept client fail! return code [%d]", connfd);
        return NULL;
    }
    Socket *client = newSock(connfd, CONN_TYPE_CLIENT);
    
    int ret = addConnection(client);
    if(0 != ret)
    {
        return NULL;
    }
    return client;
}

int32_t
ConnectionMgr::connectPeer(const char* serverip, int32_t port) 
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr(serverip);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    int connfd = connect(sockfd, (struct sockaddr *) &addr, sizeof (struct sockaddr_in));
    if (-1 == connfd) 
    {        
        __log(_ERROR, __FILE__, __LINE__, __FUNCTION__, "connect to server fail! return code [%d]", connfd);
        return -1;
    }
    Socket *client = newSock(connfd, CONN_TYPE_SERVER);;
    
    return addConnection(client);

}

void
ConnectionMgr::disconnect(Socket *sock) 
{    
    if(NULL == sock)
    {        
        __log(_ERROR, __FILE__, __LINE__, __FUNCTION__, "disconnect sock is null");
        return;
    }
    Socket* s = getPeer(sock->getIdx());
    
    if(NULL == s)
    {        
        __log(_ERROR, __FILE__, __LINE__, __FUNCTION__, "disconnect s is null");
        return;
    }
    s->closeHandle();
    
    --m_connCount;
    
}

int32_t
ConnectionMgr::receiveMsg(Socket *s, BaseMsg *msg) 
{
    if(NULL == s)
    {        
        __log(_ERROR, __FILE__, __LINE__, __FUNCTION__, "socket is null");
        return -1;
    }
    Socket *client = getPeer(s->getIdx());
    if (NULL == client) 
    {//客户端没连接        
        __log(_ERROR, __FILE__, __LINE__, __FUNCTION__, "Client socket is not connected");
        return ERROR_TYPE_NULL_SOCKET;
    }
    return client->readHandle(msg);
   
   
}

int32_t
ConnectionMgr::sendMsg(Socket *s) 
{
    Socket *client = getPeer(s->getIdx());
    if (NULL == client) 
    {//客户端没连接
        //_LOG("Client socket is not connected!", _ERROR);
        __log(_ERROR, __FILE__, __LINE__, __FUNCTION__, "send failed Client socket is not connected!");
        return -1;
    }
    return client->writeHandle();
    
}
Socket* 
ConnectionMgr::newSock(int32_t fd, int32_t type)
{
    int idx = 0;
    for(;;)
    {//这里可能会形成死循环 
        idx = (++HANDLER) % MAX_SOCKET_COUNT;
        if(-1 == m_connMap[idx].getIdx())
        {
            m_connMap[idx].init(fd, HANDLER, type);
            return &m_connMap[idx];
        }
    }
    
}
Socket*
ConnectionMgr::getPeer(int32_t idx) 
{
    uint32_t uidx = idx % MAX_SOCKET_COUNT;
   
    return &m_connMap[uidx];

}

int32_t
ConnectionMgr::getConnectionCount()
{
    return this->m_connCount;
    
}
