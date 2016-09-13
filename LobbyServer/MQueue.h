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
using namespace std;
struct InerMsg_t;
class MQueue
{
public:
    MQueue(int s);
   
    
    //推入队列尾端 
    void push(InerMsg_t *msg);
    
    int32_t getService() const;
    
    //从队头弹出
    InerMsg_t* pop();
    
    //TODO 要添加当push进去后队列内数量的监控, 当数量超标时要通知是否有阻塞存在
    bool isBusy;//是否繁忙
    
   
protected:
    int32_t service;        //服务id
    std::queue<InerMsg_t *> m_MQ;
    
    
    
};




#endif /* MQUEUE_H */

