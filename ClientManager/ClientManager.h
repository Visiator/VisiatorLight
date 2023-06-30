/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   ClientManager.h
 * Author: smorodin
 *
 * Created on May 1, 2023, 2:44 PM
 */

#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <memory>
#include <stdio.h>
#include <cstdlib>

#include "../GUI/GUI.h"
#include "../GUI/GUIlowlevel.h"
#include "../GUI/GUIitem.h"


class ClientManager {
public:
    
    std::shared_ptr<GUI> gui;
    GUIwindow* window;
    bool need_stop;
    
    
    GUIitems gui_items;
    void init_textures();
    bool InitGUIitems();
    
    ClientManager(std::shared_ptr<GUI> gui);
    ClientManager() = delete;
    ClientManager(const ClientManager& orig) = delete;
    ClientManager(ClientManager&& orig) = delete;
    
    void run();
    void wait();
    void stop();
    void exit();
    
    static void StaticUserInput(void* parent, WindowUserEvent* window_user_event) {
        printf("StaticUserInput\n");
        ((ClientManager *)parent)->UserInput(window_user_event);   
    }
    void UserInput(WindowUserEvent* window_user_event) {
        printf("UserInput %d %d \n", window_user_event->x, window_user_event->y);
        
        if(window_user_event->type == WindowUserEvent::UserEventType::uev_MousePress) {
            GUIitem* item = gui_items.get_active_item_by_xy(window_user_event->x, window_user_event->y);
            if(item == nullptr) {
                printf("null\n");
                return;
            }
            
            if(item->id == "btn_out_w") {
                change_panel_to_out();
                return;
            }
            if(item->id == "btn_inc_w") {
                change_panel_to_inc();
                return;
            }
            if(item->id == "btn_auto_w") {
                change_panel_to_auto();
                return;
            }
            if(item->id == "checkbox") {
                item->is_visible = false;
                gui_items.get_item_by_id("checkbox_set")->is_visible = true;
                window->need_refresh();
                return;
            }
            if(item->id == "checkbox_set") {
                item->is_visible = false;
                gui_items.get_item_by_id("checkbox")->is_visible = true;
                window->need_refresh();
                return;
            }
            if(item->id == "krest_black") {
                printf("krest_black\n");
                item->is_visible = false;
                gui_items.get_item_by_id("krest_white")->is_visible = true;
                window->need_refresh();
                return;
            }
            if(item->id == "btn_connect_normal") {
                printf("btn_connect_normal\n");
                item->is_visible = false;
                gui_items.get_item_by_id("btn_connect_pressed")->is_visible = true;
                window->need_refresh();
                return;
            }
            if(item->id == "btn_www_normal") {
                printf("btn_www_normal\n");
                item->is_visible = false;
                gui_items.get_item_by_id("btn_www_pressed")->is_visible = true;
                window->need_refresh();
                return;
            }
            if(item->id == "btn_save_normal") {
                printf("btn_save_normal\n");
                item->is_visible = false;
                gui_items.get_item_by_id("btn_save_pressed")->is_visible = true;
                window->need_refresh();
                return;
            }
            printf("MousePress %d %d %s\n", window_user_event->x, window_user_event->y, item->id.c_str());
            return;
        }
        
        if(window_user_event->type == WindowUserEvent::UserEventType::uev_MouseUnpress) {
            if(gui_items.get_item_by_id("krest_white")->is_visible == true) {
                gui_items.get_item_by_id("krest_black")->is_visible = true;
                gui_items.get_item_by_id("krest_white")->is_visible = false;
                window->need_refresh();
                exit();
                return;
            }
            if(gui_items.get_item_by_id("btn_connect_pressed")->is_visible == true) {
                gui_items.get_item_by_id("btn_connect_pressed")->is_visible = false;
                gui_items.get_item_by_id("btn_connect_normal")->is_visible = true;
                return;
            }
            if(gui_items.get_item_by_id("btn_www_pressed")->is_visible == true) {
                gui_items.get_item_by_id("btn_www_pressed")->is_visible = false;
                gui_items.get_item_by_id("btn_www_normal")->is_visible = true;
                return;
            }
            if(gui_items.get_item_by_id("btn_save_pressed")->is_visible == true) {
                gui_items.get_item_by_id("btn_save_pressed")->is_visible = false;
                gui_items.get_item_by_id("btn_save_normal")->is_visible = true;
                return;
            }
            return;
        }
        
        printf("+++\n");        
    }
    ////////////////////////////////////////////////////////////////////////////
    
    void change_panel_to_inc() {
        
        gui_items.get_item_by_id("panel_incoming")->is_visible = true;
        gui_items.get_item_by_id("panel_outgoing")->is_visible = false;
        gui_items.get_item_by_id("panel_autorun")->is_visible = false;
        
        gui_items.get_item_by_id("btn_inc_w")->is_visible = false;
        gui_items.get_item_by_id("btn_out_w")->is_visible = true;
        gui_items.get_item_by_id("btn_auto_w")->is_visible = true;
        
        gui_items.get_item_by_id("btn_inc_y")->is_visible = true;
        gui_items.get_item_by_id("btn_out_y")->is_visible = false;
        gui_items.get_item_by_id("btn_auto_y")->is_visible = false;
        
        window->need_refresh();
    }
    void change_panel_to_out() {
        
        
        gui_items.get_item_by_id("panel_incoming")->is_visible = false;
        gui_items.get_item_by_id("panel_outgoing")->is_visible = true;
        gui_items.get_item_by_id("panel_autorun")->is_visible = false;
                
        gui_items.get_item_by_id("btn_inc_w")->is_visible = true;
        gui_items.get_item_by_id("btn_out_w")->is_visible = false;
        gui_items.get_item_by_id("btn_auto_w")->is_visible = true;
        
        gui_items.get_item_by_id("btn_inc_y")->is_visible = false;
        gui_items.get_item_by_id("btn_out_y")->is_visible = true;
        gui_items.get_item_by_id("btn_auto_y")->is_visible = false;
        
        window->need_refresh();
    }
    
    void change_panel_to_auto() {
        
        gui_items.get_item_by_id("panel_incoming")->is_visible = false;
        gui_items.get_item_by_id("panel_outgoing")->is_visible = false;
        gui_items.get_item_by_id("panel_autorun")->is_visible = true;
        
        gui_items.get_item_by_id("btn_inc_w")->is_visible = true;
        gui_items.get_item_by_id("btn_out_w")->is_visible = true;
        gui_items.get_item_by_id("btn_auto_w")->is_visible = false;
        
        gui_items.get_item_by_id("btn_inc_y")->is_visible = false;
        gui_items.get_item_by_id("btn_out_y")->is_visible = false;
        gui_items.get_item_by_id("btn_auto_y")->is_visible = true;
        
        window->need_refresh();
    }
    ////////////////////////////////////////////////////////////////////////////
    
    virtual ~ClientManager();
private:

};

#endif /* CLIENTMANAGER_H */

