/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   Viewer.cpp
 * Author: smorodin
 * 
 * Created on July 4, 2023, 2:08 PM
 */



#include <memory>
#include <stdio.h>
#include <cstdlib>

#ifdef __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include "Viewer.h"
#include "NET/NET.h"
#include "GUI/GUIscreenBuffer.h"
#include "tools.h"


bool Register_client(std::vector<uint8_t>& recv_buf, std::vector<uint8_t>& send_buf, uint64_t& private_id, uint64_t& public_id);

void Viewer::start() {
    printf("------Viewer run\n");
    
    RECTANGLE rec(100, 100, 500, 500, 0, 255);
    InitGUIitems();
    
    window = gui->CreateWindow_( "Viewer", GUIwindow::RESIZED, rec, &gui_items, this);
    window->StaticUserInput = Viewer::StaticUserInput;
    window->test();
    
    gui->test();
    
    recv_buf.resize(510000);
    send_buf.resize(510000);

    net->gui_item_view = gui_items.get_item_by_id("viewer");
    net->gui_item_view->window = window;
    
    execute_is_run = true;
    execute_thread = new std::thread(&Viewer::execute, this);
}

void Viewer::wait() {
    while(need_stop == false) {
        sleep_();        
    }
}

bool Viewer::InitGUIitems() {

    gui_items.addd("viewer", "", GUIitem::ItemType::viewer, _FRAME(0, 0, 500, 500), nullptr, true, false);
    
    
    return true;
}

/*int viewer_main_net_loop(void *p, int sock) {
    Viewer *v;
    v = reinterpret_cast<Viewer *>(p);
    if(v != nullptr) {
        return v->main_net_loop(sock);
    }
    return 0;
}*/

/*int Viewer::main_net_loop(int sock) {
    
    while(need_stop == false) {
        

        
        sleep_();
        
    }
    return 0;
}*/

bool Register_client(std::vector<uint8_t>& recv_buf, std::vector<uint8_t>& send_buf, uint64_t& private_id, uint64_t& public_id) {
    
    sockaddr_in dest_addr;
    int recv_sz, send_sz;
    MY_AES aes;
    MY_CRC crc;
    MY_RSA rsa;
    rsa.init();
    
    PACKET_LEVEL0 *r0;
    PACKET_LEVEL1_1002_responce *r1;
    PACKET_LEVEL1_1003_responce *r1003;

    PACKET_LEVEL0 *p0;
    PACKET_LEVEL1_1002_request *p1;
    PACKET_LEVEL1_1003_request *p1003;
    
    ////////////////////////////////////////////////////////////////////////////
    // запросим ip на котором живет сервер для регистрации новых клиентов
    
    //zero_(send_buf.buf, 128);
    if( my_random(send_buf.data(), 128) == false ) return false;
    //PACKET_LEVEL0 *p0;
    //PACKET_LEVEL1_1002_request *p1;

    p0 = (PACKET_LEVEL0 *)send_buf.data();
    p0->packet_type = 1002;
    p1 = (PACKET_LEVEL1_1002_request *)p0->body;
    p1->sub_type = 101; // 101 - просим ip сервера для регистрации нового клиента
    p1->im_public_id = 0;
    p1->im_private_id = 0;

    zero_(p1->AES_pass, 32);
    if( my_random(p1->AES_pass, 16) == false ) return false;

    aes.set_key_16_byte(p1->AES_pass, L"Register_new_partner_on_proxy 1-aes");

    p0->crc32 = crc.calc(&send_buf[8], 120);

    rsa.encode_128_byte(send_buf.data(), send_buf.data());

    ////////////////////////////////////////////////////////////////////////////
    
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    
    unsigned int view_ip;
    view_ip = get_ip_view_visiator_com();
    
    if (view_ip == 0) {
        //send_udp("REQUEST_decodepass view_ip == 0");
        closesocket(sock);
        return false;
    }

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(SERVER_PORT);
    dest_addr.sin_addr.s_addr = view_ip;
    
    if(connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0)
    {
        return false;
    }

    setnonblocking(sock);
    
    send_sz = my_send(sock, (unsigned char *)send_buf.data(), 128);//, 0, "", &send__counter);//ok
    if(send_sz != 128) {
        closesocket(sock);
        return false;
    }

    do
    {
        recv_sz = my_recv(sock, recv_buf.data(), 128);//, &recv__counter);
        if(recv_sz < 0) {
            closesocket(sock);
            return false;
        }
        sleep_();
    } while(recv_sz == 0);
    aes.decrypt_128_byte(recv_buf.data());

    uint32_t crc32 = crc.calc(&recv_buf[8], 120);
    uint32_t ip_for_reg_new_partner = 0;
    
    r0 = (PACKET_LEVEL0 *)recv_buf.data();
    if (r0->crc32 == crc32)
    {
            r1 = (PACKET_LEVEL1_1002_responce *)r0->body;
            ip_for_reg_new_partner = r1->ip4;
    };

    if(ip_for_reg_new_partner < 100) {
        return false;
    }
    
    closesocket(sock);
    
    ////////////////////////////////////////////////////////////////////////////
    //
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(SERVER_PORT);
    dest_addr.sin_addr.s_addr = ip_for_reg_new_partner;
    
    if(connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0)
    {
        return false;
    }

    setnonblocking(sock);
    
    my_random(send_buf.data(), 128);

    p0 = (PACKET_LEVEL0 *)send_buf.data();
    p0->packet_type = 1003; // запрашиваем регистрацию нового партнера
    p1003 = (PACKET_LEVEL1_1003_request *)p0->body;

    zero_(p1003->AES_pass, 32);
    my_random(p1003->AES_pass, 16);

    aes.set_key_16_byte(p1003->AES_pass, L"Register_new_partner_on_proxy 2--aes");

    p0->crc32 = crc.calc(&send_buf[8], 120);

    rsa.encode_128_byte(send_buf.data(), send_buf.data());
    
    ////////////////////////////////////////////////////////////////////////////
    
    send_sz = my_send(sock, (unsigned char *)send_buf.data(), 128);//, 0, "", &send__counter);//ok
    if(send_sz != 128) {
        closesocket(sock);
        return false;
    }

    do
    {
        recv_sz = my_recv(sock, recv_buf.data(), 128);//, &recv__counter);
        if(recv_sz < 0) {
            closesocket(sock);
            return false;
        }
        sleep_();
    } while(recv_sz == 0);
    aes.decrypt_128_byte(recv_buf.data());
    
    crc32 = crc.calc(&recv_buf[8], 120);

    r0 = (PACKET_LEVEL0 *)recv_buf.data();
    if (r0->crc32 == crc32)
    {
            r1003 = (PACKET_LEVEL1_1003_responce *)r0->body;

            private_id = r1003->private_id;
            public_id = r1003->public_id;
            return true;
    };
    
    return false;
}

void Viewer::execute() {

    
    struct sockaddr_in addr;
    int sock, ff;
    ff = 0;
    while(need_stop == false) {

        if(private_id == 0 || public_id == 0) {
            Load_private_id_and_public_id(private_id, public_id);
            if(private_id == 0 || public_id == 0) {
                if(Register_client(recv_buf, send_buf, private_id, public_id)) {
                    Save_private_id_and_public_id(private_id, public_id);
                }
            }
            sleep_();
            continue;
        }
        
        MY_SHA3 sha3;
        
        if(private_id != 0 && public_id != 0) {
            uint64_t partner_id;
            unsigned char partner_pass[32];// = {"0123456789012345678901234567890"};
            zero_(partner_pass, 32);
            partner_pass[0] = '1';
            partner_pass[1] = '2';
            partner_pass[2] = '3';
            partner_pass[3] = '2';
            //partner_id = generate_ID((unsigned char *)"095-231-430");
            partner_id = generate_ID((unsigned char *)"417-082-313");
            
            
            for (int i = 0; i < 20; i++) {
                    for (int j = 0; j < 5000; j++) {
                            sha3.hash_32_byte(partner_pass, partner_pass);
                    };
            }
            
            net->Connect_to_partner(recv_buf, send_buf, private_id, public_id, partner_id, partner_pass);
        }
        
        sleep_();
    }
    execute_is_run = false;
    
}