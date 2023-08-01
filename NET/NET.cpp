/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   NET.cpp
 * Author: smorodin
 * 
 * Created on July 15, 2023, 11:49 AM
 */

#include "NET.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../tools.h"

#include "../CRYPTO/Crypto.h"
#include "../GUI/GUIwindow.h"

bool Register_new_client();

extern int g_1;

int my_read(int sock, class net_buf& read_buf) {
    
    return 0;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool Register_new_partner_on_proxy() {

    return true;
}
    /*
	uint64_t recv__counter = 0, send__counter = 0;

	//send_udp("Register_new_partner_on_proxy()...");

	if (app_attributes.is_service) {
		//send_udp("Save_private_id_and_public_id_from_SERVICE_registry...");
		if (Save_private_id_and_public_id_from_SERVICE_registry(0, 0) == false) {
			//send_udp("Register_new_partner_on_proxy() registry error");
			my_Slip(1000);
			return false;
		}
	}
	else {
		//send_udp("Save_private_id_and_public_id_from_USER_registry...");
		if (Save_private_id_and_public_id_from_USER_registry(0, 0) == false) {
			//send_udp("Register_new_partner_on_proxy() registry error");
			my_Slip(1000);
			return false;
		}
	}

	unsigned int ip_for_reg_new_partner;
	unsigned int crc32;
	unsigned char bb[500], xx[500];// , zz[500];;
	PACKET_LEVEL0 *r0;
	PACKET_LEVEL1_1002_responce *r1;
	PACKET_LEVEL1_1003_responce *r1003;

	PACKET_LEVEL0 *p0;
	PACKET_LEVEL1_1002_request *p1;
	PACKET_LEVEL1_1003_request *p1003;

	ip_for_reg_new_partner = 0;

	SOCKET sos;
	sockaddr_in dest_addr;

	MY_AES aes;
	MY_CRC crc;
	MY_RSA rsa;
	rsa.init();

	int res, snd;

	if (INVALID_SOCKET == (sos = socket(AF_INET, SOCK_STREAM, 0)))
	{ //3
		//send_udp("Register_new_partner_on_proxy status = create socket error");
		return false;
	} //3

	unsigned int view_ip;
	view_ip = get_ip_view_visiator_com();

	if (view_ip == 0) {
		//send_udp("REQUEST_decodepass view_ip == 0");
		closesocket(sos);
		return false;
	}


	PROXY_LIST *proxy_list; // app_attributes+

	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(SERVER_PORT);
	dest_addr.sin_addr.S_un.S_addr = view_ip;//inet_addr("139.162.182.46");// proxy_address );
	//status = "connect...";
	if (SOCKET_ERROR == connect(sos, (sockaddr*)&dest_addr, sizeof(dest_addr))) // proxy+!
	{//4
		closesocket(sos);
		proxy_list = app_attributes.proxy_list;
		if (proxy_list != NULL) {
			sos = proxy_list->try_connect(view_ip);//(inet_addr("139.162.182.46")); // ������� ������������ ����� ������ +
		};
		if (sos == 0) {
			closesocket(sos);
			//send_udp("Register_new_partner_on_proxy err connect");
			return false;
		};
	}//4

		//status = "connect ok";

	setnonblocking(sos);


	zero_unsigned_char(bb, 128);
	//PACKET_LEVEL0 *p0;
	//PACKET_LEVEL1_1002_request *p1;

	p0 = (PACKET_LEVEL0 *)bb;
	p0->packet_type = 1002;
	p1 = (PACKET_LEVEL1_1002_request *)p0->body;
	p1->sub_type = 101; // 101 - ������ ip ������� ��� ����������� ������ ��������
	p1->im_public_id = 0;
	p1->im_private_id = 0;

	//for (int i = 0; i < 16; i++) p1->AES_pass[i] = '0' + i;
	my_random(p1->AES_pass, 16);

	aes.set_key_16_byte(p1->AES_pass, "Register_new_partner_on_proxy 1-aes");

	p0->crc32 = crc.calc(&bb[8], 120);

	rsa.encode_128_byte(bb, bb);

	//status = "send_...";
	snd = 0;
	do
	{
		snd = my_send(sos, (unsigned char *)bb, 128, 0, "", &send__counter);//ok
	} while (snd != 128);

	//status = String("read... ");

	do
	{
		res = my_recv(sos, xx, 128, &recv__counter);
	} while (res != 128 && res != -1);
	aes.decrypt_128_byte(xx);

	//2019+ crc = CRC32_short(&xx[4], 124);
	crc32 = crc.calc(&xx[8], 120);
	//PACKET_LEVEL0 *r0;
	//PACKET_LEVEL1_1002_responce *r1;

	r0 = (PACKET_LEVEL0 *)xx;
	if (r0->crc32 == crc32)
	{
		//status = "crc ok";

		r1 = (PACKET_LEVEL1_1002_responce *)r0->body;

		ip_for_reg_new_partner = r1->ip4;
		//status = String(idx) + "read ok pause 3000 " + String(res);

		//::Sleep(300);
		//status = "close...";
	}
	else {
		//status = "crc err";
	};
	struct linger lng = { 1, 0 };
	setsockopt(sos, SOL_SOCKET, SO_LINGER, (char *)(&lng), sizeof(struct linger));

	int ee;
	ee = shutdown(sos, SD_BOTH);
	closesocket(sos);



	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	if (ip_for_reg_new_partner != 0 && ip_for_reg_new_partner != 0xffffffff) { // 3333
		if (INVALID_SOCKET == (sos = socket(AF_INET, SOCK_STREAM, 0)))
		{ //3
			//status = "create socket error";
			//send_udp("Register_new_partner_on_proxy err 4");
			return false;
		} //3




		dest_addr.sin_family = AF_INET;
		dest_addr.sin_port = htons(SERVER_PORT);
		dest_addr.sin_addr.S_un.S_addr = ip_for_reg_new_partner;// inet_addr("139.162.182.46");// proxy_address );
		//status = "connect...";
		if (SOCKET_ERROR == connect(sos, (sockaddr*)&dest_addr, sizeof(dest_addr))) // proxy+
		{//4
			closesocket(sos);
			sos = 0;
			proxy_list = app_attributes.proxy_list;
			if (proxy_list != NULL) {
				sos = proxy_list->try_connect(ip_for_reg_new_partner); // ������� ������������ ����� ������ +
			};
			if (sos == 0) {
				closesocket(sos);
				sos = 0;
				//send_udp("Register_new_partner_on_proxy err 5");
				return false;
			};
		}//4

			//status = "connect ok";

		setnonblocking(sos);


		zero_unsigned_char(bb, 128);
		//PACKET_LEVEL0 *p0;
		//PACKET_LEVEL1_1002_request *p1;

		p0 = (PACKET_LEVEL0 *)bb;
		p0->packet_type = 1003; // ����������� ����������� ������ ��������
		p1003 = (PACKET_LEVEL1_1003_request *)p0->body;


		//for (int i = 0; i < 16; i++) p1003->AES_pass[i] = '0' + i;
		my_random(p1003->AES_pass, 16);

		aes.set_key_16_byte(p1003->AES_pass, "Register_new_partner_on_proxy 2--aes");

		p0->crc32 = crc.calc(&bb[8], 120);

		rsa.encode_128_byte(bb, bb);

		//status = "send_...";
		snd = 0;
		do
		{
			snd = my_send(sos, (unsigned char *)bb, 128, 0, "", &send__counter);//ok
		} while (snd != 128); // WTF???

		//status = String("read... ");

		do
		{
			res = my_recv(sos, xx, 128, &recv__counter);
		} while (res != 128 && res != -1);
		aes.decrypt_128_byte(xx);

		//2019+ crc = CRC32_short(&xx[4], 124);
		crc32 = crc.calc(&xx[8], 120);
		//PACKET_LEVEL0 *r0;
		//PACKET_LEVEL1_1002_responce *r1;

		r0 = (PACKET_LEVEL0 *)xx;
		if (r0->crc32 == crc32)
		{
			//status = "crc ok";

			r1003 = (PACKET_LEVEL1_1003_responce *)r0->body;

			if (app_attributes.is_service) {
				//send_udp("Save_private_id_and_public_id_from_SERVICE_registry...");
				Save_private_id_and_public_id_from_SERVICE_registry(r1003->private_id, r1003->public_id);
			}
			else {
				//send_udp("Save_private_id_and_public_id_from_USER_registry...");
				Save_private_id_and_public_id_from_USER_registry(r1003->private_id, r1003->public_id);
			}
			//status = String(idx) + "read ok pause 3000 " + String(res);

			//::Sleep(300);
			//status = "close...";
		}
		else {
			//status = "crc err";
		};
		struct linger lng = { 1, 0 };
		setsockopt(sos, SOL_SOCKET, SO_LINGER, (char *)(&lng), sizeof(struct linger));

		int ee;
		ee = shutdown(sos, SD_BOTH);
		closesocket(sos);


	}//3333

	//send_udp("Register_new_partner_on_proxy() finish");
	return true;
}
*/

bool Register_new_client(std::vector<uint8_t>& recv_buf, std::vector<uint8_t>& send_buf) {

	uint64_t recv__counter = 0, send__counter = 0;

	//send_udp("Register_new_partner_on_proxy()...");

	/* 2023 if (app_attributes.is_service) {
		//send_udp("Save_private_id_and_public_id_from_SERVICE_registry...");
		if (Save_private_id_and_public_id_from_SERVICE_registry(0, 0) == false) {
			//send_udp("Register_new_partner_on_proxy() registry error");
			my_Slip(1000);
			return false;
		}
	}
	else {
		//send_udp("Save_private_id_and_public_id_from_USER_registry...");
		if (Save_private_id_and_public_id_from_USER_registry(0, 0) == false) {
			//send_udp("Register_new_partner_on_proxy() registry error");
			my_Slip(1000);
			return false;
		}
	}*/

	unsigned int ip_for_reg_new_partner;
	unsigned int crc32;
	unsigned char bb[500], xx[500];// , zz[500];;
	PACKET_LEVEL0 *r0;
	PACKET_LEVEL1_1002_responce *r1;
	PACKET_LEVEL1_1003_responce *r1003;

	PACKET_LEVEL0 *p0;
	PACKET_LEVEL1_1002_request *p1;
	PACKET_LEVEL1_1003_request *p1003;

	ip_for_reg_new_partner = 0;

	int sock;
	sockaddr_in dest_addr;

	MY_AES aes;
	MY_CRC crc;
	MY_RSA rsa;
	rsa.init();

	int res, snd;

        sock = socket(AF_INET, SOCK_STREAM, 0);
	
	unsigned int view_ip;
	view_ip = get_ip_view_visiator_com();

	if (view_ip == 0) {
		//send_udp("REQUEST_decodepass view_ip == 0");
		closesocket(sock);
		return false;
	}


	// 2023 PROXY_LIST *proxy_list; // app_attributes+

	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(SERVER_PORT);
	//dest_addr.sin_addr.S_un.S_addr = view_ip;//inet_addr("139.162.182.46");// proxy_address );
        dest_addr.sin_addr.s_addr = view_ip;
	//status = "connect...";
        
        
        
        if(connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0)
        {//4
		/* 2023 closesocket(sos);
		proxy_list = app_attributes.proxy_list;
		if (proxy_list != NULL) {
			sos = proxy_list->try_connect(view_ip);//(inet_addr("139.162.182.46")); // ������� ������������ ����� ������ +
		};
		if (sos == 0) {
			closesocket(sos);
			//send_udp("Register_new_partner_on_proxy err connect");
			return false;
		};*/
            return false;
	}//4

		//status = "connect ok";

	setnonblocking(sock);


	zero_(send_buf);
	//PACKET_LEVEL0 *p0;
	//PACKET_LEVEL1_1002_request *p1;

	p0 = (PACKET_LEVEL0 *)send_buf.data();
	p0->packet_type = 1002;
	p1 = (PACKET_LEVEL1_1002_request *)p0->body;
	p1->sub_type = 101; // 101 - просим ip сервера для регистрации нового партнера
	p1->im_public_id = 0;
	p1->im_private_id = 0;

	//for (int i = 0; i < 16; i++) p1->AES_pass[i] = '0' + i;
	my_random(p1->AES_pass, 16);

	aes.set_key_16_byte(p1->AES_pass, L"Register_new_partner_on_proxy 1-aes");

	p0->crc32 = crc.calc(&send_buf[8], 120);

	rsa.encode_128_byte(send_buf.data(), send_buf.data());

	//status = "send_...";
	
	
        snd = my_send(sock, send_buf.data(), 128);//, 0, "", &send__counter);//ok
	if(snd != 128) {
            closesocket(sock);
            return false;
        }

        do
        {
            res = my_recv(sock, recv_buf.data(), 128);//, &recv__counter);
            if(res != 128) {
                closesocket(sock);
                return false;
            }
            sleep_();
        } while(res == 0);
	aes.decrypt_128_byte(recv_buf.data());

	//2019+ crc = CRC32_short(&xx[4], 124);
	crc32 = crc.calc(&recv_buf[8], 120);
	//PACKET_LEVEL0 *r0;
	//PACKET_LEVEL1_1002_responce *r1;

	r0 = (PACKET_LEVEL0 *)recv_buf.data();
	if (r0->crc32 == crc32)
	{
		//status = "crc ok";

		r1 = (PACKET_LEVEL1_1002_responce *)r0->body;

		ip_for_reg_new_partner = r1->ip4;
		//status = String(idx) + "read ok pause 3000 " + String(res);

		//::Sleep(300);
		//status = "close...";
	}
	else {
		//status = "crc err";
	};
        
        struct linger lng = { 1, 0 };
        setsockopt(sock, SOL_SOCKET, SO_LINGER, (char *)(&lng), sizeof(struct linger));

        int ee = shutdown(sock, SHUT_RDWR);// SD_BOTH);
        closesocket(sock);
  
        return -1; }
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/*
	if (ip_for_reg_new_partner != 0 && ip_for_reg_new_partner != 0xffffffff) { // 3333
		if (INVALID_SOCKET == (sos = socket(AF_INET, SOCK_STREAM, 0)))
		{ //3
			//status = "create socket error";
			//send_udp("Register_new_partner_on_proxy err 4");
			return false;
		} //3




		dest_addr.sin_family = AF_INET;
		dest_addr.sin_port = htons(SERVER_PORT);
		dest_addr.sin_addr.S_un.S_addr = ip_for_reg_new_partner;// inet_addr("139.162.182.46");// proxy_address );
		//status = "connect...";
		if (SOCKET_ERROR == connect(sos, (sockaddr*)&dest_addr, sizeof(dest_addr))) // proxy+
		{//4
			closesocket(sos);
			sos = 0;
			proxy_list = app_attributes.proxy_list;
			if (proxy_list != NULL) {
				sos = proxy_list->try_connect(ip_for_reg_new_partner); // ������� ������������ ����� ������ +
			};
			if (sos == 0) {
				closesocket(sos);
				sos = 0;
				//send_udp("Register_new_partner_on_proxy err 5");
				return false;
			};
		}//4

			//status = "connect ok";

		setnonblocking(sos);


		zero_unsigned_char(bb, 128);
		//PACKET_LEVEL0 *p0;
		//PACKET_LEVEL1_1002_request *p1;

		p0 = (PACKET_LEVEL0 *)bb;
		p0->packet_type = 1003; // запрашиваем регистрацию нового партнера
		p1003 = (PACKET_LEVEL1_1003_request *)p0->body;


		//for (int i = 0; i < 16; i++) p1003->AES_pass[i] = '0' + i;
		my_random(p1003->AES_pass, 16);

		aes.set_key_16_byte(p1003->AES_pass, "Register_new_partner_on_proxy 2--aes");

		p0->crc32 = crc.calc(&bb[8], 120);

		rsa.encode_128_byte(bb, bb);

		//status = "send_...";
		snd = 0;
		do
		{
			snd = my_send(sos, (unsigned char *)bb, 128, 0, "", &send__counter);//ok
		} while (snd != 128); // WTF???

		//status = String("read... ");

		do
		{
			res = my_recv(sos, xx, 128, &recv__counter);
		} while (res != 128 && res != -1);
		aes.decrypt_128_byte(xx);

		//2019+ crc = CRC32_short(&xx[4], 124);
		crc32 = crc.calc(&xx[8], 120);
		//PACKET_LEVEL0 *r0;
		//PACKET_LEVEL1_1002_responce *r1;

		r0 = (PACKET_LEVEL0 *)xx;
		if (r0->crc32 == crc32)
		{
			//status = "crc ok";

			r1003 = (PACKET_LEVEL1_1003_responce *)r0->body;

			if (app_attributes.is_service) {
				//send_udp("Save_private_id_and_public_id_from_SERVICE_registry...");
				Save_private_id_and_public_id_from_SERVICE_registry(r1003->private_id, r1003->public_id);
			}
			else {
				//send_udp("Save_private_id_and_public_id_from_USER_registry...");
				Save_private_id_and_public_id_from_USER_registry(r1003->private_id, r1003->public_id);
			}
			//status = String(idx) + "read ok pause 3000 " + String(res);

			//::Sleep(300);
			//status = "close...";
		}
		else {
			//status = "crc err";
		};
		struct linger lng = { 1, 0 };
		setsockopt(sos, SOL_SOCKET, SO_LINGER, (char *)(&lng), sizeof(struct linger));

		int ee;
		ee = shutdown(sos, SD_BOTH);
		closesocket(sos);


	}//3333

	//send_udp("Register_new_partner_on_proxy() finish");
	return true;
}
*/

bool NET::Connect_to_partner(std::vector<uint8_t>& recv_buf, std::vector<uint8_t>& send_buf, uint64_t private_id, uint64_t public_id, uint64_t partner_id, const unsigned char* pass_hash16) {
    
    sockaddr_in dest_addr;
    MY_AES aes_partner, aes_proxy;
    MY_CRC crc;
    MY_RSA rsa;
    rsa.init();

    PACKET_LEVEL0 *p0, *r0;
    PACKET_LEVEL1_1005_request *p1;
    
    aes_partner.set_key_16_byte(pass_hash16, L"NCS Connect_to_server 1 aes_partner");

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
    
    
		my_random(send_buf.data(), 128);

		p0 = (PACKET_LEVEL0 *)send_buf.data();
		p0->packet_type = 1005;
		p1 = (PACKET_LEVEL1_1005_request *)p0->body;

		p1->client_public_id = public_id;
		p1->client_private_id = private_id;
		p1->client_ver = 0x1234;
		p1->partner_public_id = partner_id;//generate_id(461, 50, 164);

                zero_(p1->AES_passs, 32);
		my_random(p1->AES_passs, 16);

		aes_proxy.set_key_16_byte(p1->AES_passs, L"NCS Connect_to_server 2 aes_proxy");

		p0->crc32 = crc.calc(&send_buf[8], 120);

		rsa.encode_128_byte(send_buf.data(), send_buf.data());

		int send_sz = 0, recv_sz = 0;
                send_sz = my_send(sock, send_buf.data(), 128);
		if (send_sz != 128) {
                    closesocket(sock);
                    return false;
                }


		do
		{
                    recv_sz = my_recv(sock, recv_buf.data(), 128);
                    if(recv_sz < 0) {
                        closesocket(sock);
                        return false;
                    }
                    sleep_();
		} while (recv_sz == 0);
                
		aes_proxy.decrypt_128_byte(recv_buf.data());

		uint32_t crc32 = crc.calc(&recv_buf[8], 120);
                
                r0 = (PACKET_LEVEL0 *)recv_buf.data();
		if (r0->crc32 == crc32)
		{
			PACKET_LEVEL1_1005_responce *s1005;
			s1005 = (PACKET_LEVEL1_1005_responce *)r0->body;

			if (s1005->result == 2) { // партнер найден
                            	
                            uint32_t SERVER_VER = s1005->srv_ver;

                            //if (viewer != nullptr) {
                            //        viewer->set_SERVER_VER(SERVER_VER);
                            //}

                            uint32_t key_for_commit_connection = s1005->key;

                            //********************************************
                            zero_(send_buf.data(), 128);

                            PACKET_LEVEL1_1006_request *r1006;
                            r1006 = (PACKET_LEVEL1_1006_request *)send_buf.data();

                            r1006->key = key_for_commit_connection;
                            r1006->sol = get_sol();

                            aes_proxy.encrypt_16_byte(send_buf.data());
                            //********************************************

                            send_sz = my_send(sock, (unsigned char *)send_buf.data(), 16);

                            if (send_sz != 16) {

                            }

                            zero_(recv_buf.data(), 128);

                            do
                            {
                                recv_sz = my_recv(sock, recv_buf.data(), 16);
                                if(recv_sz < 0) {
                                    closesocket(sock);
                                    return false;
                                }
                                sleep_();
                            } while (recv_sz == 0);
                            aes_proxy.decrypt_16_byte(recv_buf.data());

                            PACKET_LEVEL1_1006_responce *s1006;
                            s1006 = (PACKET_LEVEL1_1006_responce *)recv_buf.data();

                            if (s1006->result == 1022) { // прокси подтверждает, что мы установили соединение с партнером
                                
                                bool connection_to_partner_established = true;
                            
                                need_start_screenflow_from_server = true;
                                
                                int res = Client_Main_Loop(sock, recv_buf, send_buf, aes_partner);
                                
                                connection_to_partner_established = false;
                                if (res == -1) { // pass incorrect
                                        int return_result = -1;
                                }
                            }   
                        } else { // партнер не найден
                            printf("partner not found\n");
                            sleep_(3000);
                        }
		}
                
                return true;
                
}

int NET::Client_Main_Loop(int sock, std::vector<uint8_t>& recv_buf, std::vector<uint8_t>& send_buf, MY_AES& aes_partner) {
    
    char ss[100];
    
    VECTOR_EX encoded_read_buffer;
    VECTOR_EX decoded_read_buffer;
    uint32_t last_commit_scr_id = 0;
    MY_CRC crc;
    
    bool local_stop = false;
    
    
    while (local_stop == false) {
        
        int res = my_recv(sock, recv_buf.data(), 500000);
	
        if (res > 0) { // arrived
		
                //ss[0] = 0;
                //sprintf_s(ss, 90, "READ %d", res);
                //sudp(ss);

                //read_buffer.add_to_encoded_buffer(aes_partner, recv_buf.data(), res);
                
                encoded_read_buffer.add(recv_buf.data(), res);
                
                ////////////////////////////////////////////////////////////////
                
                if (encoded_read_buffer.pos >= 16) {

                    int j = 0;
                    while (j + 16 <= encoded_read_buffer.pos) {
                            
                            aes_partner.decrypt_16_byte(encoded_read_buffer.buf + j);
                            decoded_read_buffer.add(encoded_read_buffer.buf + j, 16);
                            j += 16;
                    }

                    if (j == encoded_read_buffer.pos) {
                            encoded_read_buffer.pos = 0;
                    } else {
                        int i = 0;
                        int jj = j;
                        while (j < encoded_read_buffer.pos) {
                                encoded_read_buffer.val[i++] = encoded_read_buffer.val[j++];
                        }
                        encoded_read_buffer.pos -= jj;
                    }
                }
                while (detect_full_command(decoded_read_buffer) == true) //  <<<<==================
                {

                }
                
        }
        if (res < 0) { // disconnect 
            local_stop = true;
        }
        if (res == 0) {
            //detect_full_command(decoded_read_buffer);
            sleep_();
        };
        
        unsigned char *cmd;
	unsigned int *sz, *crc32, *type, *sol, *old_screen_id, *session_no, *reserv3, *reserv4, *ii;
        
        if ( need_start_screenflow_from_server == true) {
            
			need_start_screenflow_from_server = false;
	
			cmd = (unsigned char *)send_buf.data();
			sz = (unsigned int *)&(cmd[0]);
			crc32 = (unsigned int *)&(cmd[4]);
			type = (unsigned int *)&(cmd[8]);
			sol = (unsigned int *)&(cmd[12]);
			old_screen_id = (unsigned int *)&(cmd[16]);
			session_no = (unsigned int *)&(cmd[20]);
			reserv3 = (unsigned int *)&(cmd[24]);
			reserv4 = (unsigned int *)&(cmd[28]);

			*sz = 32;
			*crc32 = 0;
			*type = PACKET_TYPE_request_start_screenflow_ver11; // need_start_screenflow_from_server_FORMAT_VER;
			*sol = get_sol();
			*old_screen_id = 0;
			*session_no = 0;
			*reserv3 = last_commit_scr_id;
			/*if (viewer == NULL) {
				*reserv4 = 0;
			}
			else {
				if (viewer->is_sync_clipboards) {
					*reserv4 = 1;
				}
				else {
					*reserv4 = 2;
				}
			}*/

			*crc32 = crc.calc(&(cmd[8]), (8 + 16));

			aes_partner.encrypt_stream(cmd, 32);

			//sudp("CLIENT send recuest screen...");

			int send_sz = my_send(sock, (unsigned char *)send_buf.data(), 32);//, 0, "", &send__counter);
			if (send_sz > 0) {
                            //send__counter__time = GetTickCount();
			};
			//DT_request_screen = GetTickCount();
			//DT_delay_responce_screen = DT_request_screen - DT_responce_screen;

	};
        
    }
    return 0;
}

bool NET::detect_full_command(VECTOR_EX& buf) {
    if(buf.pos < 16) return false;
    
    uint32_t *sz, *crc;
    sz = (unsigned int *)&(buf.val[0]);

    crc = (unsigned int *)&(buf.val[4]);

    ENCODED_SCREEN_8bit_header *header;
    header = (ENCODED_SCREEN_8bit_header *)&(buf.buf[0]);
    
    //printf("detect_full_command %d %d\n", *sz, buf.pos);
    
    if (*sz > 3000000) {
            //ALOG("detect_full_command_in_to_low_level_buffer whf?? *sz > 3000000");
            //send_udp("WARNING detect_full_command_in_to_low_level_buffer whf?? *sz > 3000000");
            // 2019 10 need_disconnect = true;
            //return false;
    }

    if (*sz > buf.pos) {
            //if (MODE == MODE_server) set_status(L"not complete command ");
            //ALOG("not complete command");
            //fatal_error("detect_full_command_in_to_low_level_buffer *sz > low_level_buffer_read_idx");
            return false;
    }
    
    analiz_command(buf.buf, *sz);
    
    if (*sz == buf.pos) {
            //ALOG("@@@@@@@@@@@@@    CLEAN ALL %d ", low_level_buffer_read_idx );
            buf.pos = 0;
            //ALOG("2---");
            return false;
    }
    
    unsigned int i, j, jj;
    i = 0;
    j = *sz;
    jj = *sz;

    while (j < buf.pos) {
        buf.val[i++] = buf.val[j++];
    }
    buf.pos -= jj;
    
    return false;
}

void NET::analiz_command(uint8_t* buf, uint32_t len) {
    if(len < 16) return;
    
    unsigned int *sz, *crc, *type, *ii, i;// , *old_screen_id;

    sz = (unsigned int *)&(buf[0]);
    crc = (unsigned int *)&(buf[4]);
    type = (unsigned int *)&(buf[8]);
    
    ENCODED_SCREEN_8bit_header *header;

    if ( *type == PACKET_TYPE_responce_screen_ver11 ||
             *type == PACKET_TYPE_responce_screen_ver22 ||
             *type == PACKET_TYPE_responce_screen_ver33

            ) {

            //recv_responce_screen = GetTickCount();

            need_start_screenflow_from_server = true;

            header = (ENCODED_SCREEN_8bit_header *)&(buf[0]);

            //printf("screen_id = %d %d:%d %d\n", header->screen_id, header->mouse_x, header->mouse_y, *sz);
            
            if (header->mouse_cursor_type_id != 0) {
                    //char ss[500];
                    //sprintf_ s(ss, 490, "mouse_cursor_type_id = %d ", header->mouse_cursor_type_id);
                    //send_udp(ss);
            }
            screen1->unpack_8bit_v1(buf, len);
            g_1 = 1;
            if(gui_item_view != nullptr) gui_item_view->arrived_screen(screen1.get());
            g_1 = 0;
            if( gui_item_view->screen_buf.get()->lock_owner != 0) {
                printf("WTF? 1\n");
            }
            /*char ss[110];
            ss[0] = 0;
            sprintf_s( ss, 100, "sz=%d", *sz);
            sudp(ss);*/
            //if (parent_func__arrived_screen != NULL) parent_func__arrived_screen(buf, *sz, *type);

            //if (parent_low_level != nullptr) parent_low_level->invalidate();

            return;
    }
    
}