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
#include <mutex>


#include "GUIwindow.h"
#include "GUIprimitives.h"

class GUI;

class GUIwindowListElement {
public:
    int id;
    std::unique_ptr<GUIwindow> window;

    
    GUIwindowListElement() = delete;
    GUIwindowListElement(int id, GUI *gui, std::string name, GUIwindow::window_mode mode, RECTANGLE rectangle, GUIitems* gui_items, void* parent) : id(id) {
        
        //RECTANGLE rec(100, 100, 500, 500, 0, 255);
        
        
        // GUIwindow(GUI *gui, std::string name, GUIwindow::window_mode mode, RECTANGLE rectangle, GUIitems* gui_items, void* parent);
    
        
        window = std::make_unique<GUIwindow>(gui, name, mode, rectangle, gui_items, parent);
        printf("GUIwindowListElement(%d)\n", id );
    };
    GUIwindowListElement(const GUIwindowListElement& orig) = delete;
    GUIwindowListElement(GUIwindowListElement&& src) noexcept {
        *this = std::move(src);
        printf("GUIwindowListElement constructor move\n");
    }
    GUIwindowListElement& operator=( GUIwindowListElement& src) = delete;
    GUIwindowListElement& operator=( GUIwindowListElement&& src) noexcept {
        printf("GUIwindowListElement = move\n");
        if(&src == this) return *this;
        copy_from_(src);
        clear_(src);
        return *this;
    }
    void copy_from_(GUIwindowListElement& src) {
        id       = src.id;
        window   = std::move( src.window );
    }
    void clear_(GUIwindowListElement& src) {
        src.id = 0;
        src.window.reset();
    }
    
       
    virtual ~GUIwindowListElement() {
        
    }
};

class GUIwindowList {
public:

    GUI *gui;
    
    std::vector<GUIwindowListElement> window_list;
    
    GUIwindow* CreateWindow_(std::string name, GUIwindow::window_mode mode, RECTANGLE rectangle, GUIitems* gui_items, void* parent);
    
    std::mutex g_mutex;
    

    void stop();
    void wait();
    
    GUIwindowList(GUI *gui);
    GUIwindowList(const GUIwindowList& orig) = delete;
    virtual ~GUIwindowList() {
        
    };
private:

};

#endif /* GUIWINDOWS_H */

