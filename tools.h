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

void sleep_();
void wtf(std::string str);
void fatal_error(std::string str);
uint32_t transparent_color(uint32_t old_color, uint32_t new_color, int percent);
int wchar_to_ascii(int p);

#endif /* TOOLS_H */

