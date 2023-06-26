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
    
    static void StaticUserInput(void* parent, WindowUserEvent* window_user_event) {
        ((ClientManager *)parent)->UserInput(window_user_event);   
    }
    void UserInput(WindowUserEvent* window_user_event) {
        if(window_user_event->type == WindowUserEvent::UserEventType::uev_MousePress) {
            printf("MousePress %d %d\n", window_user_event->x, window_user_event->y);
            return;
        }
        printf("+++\n");        
    }
    virtual ~ClientManager();
private:

};

#endif /* CLIENTMANAGER_H */

