/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */

#include "GUIitem.h"
#include "GUIwindow.h"

void GUIitem::UserInput(WindowUserEvent* window_user_event) {
    printf("GUIitem::UserInput()\n");
    if(type == GUIitem::ItemType::editpass16 ||
       type == GUIitem::ItemType::edittext ||
       type == GUIitem::ItemType::editid) {
        if(this->is_active && this->is_visible && this->is_edit_begin) {
            printf("=%d=\n", window_user_event->keycode);
            if(window_user_event->keycode == 113) { // left
                if(this->edit_text_cursor_pos > 0) edit_text_cursor_pos--;
            }
            if(window_user_event->keycode == 114) { // right
                if(this->edit_text_cursor_pos < this->edit_text.size()) edit_text_cursor_pos++;
            }
        }
    }
}