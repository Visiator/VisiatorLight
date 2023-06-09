/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   GUIwindows.cpp
 * Author: smorodin
 * 
 * Created on May 7, 2023, 1:51 PM
 */

#include <string>

#include "GUIwindowList.h"

GUIwindowList::GUIwindowList(GUI *gui) : gui(gui) {
    
}

GUIwindow* GUIwindowList::CreateWindow_(std::string name, GUIwindow::window_mode mode, RECTANGLE rectangle, GUIitems* gui_items, void* parent) {
    
    for(const auto& w : window_list) {
        if(w.window->name == name) {
            return nullptr;
        }
    }
    
    window_list.push_back({11, gui, name, mode, rectangle, gui_items, parent});//, name, mode, rectangle, gui_items, parent}); 
    
    for(unsigned int i = 0; i < window_list.size(); i++) {
        printf("[%d] WL screen = %s\n", i, window_list[i].window->screen == NULL ? "NULL" : "OK");
        
        if(window_list[i].window->name == name) {
            window_list[i].window->run();
            return window_list[i].window.get();
        }
    }
    
    return nullptr;
}