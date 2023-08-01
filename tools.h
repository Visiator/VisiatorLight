/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cFiles/file.h to edit this template
 */

/* 
 * File:   tools.h
 * Author: smorodin
 *
 * Created on May 8, 2023, 1:54 PM
 */

#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include "NET/NET.h"

#ifdef _WIN32
#define _WINSOCKAPI_ 
#include <windows.h>
#undef _WINSOCKAPI_
#include <winsock2.h>
#include <stdlib.h>
#include <iphlpapi.h>
#include <stdio.h>
#undef _WINSOCKAPI_
#endif


class MY_AES;

class VECTOR_EX {
public:
    std::vector<uint8_t> val;
    uint8_t *buf;
    int pos, max_size;
    void add(uint8_t* b, int len) {
        if(pos + len > max_size) {
            max_size += (len + 5000);
            val.resize(max_size);
            buf = val.data();
        }
        for(int i=0; i < len; i++) {
            val[pos++] = b[i];
        }
    }
    VECTOR_EX() : pos(0), max_size(0), buf(nullptr) {

    }
};



int  my_strlen(unsigned char *v);
int  my_strlen(wchar_t *v);

void sleep_();
void sleep_(int val);
void wtf(std::string str);
void fatal_error(std::string str);
uint32_t transparent_color(uint32_t old_color, uint32_t new_color, int percent);
int  wchar_to_ascii(int p);
uint32_t convert_key_from_w32(uint32_t wp);
uint32_t convert_key_from_linux(uint32_t wp);
int  setnonblocking(int sockfd);
void closesocket(int sock);
int  my_recv(int sock, uint8_t *buf, int size);
int  my_send(int sock, uint8_t *buf, int size);

bool my_random(uint8_t *buf, int buf_len);
uint32_t get_sol();
void zero_(uint8_t *s, int len);
void zero_(std::vector<uint8_t>& buf);
uint32_t get_ip_view_visiator_com();
bool Save_private_id_and_public_id(uint64_t private_id_, uint64_t public_id_);
bool Load_private_id_and_public_id(uint64_t& private_id_, uint64_t& public_id_);
bool Connect_to_partner(std::vector<uint8_t>& recv_buf, std::vector<uint8_t>& send_buf, uint64_t private_id, uint64_t public_id, uint64_t partner_id, const unsigned char* pass_hash16);
uint64_t generate_ID(unsigned char *id);
char *get_home_dir();

#endif /* TOOLS_H */

