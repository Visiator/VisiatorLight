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
    printf("----ClientManager run\n");
    
    RECTANGLE rec(10, 10, 250, 344, 0, 255);
    InitGUIitems();
    
    window = gui->CreateWindow_( "ClientManager", GUIwindow::FIXED, rec, &gui_items, this);
    window->StaticUserInput = ClientManager::StaticUserInput;
    window->test();
    
    gui_items.set_GUIwindow(window);
    
    gui->test();
}

void ClientManager::exit() {

}

void ClientManager::stop() {

}

void ClientManager::wait() {
    while(need_stop == false) {
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
    gui_items.textura_list.add( "inc/panel_outgoing.bmp" , "panel_outgoing");
    gui_items.textura_list.add( "inc/panel_autorun.bmp" , "panel_autorun");

    gui_items.textura_list.add( "inc/krest_black.bmp" , "krest_black");
    gui_items.textura_list.add( "inc/krest_blue.bmp" , "krest_blue");
    gui_items.textura_list.add( "inc/krest_white.bmp" , "krest_white");
    
    gui_items.textura_list.add( "inc/checkbox.bmp" , "checkbox");
    gui_items.textura_list.add( "inc/checkbox_set.bmp" , "checkbox_set");
    
    gui_items.textura_list.add( "inc/btn_connect_normal.bmp" , "btn_connect_normal");
    gui_items.textura_list.add( "inc/btn_connect_pressed.bmp" , "btn_connect_pressed");
    
    gui_items.textura_list.add( "inc/btn_save_normal.bmp" , "btn_save_normal");
    gui_items.textura_list.add( "inc/btn_save_pressed.bmp" , "btn_save_pressed");

    gui_items.textura_list.add( "inc/btn_www_normal.bmp" , "btn_www_normal");
    gui_items.textura_list.add( "inc/btn_www_pressed.bmp" , "btn_www_pressed");

    gui_items.textura_list.add( "inc/eye.bmp" , "eye");
    gui_items.textura_list.add( "inc/eye_active.bmp" , "eye_active");

    //btn_inc_yel = textura_list.get_textura_by_id("btn_inc_yel");
}

bool ClientManager::InitGUIitems() {
    
    init_textures();

    gui_items.addd("main_bg", "", GUIitem::ItemType::panel, _FRAME(0, 0, 250, 344), gui_items.textura_list.get_textura_by_id("main_bg"), true, false);
    
    gui_items.addd("krest_black", "", GUIitem::ItemType::panel, _FRAME(223, 8, 20, 20), gui_items.textura_list.get_textura_by_id("krest_black"), true, true);
    gui_items.addd("krest_blue", "",  GUIitem::ItemType::panel, _FRAME(223, 8, 20, 20), gui_items.textura_list.get_textura_by_id("krest_blue"), false, true);
    gui_items.addd("krest_white", "", GUIitem::ItemType::panel, _FRAME(223, 8, 20, 20), gui_items.textura_list.get_textura_by_id("krest_white"), false, true);

    gui_items.addd("panel1", "", GUIitem::ItemType::panel, _FRAME(8, 270, 234, 65), nullptr, true, false );

    gui_items.addd("panel_incoming", "", GUIitem::ItemType::panel, _FRAME(8, 48, 234, 212), gui_items.textura_list.get_textura_by_id("panel_incoming"), true, false );
    gui_items.addd("panel_outgoing", "", GUIitem::ItemType::panel, _FRAME(8, 48, 234, 212), gui_items.textura_list.get_textura_by_id("panel_outgoing"), false, false );
    gui_items.addd("panel_autorun", "", GUIitem::ItemType::panel, _FRAME(8, 48, 234, 212), gui_items.textura_list.get_textura_by_id("panel_autorun"),   false, false );
    
    gui_items.addd("btn_inc_y",  "panel1", GUIitem::ItemType::panel, _FRAME(0, 0, 77, 65),   gui_items.textura_list.get_textura_by_id("btn_inc_y"), true, true );
    gui_items.addd("btn_out_y",  "panel1", GUIitem::ItemType::panel, _FRAME(77, 0, 80, 65),  gui_items.textura_list.get_textura_by_id("btn_out_y"), false, true );
    gui_items.addd("btn_auto_y", "panel1", GUIitem::ItemType::panel, _FRAME(157, 0, 77, 65), gui_items.textura_list.get_textura_by_id("btn_auto_y"), false, true );
    
    gui_items.addd("btn_inc_w",  "panel1", GUIitem::ItemType::panel, _FRAME(0, 0, 77, 65),   gui_items.textura_list.get_textura_by_id("btn_inc_w"), false, true );
    gui_items.addd("btn_out_w",  "panel1", GUIitem::ItemType::panel, _FRAME(77, 0, 80, 65),  gui_items.textura_list.get_textura_by_id("btn_out_w"), true, true );
    gui_items.addd("btn_auto_w", "panel1", GUIitem::ItemType::panel, _FRAME(157, 0, 77, 65), gui_items.textura_list.get_textura_by_id("btn_auto_w"), true, true );

    gui_items.addd("edit_inc_id",   "panel_incoming", GUIitem::ItemType::editid , _FRAME(68, 63, 140, 28), nullptr, true, false );
    gui_items.addd("edit_inc_pass", "panel_incoming", GUIitem::ItemType::edittext , _FRAME(68, 120, 140, 28), nullptr, true, false );

    
    gui_items.addd("edit_out_id",   "panel_outgoing", GUIitem::ItemType::editid , _FRAME(68, 63, 140, 28), nullptr, true, true );
    gui_items.addd("edit_out_pass", "panel_outgoing", GUIitem::ItemType::editpass16 , _FRAME(68, 120, 140, 28), nullptr, true, true );

    gui_items.addd("eye_out", "edit_out_pass", GUIitem::ItemType::panel , _FRAME(135, 9, 14, 10), gui_items.textura_list.get_textura_by_id("eye"), true, true );
    gui_items.addd("eye_out_active", "edit_out_pass", GUIitem::ItemType::panel , _FRAME(135, 9, 14, 10), gui_items.textura_list.get_textura_by_id("eye_active"), false, true );

    gui_items.addd("edit_auto_id",   "panel_autorun", GUIitem::ItemType::editid , _FRAME(68, 63, 140, 28), nullptr, true, false );
    gui_items.addd("edit_auto_pass", "panel_autorun", GUIitem::ItemType::editpass16 , _FRAME(68, 120, 140, 28), nullptr, true, true );
    
    gui_items.addd("eye_auto", "edit_auto_pass", GUIitem::ItemType::panel , _FRAME(135, 9, 14, 10), gui_items.textura_list.get_textura_by_id("eye"), true, true );
    gui_items.addd("eye_auto_active", "edit_auto_pass", GUIitem::ItemType::panel , _FRAME(135, 9, 14, 10), gui_items.textura_list.get_textura_by_id("eye_active"), false, true );  
    
    gui_items.addd("checkbox", "panel_autorun", GUIitem::ItemType::checkbox, _FRAME(25, 174, 18, 18), gui_items.textura_list.get_textura_by_id("checkbox"), true, true );
    gui_items.addd("checkbox_set", "panel_autorun", GUIitem::ItemType::checkbox, _FRAME(25, 174, 18, 18), gui_items.textura_list.get_textura_by_id("checkbox_set"), false, true );
    
    gui_items.addd("btn_connect_normal", "panel_outgoing", GUIitem::ItemType::panel, _FRAME(12, 162, 212, 46), gui_items.textura_list.get_textura_by_id("btn_connect_normal"), true, true );
    gui_items.addd("btn_connect_pressed", "panel_outgoing", GUIitem::ItemType::panel, _FRAME(12, 162, 212, 46), gui_items.textura_list.get_textura_by_id("btn_connect_pressed"), false, true );

    gui_items.addd("btn_www_normal", "panel_incoming", GUIitem::ItemType::panel, _FRAME(60, 175, 114, 14), gui_items.textura_list.get_textura_by_id("btn_www_normal"), true, true );
    gui_items.addd("btn_www_pressed", "panel_incoming", GUIitem::ItemType::panel, _FRAME(60, 175, 114, 14), gui_items.textura_list.get_textura_by_id("btn_www_pressed"), false, true );
    
    gui_items.addd("btn_save_normal", "panel_autorun", GUIitem::ItemType::panel, _FRAME(102, 162, 122, 46), gui_items.textura_list.get_textura_by_id("btn_save_normal"), true, true );
    gui_items.addd("btn_save_pressed", "panel_autorun", GUIitem::ItemType::panel, _FRAME(102, 162, 122, 46), gui_items.textura_list.get_textura_by_id("btn_save_pressed"), false, true );
    
    gui_items.get_item_by_id("edit_inc_id")->edit_text = L"345-678-901";
    gui_items.get_item_by_id("edit_inc_pass")->edit_text = L"Pass123!@#";

    gui_items.get_item_by_id("edit_out_id")->edit_text = L"340-678-901";
    gui_items.get_item_by_id("edit_out_pass")->edit_text = L"PaSs123!@#";

    gui_items.get_item_by_id("edit_auto_id")->edit_text = L"300-600-901";
    gui_items.get_item_by_id("edit_auto_pass")->edit_text = L"Pa00000!@#";
    
    for(int i=0; i < gui_items.items.size(); i++ ) {
        if(gui_items.items[i].parent_id != "" &&
           gui_items.items[i].parent == nullptr)
        {
            gui_items.items[i].parent = gui_items.get_item_by_id(gui_items.items[i].parent_id);
        }
    }
    
    
    return true;
}