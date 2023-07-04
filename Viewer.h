/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   Viewer.h
 * Author: smorodin
 *
 * Created on July 4, 2023, 2:08 PM
 */

#ifndef VIEWER_H
#define VIEWER_H

#include <memory>
#include <stdio.h>
#include <cstdlib>

#include "GUI/GUI.h"
#include "GUI/GUIlowlevel.h"
#include "GUI/GUIitem.h"

class Viewer {
public:
    
    std::shared_ptr<GUI> gui;
    GUIwindow* window;
    bool need_stop;
    
    GUIitems gui_items;
    
    void run();
    
    bool InitGUIitems();
    
    static void StaticUserInput(void* parent, WindowUserEvent* window_user_event) {
        printf("StaticUserInput\n");
        ((Viewer *)parent)->UserInput(window_user_event);   
    }
    void all_edit_begin_false() {
        for(int i=0; i<gui_items.items.size(); i++) {
            if(gui_items.items[i].is_edit_begin) {
                gui_items.items[i].is_edit_begin = false;
            }
        }
    }
    void UserInput(WindowUserEvent* window_user_event) {
        printf("UserInput %d %d \n", window_user_event->x, window_user_event->y);
        
        
    }
    
    
    Viewer(std::shared_ptr<GUI> gui) : gui(gui) {
        
    }
    Viewer() = delete;
    Viewer(const Viewer& orig) = delete;
    Viewer(Viewer&& orig) = delete;
    
    virtual ~Viewer() {};
private:

};

#endif /* VIEWER_H */

