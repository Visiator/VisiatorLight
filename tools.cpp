/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */

#include <thread>

#include "tools.h"

void sleep_() {
    std::this_thread::sleep_for (std::chrono::milliseconds(1));
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
