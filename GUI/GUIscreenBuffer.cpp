/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */

#include "GUIscreenBuffer.h"

void SCREEN_BUFFER::print(FONT *fnt, unsigned int x, unsigned int y, const wchar_t *text, bool is_pass, uint32_t color, int cursor_pos) {
    if(fnt == nullptr) return;
    fnt->print(this, x, y, text, is_pass, color, cursor_pos);
}