/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   NET.h
 * Author: smorodin
 *
 * Created on July 15, 2023, 11:49 AM
 */

#ifndef NET_H
#define NET_H

#include <thread>
#include <string>
#include <vector>

#include "../GUI/GUIscreenBuffer.h"

class MY_AES;
class VECTOR_EX;
class GUIwindow;

#define view_visiator_com "view.visiator.com"
#define SERVER_PORT 443

#define PACKET_TYPE_request_screen 20001

#define PACKET_TYPE_responce_screen_ver11 20002
#define PACKET_TYPE_responce_screen_ver22 20053
#define PACKET_TYPE_responce_screen_ver33 20055

#define PACKET_TYPE_responce_screen_test 20003

#define PACKET_TYPE_request_start_screenflow_ver11 20004
#define PACKET_TYPE_request_start_screenflow_ver22 20052
#define PACKET_TYPE_request_start_screenflow_ver33 20054

#define PACKET_TYPE_responce_mouse_cursor_full_format 20005
#define PACKET_TYPE_responce_mouse_cursor_short_format 20006

#define PACKET_TYPE_send_event_from_client_to_server 20007

#define PACKET_TYPE_send_mouse_from_client_to_server 20011
#define PACKET_TYPE_send_keyboard_event_list_from_client_to_server 20012
#define PACKET_TYPE_request_userlogin_list 20013
#define PACKET_TYPE_responce_userlogin_list 20014

#define PACKET_TYPE_request_userCLIPBOARD_list 20015
#define PACKET_TYPE_responce_userCLIPBOARD_list 20016

#define PACKET_TYPE_need_create_folder 20030
#define PACKET_TYPE_need_create_folder_commit 20031

#define PACKET_TYPE_transfer_file_ROUND_1 20032
#define PACKET_TYPE_transfer_file_ROUND_1_commit 20033
#define PACKET_TYPE_transfer_file_ROUND_2 20034
#define PACKET_TYPE_transfer_file_ROUND_2_commit 20035
#define PACKET_TYPE_transfer_file_ROUND_2_commit_finish 20036

#define PACKET_TYPE_transfer_file_ROUND_1_cancel_transfer 20037

#define PACKET_TYPE_need_delete_files_list 20040
#define PACKET_TYPE_need_delete_files_list_commit 20041
#define PACKET_TYPE_need_delete_files_list_intermediate 20042
#define PACKET_TYPE_need_delete_files_list_FINISH 20043
#define PACKET_TYPE_need_delete_files_list_CANCEL 20044

#define PACKET_TYPE_request_filefolder_stat 20045
#define PACKET_TYPE_responce_filefolder_stat 20046

#define PACKET_TYPE_request_filefolder_part 20047
#define PACKET_TYPE_responce_filefolder_part 20048

#define PACKET_TYPE_request_file_list_1 20017
#define PACKET_TYPE_responce_file_list_1 20018

#define PACKET_TYPE_request_file_list_2 20049
#define PACKET_TYPE_responce_file_list_2 20050

#define PACKET_TYPE_TRANSFER_from_SRV_to_CLI_CANCELED 20051

#define filefolder_part_buf_max_size 150000


#define ROUND_1_RESULT_OK_target_file_created 1
#define ROUND_1_RESULT_ERR_target_file_already_exists 2
#define ROUND_1_RESULT_ERR_fatal 3
#define ROUND_1_RESULT_OK_target_file_finish_ZEROSIZE 4


// 20052 already used
// 20053 already used
// 20054 already used
// 20055 already used

struct PACKET_LEVEL0
{
	unsigned int zero;
	unsigned int crc32;
	unsigned int sol;
	unsigned int packet_type;
	unsigned int sol2;
	unsigned char body[108];
};
struct PACKET_LEVEL1_1002_request // get_proxy_ip
{

	unsigned long long sub_type; // 101 - просим ip сервера для регистрации нового партнера
	// 102 - просим ip сервера для подключения сервера
	// 103 - просим ip сервера на котором зареган сервер требуемого партнера

	unsigned long long im_public_id;
	unsigned long long im_private_id;
	unsigned char AES_pass[32];
	unsigned char body[52];
};
struct PACKET_LEVEL1_1002_responce // get_proxy_ip
{
	unsigned int  sub_type;
	unsigned int  ip4;
	
	unsigned char body[100];
};
struct PACKET_LEVEL1_1003_request // register neww partner
{
	unsigned char AES_pass[32];
	unsigned char body[76];
};
struct PACKET_LEVEL1_1003_responce // register neww partner
{
	unsigned long long public_id;
	unsigned long long private_id;
	unsigned char body[92];
};
struct PACKET_LEVEL1_1004_request // server connecting
{
	unsigned long long my_public_id;
	unsigned long long my_private_id;
	unsigned long long server_ver;
	unsigned char AES_passs[32];
	unsigned char body[52];
};
struct PACKET_LEVEL1_1004_responce // server connecting
{
	unsigned long long result;
	unsigned int  key;
	unsigned int  current_visiator_exe_ver;
	unsigned char body[92];
};
struct PACKET_LEVEL1_1005_request // client connecting
{
	unsigned long long client_public_id;
	unsigned long long client_private_id;
	unsigned long long client_ver;
	unsigned long long partner_public_id;
	unsigned char AES_passs[32];
	unsigned char body[44];
};
struct PACKET_LEVEL1_1005_responce // client connecting
{
	unsigned long long result;
	unsigned int  key;
	unsigned int  srv_ver;
	unsigned char body[92];
};
struct PACKET_LEVEL1_1006_responce // server connecting
{
	unsigned long long result;
	unsigned int  sol;
	unsigned char body[96];
};
struct PACKET_LEVEL1_1006_request // server connecting
{
	unsigned int key;
	unsigned int sol;
	unsigned char body[100];
};
struct PACKET_LEVEL1_1007_request // decode pass to attributes
{
	unsigned char pass_unic_id[16];
	unsigned char pin[4];
	unsigned char pin2[4];
	unsigned int  get_or_change; // 1 - check pin. 2 - change pin
	unsigned int  sol;
	unsigned char AES_pass[32];
	unsigned char body[44];


	/*unsigned char pass_unic_id[16];
	unsigned char pin[4];
	unsigned int  sol;
	unsigned char AES_pass[32];
	unsigned char body[52];*/
};
struct PACKET_LEVEL1_1007_responce
{
	unsigned char pass_to_decode_attr_part1[8];
	unsigned int  sol1;
	unsigned int  sol2;
	unsigned char pass_to_decode_attr_part2[8];
	unsigned int  result;
	unsigned int  is_prem;
	unsigned char body[72];
};
struct PACKET_LEVEL1_1008_request // get exe file
{
	unsigned int file_type; // 201 - visiator.exe 
	unsigned int fragment_idx;
	unsigned long long im_public_id;
	unsigned long long im_private_id;
	unsigned char AES_pass[32];
	unsigned int  sol;
	unsigned char body[52];

};
struct PACKET_LEVEL1_1008_responce {
	unsigned int sub_type;
	unsigned int fragment_idx;
	unsigned int fragment_size;
	unsigned int fragment_crc;
	unsigned int file_ver;
	unsigned char body[1088];
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class NET {
public:
    GUIitem* gui_item_view;
    bool need_start_screenflow_from_server;

    std::unique_ptr<SCREEN_BUFFER> screen1;
    
    void analiz_command(uint8_t* buf, uint32_t len);
    bool detect_full_command(VECTOR_EX& buf);
    int  Client_Main_Loop(int sock, std::vector<uint8_t>& recv_buf, std::vector<uint8_t>& send_buf, MY_AES& aes_partner);
    bool Connect_to_partner(std::vector<uint8_t>& recv_buf, std::vector<uint8_t>& send_buf, uint64_t private_id, uint64_t public_id, uint64_t partner_id, const unsigned char* pass_hash16);
    
    NET() : need_start_screenflow_from_server(false), screen1(std::make_unique<SCREEN_BUFFER>(nullptr)), gui_item_view(nullptr) {
        
    }
    virtual ~NET() {
        
    };
private:

};

#endif /* NET_H */

