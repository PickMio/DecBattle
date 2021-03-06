/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MQueue.h
 * Author: admin
 *
 * Created on 2016年9月1日, 下午2:57
 */

#ifndef MQUEUE_H
#define MQUEUE_H
#include <queue>
#include "GameDef.h"
#include "utils.h"
#include "GlobalQueue.h"
#include "MutexLock.h"
#include "InerMsg.h"
using namespace std;

class MQueue
{
public:
    MQueue(int s, string sname);
   
    
    //推入队列尾端 
    void push(InerMsg *msg);
    
    int32_t getService() const;
    
    //从队头弹出
    InerMsg* pop();
    void setOutGlobal();
    
    //TODO 要添加当push进去后队列内数量的监控, 当数量超标时要通知是否有阻塞存在
    bool m_isBusy;//是否繁忙
    
   
protected:
    int32_t m_service;        //服务id
    string m_svcName;
    std::queue<InerMsg *> m_MQ;
    bool m_inGlobal; //是否在全局队列中
    MutexLock m_lock;
    
    
    
};




#endif /* MQUEUE_H */

