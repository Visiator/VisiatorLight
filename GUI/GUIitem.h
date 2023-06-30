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
#include <string>
#include <vector>

#include "GUIprimitives.h"
#include "GUItextura.h"

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
    };
    ItemType type;
    GUItextura *textura = nullptr;
    GUIitem(const char* item_id,  const char* parent_id, ItemType type, _FRAME frame, GUItextura *textura, bool is_visible, bool is_active) : id(item_id), parent_id(parent_id), type(type), frame(frame), textura(textura), is_visible(is_visible), is_active(is_active) { };
    GUIitem(const GUIitem& src) = delete;
    GUIitem(GUIitem&& src) noexcept {
        *this = std::move(src);
        printf("FONT constructor move\n");        
    }
    GUIitem& operator=(const GUIitem& src) = delete;
    GUIitem& operator=(GUIitem&& src) {
        printf("FONT = move\n");
        if(&src == this) return *this;
        copy_from_(src);
        clear_(src);
        return *this;
    }
    void copy_from_(GUIitem& src) {
        is_visible = src.is_visible;
        is_active = src.is_active;
        id         = src.id;
        type       = src.type;
        frame      = src.frame;
        textura    = src.textura;
        parent_id  = src.parent_id;
        parent     = src.parent;
    }
    void clear_(GUIitem& src) {
        src.id = "";
        src.type = ItemType::notset;
        src.frame.clear();
        src.textura = nullptr;
        src.parent_id = "";
        src.parent = nullptr;
        src.is_visible = false;
        src.is_active = false;
    }
    bool its_my(int x, int y) {
        int xx = get_gx(), yy = get_gy();
        if(x >= xx && x < xx + frame.w &&
           y >= yy && y < yy + frame.h) {
            return true;
        }
        return false;
    }
    ~GUIitem() {
        
    }
};

class GUIitems {
private:
public:
    GUItexturaList textura_list;
    std::vector<GUIitem> items;
    
    bool addd(const char *item_id, const char *parent_id, GUIitem::ItemType type, _FRAME&& frame, GUItextura* textura, bool is_visible, bool is_active) {
               
        //GUIitem a(item_id, GUIitem::ItemType::panel, frame, textura);
        
        items.push_back( {item_id, parent_id, GUIitem::ItemType::panel, frame, textura, is_visible, is_active} );
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
    GUIitems() {
        
    }
    GUIitems(const GUIitems& src) = delete;
    GUIitems(GUIitems&& src) = delete;
    ~GUIitems() {
        
    }
};

#endif /* GUIITEM_H */

