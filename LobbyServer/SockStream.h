/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SockStream.h
 * Author: admin
 *
 * Created on 2016年9月5日, 上午9:42
 */

#ifndef SOCKSTREAM_H
#define SOCKSTREAM_H

#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include "GameDef.h"
#include "utils.h"

using namespace std;
const uint32_t MSG_SIZE_LENGTH = sizeof(MsgHeadDef);
const uint32_t STEP_READ_SIZE = 1;
const uint32_t STEP_READ_BODY = 2;
const uint32_t BUFF_LENGTH = 4096;

class SockStream
{
public:
    SockStream();
    int32_t reciveMsg(int32_t fd, PMsgBase &msg);
    int32_t sendMsg(int32_t fd, PMsgBase msg);
    int32_t reset();
    virtual ~SockStream();
protected:
    int32_t m_bodySize; //需要读取的包体长度
    int32_t m_readSize;   //已经读取的长度
    int32_t m_readStep;   //当前读取进度
    MsgHeadDef m_head;   //包头
    
    //这个buffer 要有65535 份, 太占内存, 可否改成接收到消息后再申请内存...性能损失多少
    char m_stream[BUFF_LENGTH];  //最大缓冲区, 8KB, 改成4KB
    vector <char> m_sendStreadm;  //发送缓冲区
};


#endif /* SOCKSTREAM_H */

