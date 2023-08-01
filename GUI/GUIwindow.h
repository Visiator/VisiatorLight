/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   GUIwindow.h
 * Author: smorodin
 *
 * Created on May 7, 2023, 1:54 PM
 */

#ifndef GUIWINDOW_H
#define GUIWINDOW_H

#include <thread>
#include <string>

#ifdef __linux__
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
//#include <X11/extensions/XTest.h>
#endif

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

#include "GUIscreenBuffer.h"
#include "GUIprimitives.h"
#include "GUIitem.h"

class GUI;

class WindowUserEvent {
public:
    enum UserEventType {
          uev_notset = 0
        , uev_MousePress = 1
        , uev_KeyPress = 2
        , uev_MouseUnpress = 3
    };
    int x, y;
    UserEventType type;
    uint32_t keycode;
};

class GUIwindow {
public:
    
    GUI *gui;
#ifdef __linux__
    Display     *linux_display = nullptr;
    int         linux_screen_id = 0;
    GC gc;
    Window window;
    XImage *image;
    Atom wm_delete;
#endif

#ifdef _WIN32
    HWND viewer_window_hwnd;
    HWND m_hwnd;
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    std::unique_ptr<BITMAPINFO> bmInfo;
    
    LRESULT WM_PAINT_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_SIZING_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_SIZE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_LBUTTONDOWN_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_LBUTTONUP_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_RBUTTONDOWN_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_RBUTTONUP_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_CONTEXTMENU_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_MOUSEMOVE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_SETCURSOR_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_NCHITTEST_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_GETMINMAXINFO_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_CANCELMODE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_QUEUESYNC_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_CHILDACTIVATE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_MOUSEACTIVATE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_NCCREATE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_NCCALCSIZE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_SHOWWINDOW_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_WINDOWPOSCHANGING_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);

    LRESULT WM_GETICON_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_QUERYNEWPALETTE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_ACTIVATEAPP_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_STYLECHANGING_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);

    LRESULT WM_ACTIVATE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_SETFOCUS_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_KILLFOCUS_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_NCPAINT_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);

    LRESULT WM_ERASEBKGND_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_WINDOWPOSCHANGED_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_NCACTIVATE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_MOVE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_NCDESTROY_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    LRESULT WM_KEYDOWN_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    
#endif
    enum window_mode {
        NOTSET = 0,
        FIXED = 1,
        RESIZED = 2,
    };
    
    std::string name;
    GUIwindow::window_mode mode;
    RECTANGLE rectangle;
    
    int scr_cr = 0;
    
    std::unique_ptr<SCREEN_BUFFER> screen;
    GUIitems* gui_items = nullptr;
    
    WindowUserEvent window_user_event;
    void (*StaticUserInput) (void* parent, WindowUserEvent* window_user_event);
    void* parent = nullptr;
    
    void copy_from_( GUIwindow& src) {
        name       = src.name;
        mode       = src.mode;
        gui        = src.gui; 
        rectangle  = src.rectangle;
        need_stop  = src.need_stop;
        screen     = std::move(src.screen);
        gui_items  = src.gui_items;
        parent     = src.parent;
        StaticUserInput = src.StaticUserInput;
        

#ifdef __linux__
        linux_display   = src.linux_display;
        linux_screen_id = src.linux_screen_id;
        window          = src.window;
        image           = src.image;
        wm_delete       = src.wm_delete;
        gc              = src.gc;
#endif
#ifdef _WIN32
        bmInfo     = std::move(src.bmInfo);
        m_hwnd     = src.m_hwnd;
        viewer_window_hwnd = src.viewer_window_hwnd;
#endif
    }
    
    void clear_(GUIwindow& src) {
        src.name = "";
        src.mode = window_mode::NOTSET;
        src.gui = nullptr;
        src.need_stop  = false;
        src.screen = nullptr;
        src.gui_items = nullptr;
        src.parent = nullptr;
        src.StaticUserInput = nullptr;

#ifdef __linux__
        src.window          = 0;
        src.image           = nullptr;
        src.wm_delete       = 0;
        src.linux_display   = nullptr;
        src.linux_screen_id = 0;
        src.gc              = 0;
#endif
#ifdef _WIN32
        src.bmInfo = nullptr;
        src.m_hwnd = 0;
        src.viewer_window_hwnd = 0;
#endif        
    }
    void need_refresh();
    void test();
    void run();
    //void exit();
    
    GUIwindow(GUI *gui, std::string name, GUIwindow::window_mode mode, RECTANGLE rectangle, GUIitems* gui_items, void* parent);
    GUIwindow( GUIwindow& orig) = delete;
    
    GUIwindow( GUIwindow&& src) noexcept {
        *this = std::move(src);
        printf("GUIwindow constructor move\n");
    }
    
    GUIwindow& operator=( GUIwindow& src) = delete;
    GUIwindow& operator=( GUIwindow&& src) noexcept {
        if(&src == this) return *this;
        copy_from_(src);
        clear_(src);
        return *this;
    }
    

    bool need_stop;
    void set_need_stop() {
        printf("set need stop\n");
        need_stop = true;
    }
    bool execute_is_run = false;
    std::thread* execute_thread = nullptr;
    void execute();
    
    bool window_close();
    void event_paint();
    
    void set_size(int w, int h);
    
    void event_resize(uint32_t width, uint32_t height);
    
    virtual ~GUIwindow() {
#ifdef __linux__
        XFreeGC (linux_display, gc);
        XCloseDisplay(linux_display);
        linux_display = nullptr;
#endif
        printf("GUIwindow destructor\n");
    }
private:

};

#endif /* GUIWINDOW_H */

