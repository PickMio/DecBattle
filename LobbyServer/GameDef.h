/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GameDef.h
 * Author: admin
 *
 * Created on 2016年9月1日, 下午2:58
 */


/**
 * 注意:
 *写头文件时, 和类紧密相关的数据结构写在类里面,
 * 方便查找, 而且不会出现引用交叉问题
 *该数据结构的相关处理最好只和这个类相关
 */
#ifndef GAMEDEF_H
#define GAMEDEF_H
#include "lua.hpp"
#include <stdint.h>
#include <string>
#include <vector>

using namespace std;
#define SAFEDEL(p) do{\
    if(NULL != p) {   \
        delete(p);    \
    }                 \
} while(0)

typedef struct  event_t {
	void * s; //每个event附带的信息
	bool read;  
	bool write;
}event;

typedef struct epoll_event EPOLL_EV; 


/**
 * MQueue的警报长度
 */

const int32_t MESSAGEQUEUE_WARN_LENGTH = 100;
const int32_t SERVICE_NAME_LENGTH = 128;
//messagequeue 的前置声明


/****
 * 客户端连接相关
 **/
const int32_t MAX_CLIENT_CONNECTION = 65530; //最大支持65535个连接, 越多新连接效率越低.可能需要紧缩空间---
const int32_t MAX_SOCKET_COUNT = 65532;  //先开辟65535个socket等待连接, 这个必须比MAX_CLIENT_CONNECTION大

const int32_t CONN_TYPE_NONE = 0;  //还未初始化, 或者只是保存信息
const int32_t CONN_TYPE_CLIENT = 1;  //和客户端相关的ConnectionMgr
const int32_t CONN_TYPE_SERVER = 2;  //和其他服务器相关的ConnectionMgr

const int32_t MAX_BACK_LOG = 128; //这个设置成5太小了, 连接消息都阻塞在这里了
const int32_t MAX_EVENTS = 64;



/***
 *  常见错误
 *  -0x01
 */

//不可忽视的错误, 要down掉服务
const int32_t ERROR_TYPE_STOP_SERVER = -0x010001;
const int32_t ERROR_TYPE_EPOOL_WAIT_FAIL = -0x010002;
const int32_t ERROR_TYPE_NULL_SOCKET = -0x010003;
const int32_t ERROR_TYPE_MALLOC_FAIL = -0x010004;
const int32_t ERROR_TYPE_ADDCTL_FAIL = -0x010005;
const int32_t ERROR_TYPE_READNUM_FAIL = -0x010006;
const int32_t ERROR_TYPE_NONE_THREAD = -0x010007;
const int32_t ERROR_TYPE_BODY_OVER_FLOW = -0x010008;
const int32_t ERROR_TYPE_TOO_MANY_CONNECTOIN = -0x010008;
// SockServer::isListener 返回, 是监听者但在监听队列找不到
const int32_t ERROR_TYPE_NOT_LISTEN_NOR_ACCEPT = -0x010009;


/*******
 * socket服务器传递消息
 */
const int32_t MSG_TYPE_DISCONNECT = 0x010001;  //客户端断开连接
const int32_t MSG_TYPE_MORE = 0x010002;        //数据没有读取完整

/****
 * 接收到的外部消息
 */
typedef struct BaseMsg_t
{
    uint32_t sz;
    void *msg;
    
}BaseMsg;

/***
 *  服务器内部消息
 */
const int32_t MSG_TYPE_INNER = 1;//内部消息 
const int32_t MSG_TYPE_NET = 2;  //转发的网络消息


typedef struct Addr_t
{
    char ip[64];
    int32_t port;
    int32_t fd[2];//管道
}Addr;

//服务名称
const int MAX_SERVER_NAME_LEN = 128;


typedef struct request_open_t
{
    
}request_open;


typedef struct request_send_t
{
    int32_t sz;
    void *msg;
}request_send;

typedef struct request_start_t
{
    int32_t port;
    char    ip[0];
}request_start;


typedef struct request_close_t
{
    int32_t idx;
}request_close;

/*
	The first byte is TYPE

	S Start socket
	B Bind socket
	L Listen socket
	K Close socket
	O Connect to (Open)
	X Exit
	D Send package (high)
	P Send package (low)
	A Send UDP package
	T Set opt
	U Create UDP socket
	C set udp address
 */
typedef struct RequestMsg_t
{
    uint8_t header[8];
    union{
       char buffer[256];
       request_open open;
       request_close close; //'K'
       request_send  send;
       request_start start;
    } u;
    
}RequestMsg;


/*具体游戏相关的协议*/


//int m_AlignmentByte[4];  //对齐值


static const unsigned char IDENTIFY_VER = 0x05;
static const unsigned char MESSAGE_VER  = 0x03;

static const unsigned char ENCODE_NONE  = 0x00;
static const unsigned char ENCODE_AES   = 0x01;

static const unsigned short NM_KEEP_ALIVE = 0xF0F1;


typedef struct MsgHeadDef
{
    unsigned char  identity;
    unsigned char  encode;
    unsigned short length;
    unsigned char  version;
    unsigned char  reserve;
    unsigned short cMsgType;
}MsgHeadDef, *PMsgHeader;

//typedef struct MsgBase
class MsgBase
{
public:
    MsgHeadDef head;
    vector<char> msg;
    //void*      msg;
    
};
const int32_t MSG_SIZE_LENGTH = sizeof(MsgHeadDef);

const int32_t TIME_OUT_SECONDS = 20; // 多少秒处理定时器, 在主线程处理(主要是踢掉长时间不在线的角色,没有发送 keepalive))
const int32_t TIME_OUT = TIME_OUT_SECONDS * 1000;
const int32_t MAX_TIMEOUT_KICK = 2;  //连续多久没收到消息关闭连接
#endif /* GAMEDEF_H */
