/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   GUIwindows.cpp
 * Author: smorodin
 * 
 * Created on May 7, 2023, 1:51 PM
 */

#include <string>

#include "GUIwindowList.h"
#include "../tools.h"
#include "GUI.h"

GUIwindowList::GUIwindowList(GUI *gui) : gui(gui) {
    
}

GUIwindow* GUIwindowList::CreateWindow_(std::string name, GUIwindow::window_mode mode, RECTANGLE rectangle, GUIitems* gui_items, void* parent) {
    
    std::lock_guard<std::mutex> lock(g_mutex);
    
    for(const auto& w : window_list) {
        if(w.window->name == name) {
            return nullptr;
        }
    }
    
    window_list.push_back({11, gui, name, mode, rectangle, gui_items, parent});//, name, mode, rectangle, gui_items, parent}); 
    
    for(unsigned int i = 0; i < window_list.size(); i++) {
        printf("[%d] WL screen = %s\n", i, window_list[i].window->screen == NULL ? "NULL" : "OK");
        
        if(window_list[i].window->name == name) {
            window_list[i].window->run();
#ifdef __linux__
            /*
            XLockDisplay(linux_display);
            XSelectInput(linux_display, window_list[i].window->window, ExposureMask | StructureNotifyMask | KeyPressMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask );
            //Показываем окно на экране
            XMapWindow(linux_display, window_list[i].window->window);
            window_list[i].window->wm_delete = XInternAtom(linux_display, "WM_DELETE_WINDOW", True);
            XSetWMProtocols(linux_display, window_list[i].window->window, &(window_list[i].window->wm_delete), 1);
            XUnlockDisplay(linux_display); 
            */
#endif // __linux__    
            
            return window_list[i].window.get();
        }
    }
    
    return nullptr;
}


/*
int xxx = 0;

void GUIwindowList::execute() { 
    
#ifdef __linux__
    XEvent      event;
    
    while(need__stop == false) {
        int ff = 0;
        try
        {
            XLockDisplay(linux_display);
            if (XPending(linux_display)) {
                ff = 1;
            }
            XUnlockDisplay(linux_display);
        }
        catch(...)
        {
            printf("wtf?");
        };
        
        if(ff == 1)
        {
            try 
            {
                XLockDisplay(linux_display);
                XNextEvent(linux_display, &event);
                XUnlockDisplay(linux_display);
            } catch(...) {
                printf("catch 1\n");
            }
            
            GUIwindow *w = nullptr;
            
            {
                {
                    std::lock_guard<std::mutex> lock(g_mutex);
                    int f = 0;
                    for(unsigned int i = 0; i < window_list.size(); i++) {
                        if(event.xkey.window == window_list[i].window->window) {
                            if( event.type == ClientMessage &&
                                event.xclient.data.l[0] == window_list[i].window->wm_delete) 
                            {
                                if(window_list[i].window->window_close()) {
                                    XLockDisplay(gui->linux.display_);
                                    
                                    XUnmapWindow(gui->linux.display_, window_list[i].window->window);
                                    //XDestroyWindow(gui->linux.display_, window_list[i].window->window);
                                    XUnlockDisplay(gui->linux.display_);
                                }
                            } else {
                                window_list[i].window->event(event);
                            }
                            f = 1;
                            break;
                        }
                    }
                    if(f == 0) {
                        printf("sss\n");
                    }
                }
            }
            
        }
        else
        {
            if(need__stop == false) {
                XEvent      exppp;
                std::lock_guard<std::mutex> lock(g_mutex);
                
                for(unsigned int i = 0; i < window_list.size(); i++) {
                
                    memset(&exppp, 0, sizeof(exppp));
                    exppp.type = Expose;
                    exppp.xexpose.window = window_list[i].window->window;
                    XSendEvent(gui->linux.display_, window_list[i].window->window, False, ExposureMask, &exppp);
                }
            }
            usleep(10);
        }
        sleep_();
    }
    execute_is_run = false;
#endif // __linux__    
    
};
*/
/*  
#ifdef __linux__    
    GC prGC;
    XEvent      exppp;
    XEvent      event;
    
    XSelectInput(gui->linux.display_, window, ExposureMask | StructureNotifyMask | KeyPressMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask );
     
    //Показываем окно на экране
    XMapWindow(gui->linux.display_, window);
    
    if(xxx > 0) return;
    xxx++;
    
    while(need__stop == false) {
        
            if (XPending(gui->linux.display_)) 
            {
                try {
                    XNextEvent(gui->linux.display_, &event);
                } catch(...) {
                    printf("catch 1\n");
                }
                
                if (event.type == Expose) // Перерисовываем окно
                {
                    
                    //XDrawString (linux.display, linux.window, linux.graph_ctx, 50, 50, msg, strlen (msg));
                    
                    
                } else {
                    
                }

                
                switch (event.type) 
                {
                    case Expose:
                        event_paint();
                        
                        XPutImage(gui->linux.display_, window, gui->linux.graph_ctx, image, 0, 0, 0, 0, screen->w, screen->h);//DISPLAY_WIDTH, DISPLAY_HEIGHT);
                        XFreeGC ( gui->linux.display_, prGC );
                        
                        
                        
                    
                        //XSetForeground (gui->linux.display_, gui->linux.graph_ctx, 0x337700);
                        //printf("Expose\n");
                        break;
                    case ConfigureNotify:
                        if(event.xconfigure.width != screen->w || 
                           event.xconfigure.height != screen->h) {
                            
                            screen->set_size( event.xconfigure.width, event.xconfigure.height);
                            image->data = (char *)screen->buffer;
                            image->bytes_per_line = screen->w*4;
                            image->width = screen->w;
                            image->height = screen->h;
                            //screen->fill_all(0x117777);
                            
                            event_resize(screen->w, screen->h);
                            
                            printf("1 %d : %d - %d : %d\n", 
                                    event.xconfigure.x,
                                    event.xconfigure.y,
                                    event.xconfigure.width,
                                    event.xconfigure.height );
                            memset(&exppp, 0, sizeof(exppp));
                            exppp.type = Expose;
                            exppp.xexpose.window = window;
                            XSendEvent(gui->linux.display_, window, False, ExposureMask, &exppp);
                        }
                        break;
                    case ReparentNotify:
                        printf("12\n");
                        break;
                    case MapNotify:
                        printf("13\n");
                        break;
                    case MotionNotify:
                        //printf("14\n");
                        break;
                    case ClientMessage:
                        printf("15\n");
                        break;
                    case DestroyNotify:
                        printf("16\n");
                        break;
                    case ButtonPress:
                        printf("17\n");
                        window_user_event.type = WindowUserEvent::UserEventType::uev_MousePress;
                        window_user_event.x = event.xbutton.x;
                        window_user_event.y = event.xbutton.y;
                        StaticUserInput(parent, &window_user_event);
                        break;
                    case KeyPress:
                        printf("18\n");
                        window_user_event.type = WindowUserEvent::UserEventType::uev_KeyPress;
                        window_user_event.keycode = event.xkey.keycode;
                        StaticUserInput(parent, &window_user_event);
                        break;
                    case ButtonRelease:
                        //printf("19\n");
                        window_user_event.type = WindowUserEvent::UserEventType::uev_MouseUnpress;
                        window_user_event.x = event.xbutton.x;
                        window_user_event.y = event.xbutton.y;
                        StaticUserInput(parent, &window_user_event);
                        break;
                    default: 
                        printf("other\n");
                }
                
            }
            else
            {
                if(1==1 && need__stop == false) {
                    //screen.need_update = false;
                    memset(&exppp, 0, sizeof(exppp));
                    exppp.type = Expose;
                    exppp.xexpose.window = window;
                    XSendEvent(gui->linux.display_, window, False, ExposureMask, &exppp);
                }
                usleep(10);
            }
        
        
        sleep_();
    }
    execute_is_run = false;
#endif

#ifdef _WIN32
    int err;
    HICON hi = nullptr;
	HICON hi_sm = nullptr;
	//hi_sm = LoadIcon(app_attributes.hInstance, MAKEINTRESOURCE(101));

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = GUIwindow::WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = gui->hInstance;
	wc.hIcon = hi_sm; //  LoadIcon(app_attributes.hInstance, MAKEINTRESOURCE(1055));
	wc.hIconSm = nullptr; // LoadIcon(app_attributes.hInstance, MAKEINTRESOURCE(101));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(6);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"ClientManager";

	if (!RegisterClassEx(&wc)) {
		fatal_error("RegisterClass error");
		return;
	};
        
    int x, y, w, h;
    x = rectangle.getx(); y = rectangle.gety(); w = rectangle.getw(); h = rectangle.geth();
    printf("CreateWindow %d %d\n", w, h);
    gui->win32_param.p1 = 1001;
    screen->set_size(rectangle.getw(), rectangle.geth());
    viewer_window_hwnd = CreateWindow( L"ClientManager",  L"ClientManager", WS_POPUP, 100, 100, w, h, NULL, NULL, gui->hInstance, this);
	//app_attributes.desktop_window_hwnd = CreateWindowEx( 0, L"VISIATOR VIEWER", L"VISIATOR", WS_OVERLAPPEDWINDOW, x, y, w, h, NULL, NULL, app_attributes.hInstance, NULL);

	if (!viewer_window_hwnd) {
        err = GetLastError();
		fatal_error("CreateWindow error");
		return;
	};
    printf("11111 %x %s\n", viewer_window_hwnd, this->name.c_str());
        
    ShowWindow(viewer_window_hwnd, SW_SHOWNORMAL);
    UpdateWindow(viewer_window_hwnd);
        
    MSG msg;
    printf("w1\n");

    //timer();
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
    
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    execute_is_run = false;
#endif

}
*/

void GUIwindowList::stop() {
    
}

void GUIwindowList::wait() {
    while(1) {
        bool f = false;
        {
            std::lock_guard<std::mutex> lock(g_mutex);
            int s = window_list.size();
            if(s > 0) f = true;
            for(unsigned int i = 0; i < window_list.size(); i++) {
                if(window_list[i].window->execute_is_run == false) {
                    window_list.erase(window_list.begin() + i);
                    
                    break;
                }
            }
        }
        if(f == false) {
            printf("windowlist finish\n");
            break;
        }
        sleep_(100);
    }
}