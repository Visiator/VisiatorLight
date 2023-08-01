/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */

#include <thread>
#include <unistd.h>
#include <sys/types.h>

#ifdef __linux__
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <pwd.h>
#endif

#include <sys/stat.h>


#include "tools.h"

#include "CRYPTO/Crypto.h"
#include "GUI/GUIscreenBuffer.h"



bool detect_full_command(VECTOR_EX& buf);

void sleep_() {
    std::this_thread::sleep_for (std::chrono::milliseconds(1));
}
void sleep_(int val) {
    std::this_thread::sleep_for (std::chrono::milliseconds(val));
}

void wtf(std::string str) {
    
}

void fatal_error(std::string str) {
    
}

uint32_t transparent_color(uint32_t old_color, uint32_t new_color, int percent) {
	int need_r, need_g, need_b;

	int old_r, old_g, old_b, new_r, new_g, new_b;

	need_r = new_color & 0x0000ff;
	need_g = (new_color & 0x00ff00) >> 8;
	need_b = (new_color & 0xff0000) >> 16;

	old_r = old_color & 0xff;
	old_g = ((old_color & 0xff00) >> 8);
	old_b = ((old_color & 0xff0000) >> 16);



	new_r = (int)(old_r + (float)(need_r - old_r) / (float)255 * (float)percent);
	new_g = (int)(old_g + (float)(need_g - old_g) / (float)255 * (float)percent);
	new_b = (int)(old_b + (float)(need_b - old_b) / (float)255 * (float)percent);

	return new_r | (new_g << 8) | (new_b << 16);

}

int wchar_to_ascii(int p) {

	if (p == 0x401) return '?';
	if (p == 0x451) return '?';
	if (p >= 0x410 && p <= 0x44f) {
		return (0xC0 + (p - 0x410));
	};
	if (p >= 0x20 && p <= 0x7f)
	{
		return p;
	};
	if (p == 8226) return 150;
	return '?';
}

uint32_t convert_key_from_w32(uint32_t wp) {
    if(wp == 113) return 113;
    printf("convert_key_from_w32? %d\n", wp);
    return wp;
}

uint32_t convert_key_from_linux(uint32_t wp) {
    if(wp == 113) return 37; // left
    if(wp == 114) return 39; // right
    printf("convert_key_from_linux? %d\n", wp);
    return 0;
}

int setnonblocking(int sock) {
    /* W32
ULONG          ulB;
ulB = TRUE; // Set socket to non-blocking mode^M
ioctlsocket(sockfd, FIONBIO, &ulB);
//   fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK);^M
int flag = 1;
int result = setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int));
*/
    
/*int arg = ::fcntl(sock, F_GETFL);
long flags = arg & ~O_NONBLOCK;
if (!flag) flags |= O_NONBLOCK;
::fcntl(sock, F_SETFL, flags )*/
    
#ifdef __linux__
    unsigned long int on = 1;
    const int result = ioctl(sock, FIONBIO, &on);
    if (result < 0) {
        //fprintf(stderr, "error: ioctlsocket()=%d\n", WSAGetLastError());
        //return false;
    }
#endif
    
return 0;
}

void closesocket(int sock) {
            
    struct linger lng = { 1, 0 };
    setsockopt(sock, SOL_SOCKET, SO_LINGER, (char *)(&lng), sizeof(struct linger));

    #ifdef _WIN32
    int ee = shutdown(sock, SD_BOTH);
    #endif
    #ifdef __linux__
    int ee = shutdown(sock, SHUT_RDWR);// SD_BOTH);
    #endif
    close(sock);
        
}

int my_recv(int sock, uint8_t *buf, int size) {
	//AnsiString status;
    
	int res, er;
	res = recv(sock, (char *)buf, size, 0);
        if(res > 0) {
            return res;
        }
        if(res == 0) {
            printf("???\n");
            return -1;
        }
	if (res == -1)
	{
            if(errno == EAGAIN) {
                //printf("EAGAIN\n");
                return 0;
            } else {
                printf("recv error [%d]\n", errno);
            }
	};
        return -1;
}

int my_send(int sock, uint8_t *buf, int size) {
	//AnsiString status;
    
	int res, er;
	res = send(sock, (char *)buf, size, 0);
        if(res > 0) {
            return res;
        }
        if(res == 0) {
            printf("???\n");
            return 0;
        }
	if (res == -1)
	{
            if(errno == EAGAIN) {
                return 0;
            } else {
                printf("recv error [%d]\n", errno);
            }
	};
        return -1;
}

bool my_random(uint8_t *buf, int buf_len) {
    FILE *f;
    f = fopen("/dev/random", "rb");
    if(f == NULL) {
        return false;
    }
    int r  = fread(buf, 1, buf_len, f);
    fclose(f);
    if(r == buf_len) return true;
    return false;
}

uint32_t get_sol() {
    uint32_t a;
    my_random((uint8_t *)&a, 4);
    return a;
};

void zero_(uint8_t *s, int len) {
    for (int i = 0; i < len; i++) s[i] = 0;
}

void zero_(std::vector<uint8_t>& buf) {
    for(int i=0; i < buf.size(); i++) {
        buf[i] = 0;
    }
}

uint32_t get_ip_view_visiator_com() {

	uint32_t ip;
	//PROXY_LIST *proxy_list = app_attributes.proxy_list;

	hostent *host;
	host = gethostbyname(view_visiator_com);
	if (host == NULL) {
		ip = 0;
	}
	else {
		ip = ((in_addr*)host->h_addr_list[0])->s_addr;
	};
	//ip = 0;

	//if (ip == 0) {
	//	if (proxy_list != NULL) {
	//		ip = proxy_list->get_ip_view_visiator_com();
	//	}
	//}


	return ip;
}

bool Save_private_id_and_public_id(uint64_t private_id_, uint64_t public_id_) {

#ifdef _WIN32
    return true;
#endif
    
#ifdef __linux__
    const char *homedir = getpwuid(getuid())->pw_dir;
    
    std::string dir = homedir;
    dir += "/.visiator";
    
    struct stat st;
    int status = 0;
    if (stat(dir.c_str(), &st) != 0) mkdir(dir.c_str(), 0x41C0);
    
    std::string filename = "/home/smorodin/.visiator/token";
    
    
    {
        using unique_file_t = std::unique_ptr<FILE, decltype(&std::fclose)>;
        unique_file_t fi(std::fopen(filename.c_str(), "wb"), &std::fclose);
        if(fi.get() == NULL) {
            return false;
        }
        std::fwrite((void *)&private_id_, 1, 8, fi.get());
        std::fwrite((void *)&public_id_, 1, 8, fi.get());
    }
    return true;
#endif
    
}

bool Load_private_id_and_public_id(uint64_t& private_id_, uint64_t& public_id_) {

#ifdef _WIN32
    return true;
#endif
    
#ifdef __linux__
    const char *homedir = getpwuid(getuid())->pw_dir;
    
    std::string dir = homedir;
    dir += "/.visiator";
    
    struct stat st;
    int status = 0;
    if (stat(dir.c_str(), &st) != 0) mkdir(dir.c_str(), 0x41C0);
    
    std::string filename = "/home/smorodin/.visiator/token";
    
    
    {
        using unique_file_t = std::unique_ptr<FILE, decltype(&std::fclose)>;
        unique_file_t fi(std::fopen(filename.c_str(), "r"), &std::fclose);
        if(fi.get() == NULL) {
            return false;
        }
        std::fread((void *)&private_id_, 1, 8, fi.get());
        std::fread((void *)&public_id_, 1, 8, fi.get());
    }
    return true;
#endif
    
}


int  my_strlen(unsigned char *v) {
	if (v == NULL || v[0] == 0) return 0;
	int i;
	i = 0;
	while (v[i] != 0) i++;
	return i;
};
int  my_strlen(wchar_t *v) {
	if (v == NULL || v[0] == 0) return 0;
	int i;
	i = 0;
	while (v[i] != 0) i++;
	return i;
};
uint64_t generate_ID(unsigned char *id) {
    if (id == nullptr) return 0;

    uint16_t p1, p2, p3;
    unsigned long long ll;
    uint16_t *s;
    s = (uint16_t *)&ll;
    ll = 0;

    if (my_strlen(id) == 11) {

            if (id[0] < '0' || id[0] > '9') return 0;
            if (id[1] < '0' || id[1] > '9') return 0;
            if (id[2] < '0' || id[2] > '9') return 0;

            if (id[3] >= '0' && id[3] <= '9') return 0;

            if (id[4] < '0' || id[4] > '9') return 0;
            if (id[5] < '0' || id[5] > '9') return 0;
            if (id[6] < '0' || id[6] > '9') return 0;

            if (id[7] >= '0' && id[7] <= '9') return 0;

            if (id[8] < '0' || id[8] > '9') return 0;
            if (id[9] < '0' || id[9] > '9') return 0;
            if (id[10] < '0' || id[10] > '9') return 0;

            p1 = (id[0] - '0') * 100 + (id[1] - '0') * 10 + (id[2] - '0');
            p2 = (id[4] - '0') * 100 + (id[5] - '0') * 10 + (id[6] - '0');
            p3 = (id[8] - '0') * 100 + (id[9] - '0') * 10 + (id[10] - '0');

            *s = p1; 	s++;
            *s = p2;	s++;
            *s = p3;	s++;

            return ll;
    }

    if (my_strlen(id) != 9) return 0;




    p1 = (id[0] - '0') * 100 + (id[1] - '0') * 10 + (id[2] - '0');
    p2 = (id[3] - '0') * 100 + (id[4] - '0') * 10 + (id[5] - '0');
    p3 = (id[6] - '0') * 100 + (id[7] - '0') * 10 + (id[8] - '0');

    *s = p1; 	s++;
    *s = p2;	s++;
    *s = p3;	s++;

    return ll;
}
void SCREEN_BUFFER::init_decode_color2() {

	int i;
	unsigned int r, g, b;
	for (i = 1; i < 256; i++) decode_color_matrix_G7C223[i] = 0xff;

	decode_color_matrix_G7C223[0x80] = 0x000000;
	decode_color_matrix_G7C223[0x81] = 0x242424;
	decode_color_matrix_G7C223[0x82] = 0x494949;
	decode_color_matrix_G7C223[0x83] = 0x6D6D6D;
	decode_color_matrix_G7C223[0x84] = 0x929292;
	decode_color_matrix_G7C223[0x85] = 0xC6C6C6;
	decode_color_matrix_G7C223[0x86] = 0xEBEBEB;
	decode_color_matrix_G7C223[0x87] = 0xffffff;

	for (i = 1; i <= 127; i++) {
		r = i & 0x07; 
		if (r == 0) r = 0x000000;
		if (r == 1) r = 0x240000;
		if (r == 2) r = 0x490000;
		if (r == 3) r = 0x6D0000;
		if (r == 4) r = 0x920000;
		if (r == 5) r = 0xB60000;
		if (r == 6) r = 0xDB0000;
		if (r == 7) r = 0xFF0000;

		g = i & 0x18; g = g >> 3;
		if (g == 0) g = 0x000000;
		if (g == 1) g = 0x005500;
		if (g == 2) g = 0x00AA00;
		if (g == 3) g = 0x00ff00;

		b = i & 0x60; b = b >> 5;
		if (b == 0) b = 0x000000;
		if (b == 1) b = 0x000055;
		if (b == 2) b = 0x0000AA;
		if (b == 3) b = 0x0000ff;
		decode_color_matrix_G7C223[i] = r | g | b;
	};
	decode_color_matrix_G7C223[0xfe] = 0xfe;
        decode_color_matrix_G7C223[0] = 0;
};
bool SCREEN_BUFFER::unpack_8bit_v1(uint8_t* buf, uint32_t len) {
    ENCODED_SCREEN_8bit_header* header;
    header = (ENCODED_SCREEN_8bit_header *)&(buf[0]);
    if(header == nullptr) return false;
    if(header->w == 0 || header->w > 10000 ||
       header->h == 0 || header->h > 10000) return false;
    set_size(header->w, header->h);
    
    if (header->format != 2) return false;
    if (header->color_bit != 32) return false;
    

    
    keyboard_location = header->keyboard_location;
    
    int header_size = 16 * 4;
    if(header_size >= len) return false;
    C8SH16 *pal = (struct C8SH16 *)&(buf[header_size]);

    if(header_size + header->pal_size > len) return false;
    uint8_t* bdy = &(buf[header_size + header->pal_size]);
    
    ////////////////////////////////////////////////////////////////////////////
    
    uint32_t* q = buffer;
    
    uint32_t bdy_size = header->body_size;
    uint32_t pl_size = header->pal_size;
    
    if(header->reserv01 != len) {
        
        return false;
    }
    
    uint32_t pl_idx, i = 0;
    uint32_t jj, j = 0;
    while (i < bdy_size) {
            if (*bdy < 128) {
                    pl_idx = *bdy;
                    bdy++;
            }
            else {
                    pl_idx = (*bdy) & 0x7f;
                    bdy++;
                    i++;
                    pl_idx = pl_idx << 8;
                    pl_idx += *bdy;
                    bdy++;
            }
            if (pl_idx < 0 || pl_idx >= pl_size) {
                    //if(raw_screen->is_lock_FULL) raw_screen->unlock_FULL(3003);
                    //fatal_error("SCREEN::decode()  pl_idx < 0 || pl_idx >= pl_size ");
                    return false;
            }

            jj = pal[pl_idx].s;

            j += jj;

            uint32_t clr = decode_color_matrix_G7C223[pal[pl_idx].c];// decode_clr(pl[pl_idx].c);
            
            //clr = invert(clr);
            if(clr == 0xff8000) {
                clr = 0xff8000;
            }
            if (clr == 0xfe) {
                    q += jj;

            }
            else {
                    for (int ii = 0; ii < jj; ii++) {

                            *q = clr;

                            q++;
                    }
            };
            i++;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    
    return true;
}

