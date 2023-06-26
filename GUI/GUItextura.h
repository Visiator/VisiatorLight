/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cFiles/file.h to edit this template
 */

/* 
 * File:   GUItextura.h
 * Author: smorodin
 *
 * Created on June 23, 2023, 8:07 PM
 */

#ifndef GUITEXTURA_H
#define GUITEXTURA_H

#include <vector>
#include <string>

struct _BMP_header_part1 {
public:    
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offBits;
    uint32_t bitmapinfo_ver;
} __attribute__((packed));

struct _BMP_header_part2 {
public:    
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bitcount;
    uint32_t compression;
    uint32_t sizeimage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t clrused;
    uint32_t clrimportant;
} __attribute__((packed));

class GUItextura {
public:
    std::string id;
    std::vector<uint32_t>buf;
    uint32_t w, h;
    GUItextura(const char* file_name, const char* id) : id(id), w(0), h(0) {
        load_from_file(file_name);
    }
    GUItextura() = delete;
    GUItextura(const GUItextura& src) = delete;
    GUItextura(GUItextura&& src) noexcept {
        *this = std::move(src);
        printf("GUItextura constructor move\n");       
    }
    GUItextura& operator=(const GUItextura& src) = delete;
    GUItextura& operator=(GUItextura&& src) {
        printf("GUItextura = move\n");
        if(&src == this) return *this;
        copy_from_(src);
        clear_(src);
        return *this;
    }
    void copy_from_(GUItextura& src) {
        id = src.id;
        w = src.w;
        h = src.h;
        buf = src.buf;
    }
    void clear_(GUItextura& src) {
        src.id = "";
        src.w = 0;
        src.h = 0;
        //src.buf.clear();
    }
    void load_from_file(const char *file_name);
    ~GUItextura() {
        
    }
};

class GUItexturaList {
public:
    std::vector<GUItextura> items;
    void add(const char* file_name, const char* id) {
        items.push_back( {file_name, id} );
    }
    GUItextura* get_textura_by_id(const char *id);
    GUItexturaList() {
        
    }
    ~GUItexturaList() {
        
    }
};

#endif /* GUITEXTURA_H */

