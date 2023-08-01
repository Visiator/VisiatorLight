/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/main.cc to edit this template
 */

/* 
 * File:   main.cpp
 * Author: smorodin
 *
 * Created on May 1, 2023, 2:33 PM
 */

#ifdef _WIN32

#define _WINSOCKAPI_ 
#include <windows.h>
#undef _WINSOCKAPI_
#include <winsock2.h>
#include <stdlib.h>
#include <iphlpapi.h>
#include <stdio.h>
#undef _WINSOCKAPI_

#endif

#include <memory>
#include <stdio.h>
#include <cstdlib>


#include "NET/NET.h"
#include "GUI/GUIlowlevel.h"
#include "GUI/GUI.h"
#include "ClientManager/ClientManager.h"
#include "Viewer.h"


std::unique_ptr<ClientManager> g_ClientManager;
std::unique_ptr<Viewer> g_Viewer;


#ifdef __linux__
void _start_();
#endif

#ifdef _WIN32
void _start_(HINSTANCE hInstance);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    printf("start WinMain+\n");

    _start_(hInstance);
    
    printf("WinMain stop\n");

    return 0;
}
#endif

int g_1 = 0;

#ifdef __linux__

//void *SigCatcher(int n)
//{
//  wait3(NULL,WNOHANG,NULL);
//}



int main(int argc, char** argv) {
    
    //boost::array<int, 4> arr = {{1,2,3,4}};
    
    //signal(SIGCHLD,SigCatcher);
    
    printf("main\n");
    
    _start_();
    printf("main stop\n");
}
void _start_() {
#endif
    
#ifdef _WIN32
void _start_(HINSTANCE hInstance) {
#endif    
    printf("start+\n");

    std::shared_ptr<GUI> gui = std::make_shared<GUI>();
    
#ifdef _WIN32
    gui->hInstance = hInstance;
#endif    
    //g_Net = std::make_unique<NET>();
    g_ClientManager = std::make_unique<ClientManager>(gui);
    g_Viewer = std::make_unique<Viewer>(gui);
    
    g_ClientManager->run();
    //clientManager.reset();
    
    g_Viewer->start();
    
    //g_ClientManager->wait();
    //g_Viewer->wait();
    
    //g_ClientManager->stop();
    //g_Viewer->stop();
    
    
    gui->window_list->wait();
    //g_Net->set_need_stop();
    //g_Net->wait_stop();
    printf("stop\n");
    return;
}

