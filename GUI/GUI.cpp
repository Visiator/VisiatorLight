/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   GUI.cpp
 * Author: smorodin
 * 
 * Created on May 1, 2023, 2:46 PM
 */

#ifdef __linux__
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/extensions/XTest.h>
#endif

#include <string>

#include "GUI.h"
#include "GUIprimitives.h"
#include "../tools.h"

GUI::GUI() {
    printf("GUI constructor1\n");

#ifdef __linux__
    //if ((linux.display_ = XOpenDisplay(getenv("DISPLAY"))) == NULL) {
    //    wtf("Can't connect X server");//, strerror(errno));
    //    return;
    //}
    //linux.screen_id = XDefaultScreen(linux.display_);
#endif
    
     window_list = std::make_unique<GUIwindowList>(this);
    
}

GUI::~GUI() {
    
    printf("GUI destructor\n");
#ifdef __linux__
    //XCloseDisplay(linux.display_);
#endif
}

void GUI::test() {
    
    //execute_is_run = true;
    //execute_thread = new std::thread(&GUIwindow::execute, this);
    
    printf("GUI test\n");
}

GUIwindow* GUI::CreateWindow_(std::string name, GUIwindow::window_mode mode, RECTANGLE rectangle, GUIitems* gui_items, void* parent) {
    
    return window_list->CreateWindow_(name, mode, rectangle, gui_items, parent);
}

