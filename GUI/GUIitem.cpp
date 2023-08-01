/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */

#include "GUIitem.h"
#include "GUIwindow.h"

extern int g_1;

GUIitem::GUIitem(GUIitems* items, const char* item_id,  const char* parent_id, ItemType type, _FRAME frame, GUItextura *textura, bool is_visible, bool is_active) 
      : items(items), id(item_id), parent_id(parent_id), type(type), frame(frame), textura(textura), is_visible(is_visible), is_active(is_active)
      , edit_text(L""), edit_text_cursor_pos(-1), is_edit_begin(false), is_visible_pass(false) , window(nullptr)
    { 
        if(type == ItemType::viewer) {
            screen_buf = std::make_unique<SCREEN_BUFFER>(nullptr);
            screen_buf.get()->set_size(50, 40);
        }
    };

void GUIitem::UserInput(WindowUserEvent* window_user_event) {
    printf("GUIitem::UserInput()\n");
    if(type == GUIitem::ItemType::editpass16 ||
       type == GUIitem::ItemType::edittext ||
       type == GUIitem::ItemType::editid) {
        if(this->is_active && this->is_visible && this->is_edit_begin) {
            
            if(window_user_event->keycode == 37) { // left
                printf("=%d=-\n", window_user_event->keycode);
                if(this->edit_text_cursor_pos > 0) {
                    edit_text_cursor_pos--;
                    need_refresh();
                }
            }
            if(window_user_event->keycode == 39) { // right
                printf("=%d=+\n", window_user_event->keycode);
                if(this->edit_text_cursor_pos < this->edit_text.size()) {
                    edit_text_cursor_pos++;
                    need_refresh();
                }
            }
        }
    }
}

void GUIitem::arrived_screen(SCREEN_BUFFER *src) {
    if(src == nullptr) return;
    if(src->w == 0 || src->w > 10000 ||
       src->h == 0 || src->h > 10000) return;
    
    bool need_window_size = false;
    
    g_1 = 10;
    screen_buf.get()->lock(1);
    g_1 = 20;
    
    float k;
    k = (float)src->w / (float)src->h;
    int hh;
    hh = (float)window->rectangle.w / k;
    

    if(window->rectangle.h != hh) {
        window->rectangle.h = hh;
        g_1 = 21;
        screen_buf.get()->set_size(window->rectangle.w, window->rectangle.h);
        g_1 = 22;
        need_window_size = true;
        g_1 = 23;
    }

    g_1 = 27;
    screen_buf.get()->resize_from(src);
    g_1 = 28;
    screen_buf.get()->unlock(1);
    g_1 = 29;
    
    if(window != nullptr) {
        if(need_window_size) {
            frame.w = window->rectangle.w;
            frame.h = window->rectangle.h;
            window->set_size(window->rectangle.w, window->rectangle.h);
        }
        window->need_refresh();
    }
}

void GUIitem::need_refresh() {
    if(items != nullptr && items->gui_window != nullptr) {
        items->gui_window->need_refresh();
    }
}