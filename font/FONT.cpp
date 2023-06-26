/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   FONT.cpp
 * Author: smorodin
 * 
 * Created on May 26, 2023, 6:18 PM
 */

#include <fstream>

#include "../GUI/GUIprimitives.h"
#include "FONT.h"
#include "../GUI/GUIscreenBuffer.h"
#include "../tools.h"

LETTER::LETTER(uint32_t w, uint32_t h, const uint8_t* buf_src) {
    if(w >= 512 || h >= 512 || buf_src == nullptr) return;
    this->w = w;
    this->h = h;
    buf.resize(w*h);
    for(int i=0;i<w*h;i++) {
        buf[i] = buf_src[i];
    }
    
}

void LETTER::load_from_buffer(std::vector<uint8_t> &buffer, uint32_t idx, uint32_t w, uint32_t h) {
    
}

//////////////////////////////////////

void FONT::load_from_buffer(std::vector<uint8_t> buffer) {
    uint32_t idx, no, w, h, max_size, v, n;
    idx = 0;
    max_size = buffer.size();
    no = 0;
    while(idx+3 <= max_size) {
        v = buffer[idx+0];
        w = buffer[idx+1];
        h = buffer[idx+2];
        if(v != 0x7c) return;
        if(idx + 3 + w*h > max_size) {
            return;
        }
        n = no;       
        if(w > 0) letter.emplace( std::make_pair<uint32_t, LETTER>(std::move(n), {w, h, buffer.data()+idx+3}) );
        idx += w*h + 3;
        if(idx == max_size) {
            is_load_compleete = true;
        }
        no++;
    }
}

void FONT::load_from_file(std::string name_file) {
    std::string name;
    int idx, w, h;
    
    name = "font/" + name_file;
    
    std::ifstream fstFile;
    fstFile.open(name.c_str(), std::ios::binary);
    if (fstFile.is_open())
    {
        fstFile.seekg( 0, std::ios_base::end);
        int len = fstFile.tellg();
        fstFile.seekg( 0, std::ios_base::beg);
        std::vector<uint8_t>buf;
        buf.clear();
        buf.resize(len);
        fstFile.read((char *)buf.data(), len);
        if(fstFile.gcount() == len) {
            fstFile.close();
            load_from_buffer(buf);
        } else {
            fstFile.close();
        }
    } else {
        printf("open file error\n");
    }
    
    /*FILE *f;
    f = fopen(name.c_str(), "rb");
    if(f != NULL) {
        unsigned char *b;
        b = new unsigned char[100000];
        int i;
        i = fread(b, 1, 100000, f);
        
        
        
        fclose(f);
    }*/
}

void LETTER::print(SCREEN_BUFFER *scr, int& x, int y) {
    int ii = 0;
    for(int hh=0; hh < h; hh++) {
        for(int ww=0; ww < w; ww++) {
            uint32_t clr;
            clr = scr->get_pix(x+ww, y+hh);
            
            uint32_t percent = 255 - buf[ii++];

            clr = transparent_color(clr, 0xffffff, percent);
            
            scr->set_pix(x+ww, y+hh, clr);
        }
    }
    x += w;
}

void FONT::print_letter(LETTER& lt, SCREEN_BUFFER *scr, int& x, int y) {
    lt.print(scr, x, y);
}

void FONT::print(SCREEN_BUFFER *scr, int x, int y, const wchar_t *text) {
    if(text == nullptr || scr == nullptr) return;
    int text_idx = 0;
    while(text[text_idx] != 0) {
        wchar_t t;
        t = wchar_to_ascii( text[text_idx] );
        print_letter(letter.at(t), scr, x, y);
        text_idx++;
    }
    
}