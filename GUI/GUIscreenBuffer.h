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

#include <stdint.h>
#include <vector>
#include <mutex>

//#include "../font/FONT.h"
//#include "GUIitem.h"

class FONTS;
class FONT;
class GUItextura;
class GUIitems;
class GUIitem;

struct C8SH16 {
	uint16_t c;
	uint16_t s;
};

struct PAL {
	uint8_t color;
	uint16_t len;
	uint32_t count;
};

struct PAL12 {
	unsigned short color;
	unsigned short len;
	unsigned int   count;
	unsigned int   sort;
	PAL12() {
		color = 0;
		len = 0;
		count = 0;
		sort = 0;
	}
};

struct  ENCODED_SCREEN_12bit_header {
	unsigned int reserv01;
	unsigned int reserv02;
	unsigned int reserv03;
	unsigned int reserv04;

	unsigned int format;
	unsigned int header_size; // 

	unsigned short w, h;

	unsigned char color_bit;
	unsigned char reserve1;
	unsigned char reserve2;
	unsigned char reserve3;

	unsigned int screen_id;
	unsigned int old_screen_id;
	unsigned int pal_size;
	unsigned int body_size;

	unsigned short mouse_x;
	unsigned short mouse_y;
	unsigned int   mouse_cursor_type_id;
	unsigned int   keyboard_location;
	unsigned int   itis_user_move_mouse;

};

#define PAL12_color_maxcount 4096
#define PAL12_len_maxcount 4096

struct  ENCODED_SCREEN_8bit_header {
    unsigned int reserv01;
    unsigned int reserv02;
    unsigned int reserv03;
    unsigned int reserv04;

    unsigned int format;
    unsigned int header_size; // 

    unsigned short w, h;

    unsigned char color_bit;
    unsigned char reserve1;
    unsigned char reserve2;
    unsigned char reserve3;

    unsigned int screen_id;
    unsigned int old_screen_id;
    unsigned int pal_size;
    unsigned int body_size;

    unsigned short mouse_x;
    unsigned short mouse_y;
    unsigned int   mouse_cursor_type_id;
    unsigned int   keyboard_location;
    unsigned int   itis_user_move_mouse;

};



class SCREEN_BUFFER {
private:
    std::mutex mutexx;
    
public:
    int lock_owner;
    
    FONTS* fonts;
    bool need_update = false;
    unsigned int *buffer;
    unsigned int w, h;
    uint32_t keyboard_location;
    uint32_t decode_color_matrix_G7C223[256];
    void init_decode_color2();
    bool unpack_8bit_v1(uint8_t* buf, uint32_t len);
    std::vector<uint32_t> tmp_buffer;
    void resize_from(SCREEN_BUFFER* src);
    
    
    void lock(int v);
    void unlock(int v);
    
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
    void print(FONT *fnt, unsigned int x, unsigned int y, const wchar_t *text, bool is_pass, uint32_t color, int cursor_pos);
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
    void paint_textura(unsigned int x, unsigned int y, GUItextura* textura);
    void set_size(unsigned int w_, unsigned int h_) {
        //printf("set_size(%d->%d, %d->%d)...\n", w, w_, h, h_);
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
    
    void paint_item_editpass16(GUIitem& item);

    void paint_item_edittext(GUIitem& item);
    
    void paint_item_editid(GUIitem& item);
    
    void paint_item(GUIitem& item);
    
    void paint_items(GUIitems* items);
    
    void paint_item_viewer(GUIitem& item);
    
    SCREEN_BUFFER(FONTS* fonts) : fonts(fonts), keyboard_location(0), lock_owner(0) {
        buffer = nullptr;
        w = 0;
        h = 0;
        init_decode_color2();
    }
};

#endif /* GUISCREENBUFFER_H */

