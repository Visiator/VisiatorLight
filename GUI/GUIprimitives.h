/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cFiles/file.h to edit this template
 */

/* 
 * File:   GUIprimitives.h
 * Author: smorodin
 *
 * Created on May 8, 2023, 1:14 PM
 */

#ifndef GUIPRIMITIVES_H
#define GUIPRIMITIVES_H

#include <vector>
#include "../font/FONT.h"



enum class MOUSE_BUTTON_TYPE : short { left, muddle, right };

class _FRAME {
private:
    
public:
    int x, y, w, h;
    //int gx, gy, gw, gh;
    
    int getx() { return x; };
    int gety() { return y; };
    int getw() { return w; };
    int geth() { return h; };
    void set_xywh(int x, int y, int w, int h) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
        
        /*gx = 0;
        gy = 0;
        gw = 0;
        gh = 0;*/
        
    }
    void copy_from_src(const _FRAME &src) {
        x = src.x;
        y = src.y;
        w = src.w;
        h = src.h;        
    }
    void clear() {
        x = 0;
        y = 0;
        w = 0;
        h = 0;
    }
    bool its_me(int xx, int yy) {
        if(xx >= x && xx <=x+w-1 &&
           yy >= y && yy <=y+h-1) return true;
        return false;
    }
    _FRAME(const _FRAME& src) {
        copy_from_src(src);
    }
    _FRAME(unsigned int x, unsigned int y, unsigned int w, unsigned int h) : x(x), y(y), w(w), h(h) {};
    _FRAME() : x(0), y(0), w(0), h(0) {};
    ~_FRAME() {};
};

class RECTANGLE : public _FRAME {
public:
    unsigned int bg_color = 0, border_color = 0x006600, bg_color_pressed = 0x00aa00;
    void copy_from_src(const RECTANGLE &src) {
        bg_color = src.bg_color;
        border_color = src.border_color;
        bg_color_pressed = src.bg_color_pressed;
        _FRAME::copy_from_src(src);
    }
    // 2023 void paint(SCREEN_BUFFER *screen);
    RECTANGLE(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int bg_color, unsigned int border_color) : _FRAME(x, y, w, h), bg_color(bg_color), border_color(border_color) {};
    RECTANGLE() {};
    ~RECTANGLE(){};
};


#endif /* GUIPRIMITIVES_H */

