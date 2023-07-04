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

#include "GUI/GUIlowlevel.h"
#include "GUI/GUI.h"
#include "ClientManager/ClientManager.h"
#include "Viewer.h"

std::unique_ptr<ClientManager> clientManager;
std::unique_ptr<Viewer> viewer;

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



#ifdef __linux__
int main(int argc, char** argv) {
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
    clientManager = std::make_unique<ClientManager>(gui);
    viewer = std::make_unique<Viewer>(gui);
    
    clientManager->run();
    //clientManager.reset();
    
    viewer->run();
    
    clientManager->wait();
    
    clientManager->stop();
    
    //gui.reset();
    
    printf("stop\n");
    return;
}

