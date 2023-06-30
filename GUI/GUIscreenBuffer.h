/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cFiles/file.h to edit this template
 */

/* 
 * File:   GUIscreenBuffer.h
 * Author: smorodin
 *
 * Created on June 22, 2023, 12:49 PM
 */

#ifndef GUISCREENBUFFER_H
#define GUISCREENBUFFER_H

#include "../font/FONT.h"
#include "GUIitem.h"

class SCREEN_BUFFER {
public:
    // 2023 FONTS fonts{this};
    bool need_update = false;
    unsigned int *buffer;
    unsigned int w, h;
    void set_pix(unsigned int x, unsigned int y, unsigned int color) {
        if(x < w && y < h) {
            buffer[x + y*w] = color;
        }
    }
    uint32_t get_pix(unsigned int x, unsigned int y) {
        if(x < w && y < h) {
            return buffer[x + y*w];
        } else {
            return 0;
        }
    }
    void print(FONT *fnt, unsigned int x, unsigned int y, const wchar_t *text);
    void line_h(unsigned int x, unsigned int y, unsigned int ww, unsigned int color) {
        if(x+ww >= w ) return;
        if(y >= h ) return;
        unsigned int *q, i;
        q = buffer + y*w + x;
        for(i = 0;i<ww;i++) *q++ = color;
    }
    void line_v(unsigned int x, unsigned int y, unsigned int hh, unsigned int color) {
        if(x >= w ) return;
        if(y+hh >= h ) return;
        unsigned int *q, i;
        q = buffer + y*w + x;
        for(i = 0;i<hh;i++) {
            *q = color;
            q += w;
        };
    }
    void rectangle(unsigned int x, unsigned int y, unsigned int ww, unsigned int hh, unsigned int color) {
        line_h(x, y, ww, color);
        line_h(x, y+hh-1, ww, color);
        line_v(x, y, hh, color);
        line_v(x+ww-1, y, hh, color);
        
    }
    void paint_textura(unsigned int x, unsigned int y, GUItextura* textura) {
        if(textura == nullptr ||
           x < 0 || x + textura->w > w ||
           y < 0 || y + textura->h > h) return;         
        
        int idx = 0;
        for(int yy = 0; yy < textura->h; yy++) {
            for(int xx = 0; xx < textura->w; xx++) {
                set_pix(x+xx, y+yy, textura->buf[idx++]);
            }
        }
    }
    void set_size(unsigned int w_, unsigned int h_) {
        printf("set_size(%d->%d, %d->%d)...\n", w, w_, h, h_);
        if(w_ == w && h_ == h) return;
        w = w_;
        h = h_;
        if(buffer != nullptr) delete[] buffer;
        buffer = new unsigned int[w*h];
    }
    void fill_all(unsigned int col) {
        if(buffer == nullptr) return;
        int k = 0;
        for(int i=0;i<h;i++) {
            for(int j=0;j<w;j++) {
                buffer[k++] = col;
            }
        }
    }
    
    void fill(unsigned int xx, unsigned int yy, unsigned int ww, unsigned int hh, unsigned int color) {
        if(xx+ww >= w ) return;
        if(yy+hh >= h ) return;
        
        unsigned int *q;
        unsigned int i, j;
        for(j = 0; j < hh; j++) {
            q = buffer + (yy+j)*w + xx;
            for(i = 0; i < ww; i++) {
                *q++ = color;
            }
        }
        
    }
    
    void fill(unsigned int xx, unsigned int yy, unsigned int ww, unsigned int hh, std::vector<unsigned int> &bitmap) {
        if(xx+ww >= w ) return;
        if(yy+hh >= h ) return;
        
        unsigned int *q;
        unsigned int i, j, k = 0;
        for(j = 0; j < hh; j++) {
            q = buffer + (yy+j)*w + xx;
            for(i = 0; i < ww; i++) {
                *q++ = bitmap[k++];
            }
        }
        
    }
    
    void paint_item(GUIitem& item) {
        if(item.get_is_visible() == false) {
            return;
        }
        int x = item.get_gx(), y = item.get_gy();
        rectangle(x, y, item.frame.w, item.frame.h, 0xff00ff);
        if(item.textura != nullptr) {
            paint_textura(x, y, item.textura);
        }
        
    }
    
    void paint_items(GUIitems* items) {
        for(int i=0; i < items->items.size(); i++) {
            paint_item(items->items[i]);
        }
    }
    
    SCREEN_BUFFER() {
        buffer = nullptr;
        w = 0;
        h = 0;

    }
};

#endif /* GUISCREENBUFFER_H */

