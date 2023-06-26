/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   GUI.h
 * Author: smorodin
 *
 * Created on May 1, 2023, 2:46 PM
 */

#ifndef GUI_H
#define GUI_H

#include <memory>
#include <stdio.h>
#include <cstdlib>

#ifdef __linux__
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/extensions/XTest.h>
#endif

#include "GUIlowlevel.h"
#include "GUItextura.h"
#include "GUIwindowList.h"
#include "../font/FONT.h"


#ifdef __linux__
class LINUX_PARAM {
public:
    Display     *display_ = nullptr;
    //Window      window = 0;
    int         screen_id = 0;
    GC          graph_ctx = 0;
    XGCValues   *gc_values = nullptr;
    //XImage      *image;
    /*MOUSE_BUTTON_TYPE DetectButtonTypeLinux(unsigned int v) {
        if(v == 1) return MOUSE_BUTTON_TYPE::left;
        if(v == 2) return MOUSE_BUTTON_TYPE::muddle;
        if(v == 3) return MOUSE_BUTTON_TYPE::right;
        return MOUSE_BUTTON_TYPE::left;
    }*/
    unsigned int DetectKeyTypeLinux(unsigned int v) {
        return v;
    }
};
#endif

#ifdef _WIN32
class WIN32_PARAM {
public:
    uint32_t p1;
};
#endif

class GUI {
public:
    
#ifdef __linux__
    LINUX_PARAM linux;
#endif
    
#ifdef _WIN32
    HINSTANCE hInstance;
    WIN32_PARAM win32_param;
#endif
    
    FONTS fonts;
    std::unique_ptr<GUIlowlevel> gui_low_level;
    GUIwindowList window_list;
    
    void test();
 
    GUIwindow* CreateWindow_(std::string name, GUIwindow::window_mode mode, RECTANGLE rectangle, GUIitems* gui_items, void* parent);
    
    GUI();
    GUI(const GUI& orig) = delete;
    virtual ~GUI();
private:

};

#endif /* GUI_H */

