/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   FONT.h
 * Author: smorodin
 *
 * Created on May 26, 2023, 6:18 PM
 */

#ifndef FONT_H
#define FONT_H

#include <map>
#include <string>
#include <vector>

class SCREEN_BUFFER;

class LETTER {
private:
    uint32_t w, h;
    std::vector<uint8_t> buf;
public:
    void print(SCREEN_BUFFER *src, int& x, int y, uint32_t color, bool show_cursor);
    int get_w() { return w; };
    int get_h() { return h; };
    //LETTER(uint32_t w, uint32_t h, uint8_t* buf);
    LETTER(uint32_t w, uint32_t h, const uint8_t* buf_src);
    LETTER() = delete;
    LETTER(const LETTER& src) = delete;
    LETTER(LETTER&& src) {
        *this = std::move(src);
    }
    LETTER& operator=( LETTER& src) = delete;
    LETTER& operator=( LETTER&& src) noexcept {
        if(&src == this) return *this;
        copy_from_(src);
        clear_(src);
        return *this;
    }
    void load_from_buffer(std::vector<uint8_t> &buffer, uint32_t idx, uint32_t w, uint32_t h);
    void copy_from_(LETTER& src) {
        w  = src.w;
        h  = src.h;
        buf = std::move(src.buf);        
    }
    void clear_(LETTER& src) {
        src.w  = 0;
        src.h  = 0;
        src.buf.clear();
    }
    ~LETTER(){};
};

class FONT { // explicit ? }__attribute__((__packed__)); ?
private:
    int id;
    bool is_load_compleete;
    std::map<uint32_t, LETTER> letter;
public:
    FONT() = delete;
    FONT(int id, const std::string& name_file) : id(id), is_load_compleete(false) {
        load_from_file(name_file);
        printf("FONT::FONT(%s)\n", name_file.c_str());
    };
    FONT(const FONT& orig) = delete;
    FONT(FONT&& src) noexcept {
        *this = std::move(src);
        printf("FONT constructor move\n");
    }
    FONT& operator=( FONT& src) = delete;
    FONT& operator=( FONT&& src) noexcept {
        printf("FONT = move\n");
        if(&src == this) return *this;
        copy_from_(src);
        clear_(src);
        return *this;
    }
    void print(SCREEN_BUFFER *scr, int x, int y, const wchar_t *text, uint32_t color, int cursor_pos);
    void print(SCREEN_BUFFER *scr, int x, int y, std::wstring& text, uint32_t color, int cursor_pos) {
        print(scr, x, y, text.c_str(), color, cursor_pos);
    }
    int letter_height(LETTER& lt);
    int text_height(const wchar_t* text);
    int text_height(std::wstring& text);
    
    int letter_width(LETTER& lt);
    int text_width(const wchar_t* text);
    int text_width(std::wstring& text);

    
    void print_letter(LETTER& lt, SCREEN_BUFFER *scr, int& x, int y, uint32_t color, bool show_cursor);
    void load_from_file(std::string name_file);
    void load_from_buffer(std::vector<uint8_t> buffer);
    void copy_from_(FONT& src) {
        id       = src.id;
        is_load_compleete = src.is_load_compleete;
        letter = std::move(src.letter);
    }
    void clear_(FONT& src) {
        src.id = 0;
        src.is_load_compleete = false;
        src.letter.clear();
    }
    virtual ~FONT(){
        printf("FONT::~FONT\n");
    };
private:

};

class FONTS {
public:
    std::map<std::string, FONT> font;
    FONT* roboto220;
    FONT* roboto150;
    FONTS() {
        
        font.emplace( std::make_pair<std::string, FONT>("roboto220", {15, "font_roboto_220.rawaaaa"}) );
        font.emplace( std::make_pair<std::string, FONT>("roboto150", {15, "font_roboto_150.rawaaaa"}) );
        
        roboto220 = &font.at("roboto220");
        roboto150 = &font.at("roboto150");
        //roboto220->print(1, 2, "123");
    }; 
    //FONTS();
    ~FONTS(){};
};

#endif /* FONT_H */

