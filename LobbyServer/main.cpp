/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: admin
 *
 * Created on 2016年9月1日, 上午11:29
 */

#include <cstdlib>
#include <dlfcn.h>

#include "LobbyServer.h"


using namespace std;

/*
 * 
 */
int main(int argc, char** argv) 
{
    LobbyServer s;
    s.start();
    
    
    return 0;
}

