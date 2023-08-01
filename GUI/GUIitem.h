/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cFiles/file.h to edit this template
 */

/* 
 * File:   GUIitem.h
 * Author: smorodin
 *
 * Created on June 17, 2023, 12:14 PM
 */

#ifndef GUIITEM_H
#define GUIITEM_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "GUIprimitives.h"
#include "GUIscreenBuffer.h"
#include "GUItextura.h"
#include "GUIwindow.h"

class WindowUserEvent;
class GUIwindow;
class GUItextura;
class GUIitems;

class GUIitem {
private:
    
public:
    bool is_visible;
    bool is_active;
    std::string id;
    std::string parent_id;
    GUIitem* parent = nullptr;
    _FRAME frame;
    int get_gx() {
        if(parent == nullptr) {
            return frame.x;
        }
        return frame.x + parent->get_gx();
    };
    int get_gy() {
        if(parent == nullptr) {
            return frame.y;
        }
        return frame.y + parent->get_gy();
    };
    bool get_is_visible() {
        if(parent == nullptr) {
            return is_visible;
        }
        if(is_visible == false) {
            return false;
        }
        return parent->get_is_visible();
    }
    enum ItemType {
          notset = 1
        , panel = 2
        //, textura = 3
        , edittext = 4
        , editid = 5
        , editpass16 = 6
        , checkbox = 7
        , viewer = 8          
    };
    ItemType type;
    std::wstring edit_text;
    int edit_text_cursor_pos;
    bool is_edit_begin;
    bool is_visible_pass;
    GUIwindow* window;
    std::unique_ptr<SCREEN_BUFFER> screen_buf;
    void need_refresh();
    GUItextura *textura = nullptr;
    GUIitems* items = nullptr;
    GUIitem(GUIitems* items, const char* item_id,  const char* parent_id, ItemType type, _FRAME frame, GUItextura *textura, bool is_visible, bool is_active);
    GUIitem(const GUIitem& src) = delete;
    GUIitem(GUIitem&& src) noexcept {
        *this = std::move(src);
        //printf("GUIitem constructor move\n");
    }
    GUIitem& operator=(const GUIitem& src) = delete;
    GUIitem& operator=(GUIitem&& src) {
        //printf("GUIitem = move\n");
        if(&src == this) return *this;
        copy_from_(src);
        clear_(src);
        return *this;
    }
    void copy_from_(GUIitem& src) {
        items      = src.items;
        is_visible = src.is_visible;
        is_active  = src.is_active;
        id         = src.id;
        type       = src.type;
        frame      = src.frame;
        textura    = src.textura;
        parent_id  = src.parent_id;
        parent     = src.parent;
        edit_text  = src.edit_text;
        edit_text_cursor_pos = src.edit_text_cursor_pos;
        is_edit_begin = src.is_edit_begin;
        is_visible_pass = src.is_visible_pass;
        window     = src.window;
        screen_buf   = std::move( src.screen_buf );
    }
    void clear_(GUIitem& src) {
        src.items = nullptr;
        src.id = "";
        src.type = ItemType::notset;
        src.frame.clear();
        src.textura = nullptr;
        src.parent_id = "";
        src.parent = nullptr;
        src.is_visible = false;
        src.is_active = false;
        src.edit_text = L"";
        src.edit_text_cursor_pos = -1;
        src.is_edit_begin = false;
        src.is_visible_pass = false;
        src.window = nullptr;
        src.screen_buf.reset();
    }
    bool its_my(int x, int y) {
        int xx = get_gx(), yy = get_gy();
        if(x >= xx && x < xx + frame.w &&
           y >= yy && y < yy + frame.h) {
            return true;
        }
        return false;
    }
    
    void arrived_screen(SCREEN_BUFFER *src);
    
    void UserInput(WindowUserEvent* window_user_event);
    ~GUIitem() {
        
    }
};

class GUIitems {
private:
public:
    GUItexturaList textura_list;
    std::vector<GUIitem> items;
    GUIwindow* gui_window;
    void set_GUIwindow(GUIwindow* w) {
        gui_window = w;
    }
    bool addd(const char *item_id, const char *parent_id, GUIitem::ItemType type, _FRAME&& frame, GUItextura* textura, bool is_visible, bool is_active) {
               
        //GUIitem a(item_id, GUIitem::ItemType::panel, frame, textura);
        
        items.push_back( {this, item_id, parent_id, type, frame, textura, is_visible, is_active} );
        return true;
    }
    GUIitem* get_item_by_id(std::string& id) {
        for(int i = 0; i < items.size(); i++) {
            if(items[i].id == id) {
                return &items[i];
            }
        }
        return nullptr;
    }
    GUIitem* get_item_by_id(const char* id) {
        for(int i = 0; i < items.size(); i++) {
            if(items[i].id == id) {
                return &items[i];
            }
        }
        return nullptr;
    }
    GUIitem* get_active_item_by_xy(int x, int y) {
        for(int i = 0; i < items.size(); i++) {
            if(items[i].its_my(x, y)) {
                if(items[i].get_is_visible() == true) {
                    if(items[i].is_active == true) {
                        return &items[i];
                    }
                }
            }
        }
        return nullptr;
    }
    GUIitem* get_edit_item() {
        for(int i = 0; i < items.size(); i++) {
            if(items[i].is_edit_begin ) {
                return &items[i];
            }
        }
        return nullptr;
    }
    GUIitems() : gui_window(nullptr) {
        
    }
    GUIitems(const GUIitems& src) = delete;
    GUIitems(GUIitems&& src) = delete;
    ~GUIitems() {
        
    }
};

#endif /* GUIITEM_H */

