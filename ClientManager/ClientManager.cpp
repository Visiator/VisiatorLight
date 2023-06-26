/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   ClientManager.cpp
 * Author: smorodin
 * 
 * Created on May 1, 2023, 2:44 PM
 */

#include <string>

#include "ClientManager.h"
#include "../GUI/GUIscreenBuffer.h"
#include "../tools.h"

ClientManager::ClientManager(std::shared_ptr<GUI> gui) : gui(gui), window(nullptr), need_stop(false) {
    printf("ClientManager constructor1\n");
}

ClientManager::~ClientManager() {
    printf("ClientManager destructor\n");
}

void ClientManager::run() {
    printf("ClientManager run\n");
    
    RECTANGLE rec(10, 10, 250, 344, 0, 255);
    InitGUIitems();
    
    window = gui->CreateWindow_( "ClientManager", GUIwindow::FIXED, rec, &gui_items, this);
    window->StaticUserInput = ClientManager::StaticUserInput;
    window->test();
    
    gui->test();
}

void ClientManager::stop() {
    if(window) {
        window->stop();
    }
}

void ClientManager::wait() {
    if(!window) return;
    while(window->execute_is_run) {
        sleep_();
    }
}

void ClientManager::init_textures() {
    
    
    gui_items.textura_list.add( "inc/btn_incomming_yellow.bmp" ,  "btn_inc_y");
    gui_items.textura_list.add( "inc/btn_outcomming_yellow.bmp" , "btn_out_y");
    gui_items.textura_list.add( "inc/btn_autorun_yellow.bmp" ,    "btn_auto_y");
    
    gui_items.textura_list.add( "inc/main_bg.bmp" , "main_bg");
    
    gui_items.textura_list.add( "inc/btn_incomming_white.bmp" ,  "btn_inc_w");
    gui_items.textura_list.add( "inc/btn_outcomming_white.bmp" , "btn_out_w");
    gui_items.textura_list.add( "inc/btn_autorun_white.bmp" ,    "btn_auto_w");

    gui_items.textura_list.add( "inc/panel_incoming.bmp" , "panel_incoming");
    gui_items.textura_list.add( "inc/krest_black.bmp" , "krest_black");
    
    
    //btn_inc_yel = textura_list.get_textura_by_id("btn_inc_yel");
}

bool ClientManager::InitGUIitems() {
    
    init_textures();

    gui_items.addd("main_bg", "", GUIitem::ItemType::panel, _FRAME(0, 0, 250, 344), gui_items.textura_list.get_textura_by_id("main_bg"), true);
    
    gui_items.addd("krest_black", "", GUIitem::ItemType::panel, _FRAME(223, 8, 20, 20), gui_items.textura_list.get_textura_by_id("krest_black"), true);

    gui_items.addd("panel1", "", GUIitem::ItemType::panel, _FRAME(8, 270, 234, 65), nullptr, true );

    gui_items.addd("panel_incoming", "", GUIitem::ItemType::panel, _FRAME(8, 48, 234, 212), gui_items.textura_list.get_textura_by_id("panel_incoming"), true );
    
    gui_items.addd("btn_inc_y",  "panel1", GUIitem::ItemType::panel, _FRAME(0, 0, 77, 65),   gui_items.textura_list.get_textura_by_id("btn_inc_y"), true );
    gui_items.addd("btn_out_y",  "panel1", GUIitem::ItemType::panel, _FRAME(77, 0, 80, 65),  gui_items.textura_list.get_textura_by_id("btn_out_y"), false );
    gui_items.addd("btn_auto_y", "panel1", GUIitem::ItemType::panel, _FRAME(157, 0, 77, 65), gui_items.textura_list.get_textura_by_id("btn_auto_y"), false );
    
    gui_items.addd("btn_inc_w",  "panel1", GUIitem::ItemType::panel, _FRAME(0, 0, 77, 65),   gui_items.textura_list.get_textura_by_id("btn_inc_w"), false );
    gui_items.addd("btn_out_w",  "panel1", GUIitem::ItemType::panel, _FRAME(77, 0, 80, 65),  gui_items.textura_list.get_textura_by_id("btn_out_w"), true );
    gui_items.addd("btn_auto_w", "panel1", GUIitem::ItemType::panel, _FRAME(157, 0, 77, 65), gui_items.textura_list.get_textura_by_id("btn_auto_w"), true );

    for(int i=0; i < gui_items.items.size(); i++ ) {
        if(gui_items.items[i].parent_id != "" &&
           gui_items.items[i].parent == nullptr)
        {
            gui_items.items[i].parent = gui_items.get_item_by_id(gui_items.items[i].parent_id);
        }
    }
    
    
    return true;
}