/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   Viewer.h
 * Author: smorodin
 *
 * Created on July 4, 2023, 2:08 PM
 */

#ifndef VIEWER_H
#define VIEWER_H

#include <memory>
#include <stdio.h>
#include <cstdlib>

#include "GUI/GUI.h"
#include "NET/NET.h"
#include "GUI/GUIlowlevel.h"
#include "GUI/GUIitem.h"
#include "CRYPTO/Crypto.h"
#include "tools.h"

class Viewer {
public:
    
    uint64_t private_id, public_id;
    
    std::vector<uint8_t> recv_buf, send_buf;
    std::unique_ptr<NET> net;
    std::shared_ptr<GUI> gui;
    GUIwindow* window;
    bool need_stop;
    
    GUIitems gui_items;
    
    void start();
    void wait();
    void stop() { need_stop = true; };
    
    bool init();
    bool InitGUIitems();
    
    bool execute_is_run = false;
    std::thread* execute_thread = nullptr;
    void execute();
    
    static void StaticUserInput(void* parent, WindowUserEvent* window_user_event) {
        printf("StaticUserInput\n");
        ((Viewer *)parent)->UserInput(window_user_event);   
    }
    void all_edit_begin_false() {
        for(int i=0; i<gui_items.items.size(); i++) {
            if(gui_items.items[i].is_edit_begin) {
                gui_items.items[i].is_edit_begin = false;
            }
        }
    }
    void UserInput(WindowUserEvent* window_user_event) {
        printf("UserInput %d %d \n", window_user_event->x, window_user_event->y);
        
        
    }
    void generate_packet_ReviestIP(std::vector<uint8_t>& buf) {
        //unsigned int view_ip;
	//view_ip = get_ip_view_visiator_com();    

        MY_AES aes;
	MY_CRC crc;
	MY_RSA rsa;
	rsa.init();
        
        PACKET_LEVEL0 *p0;
	PACKET_LEVEL1_1002_request *p1;
        
        p0 = (PACKET_LEVEL0 *)buf.data();
	p0->packet_type = 1002;
	p1 = (PACKET_LEVEL1_1002_request *)p0->body;
	p1->sub_type = 101; // 101 - просим ip сервера для регистрации нового партнера
	p1->im_public_id = 0;
	p1->im_private_id = 0;

	//for (int i = 0; i < 16; i++) p1->AES_pass[i] = '0' + i;
	my_random(p1->AES_pass, 16);

	aes.set_key_16_byte(p1->AES_pass, L"Register_new_partner_on_proxy 1-aes");

	p0->crc32 = crc.calc(&buf[8], 120);

	rsa.encode_128_byte(buf.data(), buf.data());
        
    }
    bool need_register_client() {
        return true;
    }
    
    //int main_net_loop(int sock);
    
    Viewer(std::shared_ptr<GUI> gui) : gui(gui), need_stop(false), private_id(0L), public_id(0L), net(std::make_unique<NET>()) {
        
    }
    Viewer() = delete;
    Viewer(const Viewer& orig) = delete;
    Viewer(Viewer&& orig) = delete;
    
    virtual ~Viewer() {};
private:

};

#endif /* VIEWER_H */

