/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   Viewer.cpp
 * Author: smorodin
 * 
 * Created on July 4, 2023, 2:08 PM
 */



#include <memory>
#include <string>

#include "Viewer.h"
#include "GUI/GUIscreenBuffer.h"
#include "tools.h"



void Viewer::run() {
    printf("Viewer run\n");
    
    RECTANGLE rec(100, 100, 500, 500, 0, 255);
    InitGUIitems();
    
    window = gui->CreateWindow_( "Viewer", GUIwindow::FIXED, rec, &gui_items, this);
    window->StaticUserInput = Viewer::StaticUserInput;
    window->test();
    
    gui->test();
}

bool Viewer::InitGUIitems() {
    
    return true;
}