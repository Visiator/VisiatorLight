/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   GUIwindows.h
 * Author: smorodin
 *
 * Created on May 7, 2023, 1:51 PM
 */

#ifndef GUIWINDOWS_H
#define GUIWINDOWS_H

#include <vector>
#include <string>


#include "GUIwindow.h"
#include "GUIprimitives.h"

class GUI;

class GUIwindowList {
public:

    GUI *gui;
    
    std::vector<GUIwindow> window_list;
    
    GUIwindow* CreateWindow_(std::string name, GUIwindow::window_mode mode, RECTANGLE rectangle, GUIitems* gui_items, void* parent);
    
    GUIwindowList(GUI *gui);
    GUIwindowList(const GUIwindowList& orig) = delete;
    virtual ~GUIwindowList() {
        
    };
private:

};

#endif /* GUIWINDOWS_H */

