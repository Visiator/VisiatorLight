/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   GUIwindow.cpp
 * Author: smorodin
 * 
 * Created on May 7, 2023, 1:54 PM
 */
//#define _WIN32

#include <thread>

#include <stdio.h>

#include "../tools.h"
#include "GUIwindow.h"
#include "GUI.h"

static const char *icon1[]={
"16 16 30 1",
". c #172b3d",
"k c #25353e",
"d c #333f3e",
"l c #343f3e",
"r c #50523f",
"y c #50533f",
"# c #51533f",
"c c #5f5d40",
"n c #6d6640",
"q c #6d6740",
"t c #7b7040",
"a c #7c7040",
"j c #7c7141",
"z c #897a41",
"b c #8a7a41",
"h c #8a7b42",
"m c #978442",
"u c #988442",
"s c #988443",
"g c #a68e42",
"x c #b59843",
"p c #c3a244",
"e c #c4a244",
"A c #d1ac45",
"o c #d2ac45",
"i c #e0b645",
"w c #e1b645",
"v c #eec045",
"B c #efc045",
"f c #fdca46",
".....#abbcd.....",
"...#effffffgd...",
"..hfffffffffid..",
".jfffffffffffik.",
"lifmnnbfinnnofg.",
"bffp...qr..lfffk",
"offfd......sffft",
"ffffb......ifffu",
"ffffvk....qffffg",
"wffffb....iffffb",
"xffffvk..qfffffr",
"yffffft..iffffi.",
".pffffi.#fffffn.",
".liffffyxffffz..",
"..lpfffAfffft...",
"....qxBffiud...."};

#ifdef __linux__

static void SetWindowManagerHints (
Display * prDisplay, /*Указатель на структуру Display */
char * psPrgClass, /*Класс программы */
char * argv[], /*Аргументы программы */
int argc, /*Число аргументов */
Window nWnd, /*Идентификатор окна */
int x, /*Координаты левого верхнего*/
int y, /*угла окна */
int nWidth,
int nHeight, /*Ширина и высота окна */
int nMinWidth,
int nMinHeight, /*Минимальные ширина и высота окна */
char * psTitle, /*Заголовок окна */
char * psIconTitle,/*Заголовок пиктограммы окна */
Pixmap nIconPixmap /*Рисунок пиктограммы */
)
{
    XSizeHints rSizeHints; /*Рекомендации о размерах окна*/
    XWMHints rWMHints;
    XClassHint rClassHint;

    XTextProperty prWindowName, prIconName;
    if ( !XStringListToTextProperty (&psTitle, 1, &prWindowName ) ||
         !XStringListToTextProperty (&psIconTitle, 1, &prIconName ) ) {
        puts ( "No memory!\n");
        exit ( 1 );
    }
    rSizeHints.flags = PPosition | PSize | PMinSize | PMaxSize;
    rSizeHints.min_width = nMinWidth;
    rSizeHints.min_height = nMinHeight;

    rSizeHints.max_width = nMinWidth;
    rSizeHints.max_height = nMinHeight;
    
    rWMHints.flags = StateHint | IconPixmapHint | InputHint;
    rWMHints.initial_state = NormalState;
    rWMHints.input = True;
    rWMHints.icon_pixmap= nIconPixmap;
    rClassHint.res_name = argv[0];
    rClassHint.res_class = psPrgClass;
    XSetWMProperties ( prDisplay, nWnd,
                       &prWindowName,
                       &prIconName, argv, argc, &rSizeHints,
                       &rWMHints,
                       &rClassHint );
}
#endif

GUIwindow::GUIwindow(GUI *gui, std::string name, GUIwindow::window_mode mode, RECTANGLE rectangle, GUIitems* gui_items, void* parent) : gui(gui), name(name), mode(mode), rectangle(rectangle), gui_items(gui_items), parent(parent)
, need__stop(false)
, screen(std::make_unique<SCREEN_BUFFER>())
#ifdef __linux__
, window(0)
, image(nullptr)
#endif
#ifdef _WIN32
, viewer_window_hwnd(0)
, m_hwnd(0)
, bmInfo(std::make_unique<BITMAPINFO>())
#endif

{
    printf("GUIwindow constructor 1-1 [%s][%d]\n", name.c_str(), mode);

    screen->set_size(rectangle.getw(), rectangle.geth());
    
#ifdef __linux__    
    window = XCreateSimpleWindow(gui->linux.display_
                                 , RootWindow(gui->linux.display_, gui->linux.screen_id)
                                 , rectangle.getx(), rectangle.gety(), rectangle.getw(), rectangle.geth(), 10
                                 , XBlackPixel(gui->linux.display_, gui->linux.screen_id)
                                 , XWhitePixel(gui->linux.display_, gui->linux.screen_id)
                                );
    
    /* эта переменная будет содержать дескриптор новой пиксельной карты */
    Pixmap pixmap;

    /* эта переменная будет содержать дескриптор корневого окна экрана, */
    /* для которого создадим пиксельную карту          */
    Window window = DefaultRootWindow(gui->linux.display_);

    /* эта переменная будет содержать глубину цвета создаваемой */
    /* пиксельной карты - количество бит, используемых для   */
    /* представления индекса цвета в палитре (количество цветов */
    /* равно степени двойки глубины)              */
    int depth = DefaultDepth(gui->linux.display_, DefaultScreen(gui->linux.display_));

    /* создаем новую пиксельную карту шириной 30 и высотой в 40 пикселей */
    pixmap = XCreatePixmap(gui->linux.display_, window, 16, 16, depth);

    /* для полноты ощущений нарисуем точку в центре пиксельной карты */
    //XDrawPoint(gui->linux.display_, pixmap, gc, 15, 20);    

    
    char *argv_[] = {(char *)"Visiator", (char *)"", NULL};
    /* Задаем рекомендации для менеджера окон */
    SetWindowManagerHints ( gui->linux.display_, (char *)"Visiator",
    argv_, 1,
    window, rectangle.getx(), rectangle.gety(), rectangle.getw(), rectangle.geth(),
    rectangle.getw(), rectangle.geth(),
    (char *)"WND_TITLE", (char *)"WND_ICON_TITLE", pixmap
    );
    
    gui->linux.graph_ctx=XCreateGC(gui->linux.display_, window, 0, gui->linux.gc_values);
    
    image = XCreateImage(
            gui->linux.display_, 
            DefaultVisual(gui->linux.display_, DefaultScreen(gui->linux.display_)),
            DefaultDepth(gui->linux.display_, DefaultScreen(gui->linux.display_)),
            ZPixmap,
            0,
            (char*) screen->buffer,
            rectangle.getw(), 
            rectangle.geth(),
            32,
            (rectangle.getw())*4 );
#endif

#ifdef _WIN32
    //bmInfo = new BITMAPINFO(); //neew(sizeof(BITMAPINFO), bmInfo, "BITMAPINFO()");
    bmInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmInfo->bmiHeader.biBitCount = 32;
    bmInfo->bmiHeader.biClrUsed = 0;
    bmInfo->bmiHeader.biClrImportant = 0;
    bmInfo->bmiHeader.biPlanes = 1;
    bmInfo->bmiHeader.biCompression = BI_RGB;
    bmInfo->bmiHeader.biSizeImage = 0;
    bmInfo->bmiHeader.biHeight = rectangle.geth() * -1;
    bmInfo->bmiHeader.biWidth = rectangle.getw();
#endif

}

void GUIwindow::run() {
    if(screen == NULL) {
        printf("run() NULL 0\n");
    } else {
        if(screen.get() == NULL) {
            printf("run() NULL get()\n");
        } else {
            printf("run() OK get() \n");
        }
    }
    execute_is_run = true;
    execute_thread = new std::thread(&GUIwindow::execute, this);
}


void GUIwindow::execute() { 

#ifdef __linux__    
    GC prGC;
    XEvent      exppp;
    XEvent      event;
    
    XSelectInput(gui->linux.display_, window, ExposureMask | StructureNotifyMask | KeyPressMask | PointerMotionMask | ButtonPressMask );
     
    //Показываем окно на экране
    XMapWindow(gui->linux.display_, window);
    
    
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
                        /* Запрос на перерисовку */
                        if (event.xexpose.count != 0) break;
                        prGC = XCreateGC(gui->linux.display_, window, 0, nullptr);
                        /*XSetForeground ( prDisplay, prGC, BlackPixel ( prDisplay, 0) );
                        XDrawString ( prDisplay, nWnd, prGC, 10,
                        50,
                        "Hello,
                        world!",
                        strlen
                        world!" ) );*/
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

void GUIwindow::stop() {
    need__stop = true;
    while(execute_is_run == true) {
        sleep_();
    }
}


void GUIwindow::test() {
    printf("GUIwindow::test()\n");
}

#ifdef _WIN32
LRESULT CALLBACK GUIwindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    
    //printf("WindowProc\n");

    GUIwindow *pThis = NULL;

    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        pThis = (GUIwindow*)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

        pThis->m_hwnd = hwnd;
        printf("set m_hwnd {%s}\n", pThis->name.c_str());
    }
    else
    {
        pThis = (GUIwindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }
    if (pThis)
    {
        return pThis->HandleMessage(uMsg, wParam, lParam);
    }
    else
    {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

}

LRESULT GUIwindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {

    //printf("HandleMessage [%d]\n", uMsg);

    switch (uMsg)
    {
        case WM_NCDESTROY:          return WM_NCDESTROY_(m_hwnd, uMsg, wParam, lParam);
        case WM_NCCREATE:           return WM_NCCREATE_(m_hwnd, uMsg, wParam, lParam);
        case WM_NCCALCSIZE:         return WM_NCCALCSIZE_(m_hwnd, uMsg, wParam, lParam);
        case WM_SHOWWINDOW:         return WM_SHOWWINDOW_(m_hwnd, uMsg, wParam, lParam);
        case WM_WINDOWPOSCHANGING:  return WM_WINDOWPOSCHANGING_(m_hwnd, uMsg, wParam, lParam);
        case WM_GETICON:            return WM_GETICON_(m_hwnd, uMsg, wParam, lParam);
        case WM_QUERYNEWPALETTE:    return WM_QUERYNEWPALETTE_(m_hwnd, uMsg, wParam, lParam);
        case WM_ACTIVATEAPP:        return WM_ACTIVATEAPP_(m_hwnd, uMsg, wParam, lParam);
        case WM_STYLECHANGING:      return WM_STYLECHANGING_(m_hwnd, uMsg, wParam, lParam);
        case WM_ACTIVATE:           return WM_ACTIVATE_(m_hwnd, uMsg, wParam, lParam);
        case WM_SETFOCUS:           return WM_SETFOCUS_(m_hwnd, uMsg, wParam, lParam);
        case WM_KILLFOCUS:          return WM_KILLFOCUS_(m_hwnd, uMsg, wParam, lParam);
        case WM_NCPAINT:            return WM_NCPAINT_(m_hwnd, uMsg, wParam, lParam);
        case WM_ERASEBKGND:         return WM_ERASEBKGND_(m_hwnd, uMsg, wParam, lParam);
        case WM_WINDOWPOSCHANGED:   return WM_WINDOWPOSCHANGED_(m_hwnd, uMsg, wParam, lParam);
        case WM_NCACTIVATE:         return WM_NCACTIVATE_(m_hwnd, uMsg, wParam, lParam);
        case WM_MOVE:               return WM_MOVE_(m_hwnd, uMsg, wParam, lParam);
        case WM_CANCELMODE:         return WM_CANCELMODE_(m_hwnd, uMsg, wParam, lParam);
        case WM_QUEUESYNC:          return WM_QUEUESYNC_(m_hwnd, uMsg, wParam, lParam);
        case WM_CHILDACTIVATE:      return WM_CHILDACTIVATE_(m_hwnd, uMsg, wParam, lParam);
        case WM_MOUSEACTIVATE:      return WM_MOUSEACTIVATE_(m_hwnd, uMsg, wParam, lParam);
        case WM_MOUSEMOVE:          return WM_MOUSEMOVE_(m_hwnd, uMsg, wParam, lParam);
        case WM_SETCURSOR:          return WM_SETCURSOR_(m_hwnd, uMsg, wParam, lParam);
        case WM_NCHITTEST:          return WM_NCHITTEST_(m_hwnd, uMsg, wParam, lParam);
        case WM_GETMINMAXINFO:      return WM_GETMINMAXINFO_(m_hwnd, uMsg, wParam, lParam);
        case WM_RBUTTONDOWN:        return WM_RBUTTONDOWN_(m_hwnd, uMsg, wParam, lParam);
        case WM_RBUTTONUP:          return WM_RBUTTONUP_(m_hwnd, uMsg, wParam, lParam);
        case WM_CONTEXTMENU:        return WM_CONTEXTMENU_(m_hwnd, uMsg, wParam, lParam);
        case WM_DESTROY:
            PostQuitMessage(0);
            return TRUE;
        case WM_CLOSE:
            PostMessage(m_hwnd, WM_DESTROY, 0, 0);
            return TRUE;
        case WM_PAINT:				return WM_PAINT_(m_hwnd, uMsg, wParam, lParam);
        case WM_SIZING:				return WM_SIZING_(m_hwnd, uMsg, wParam, lParam);
        case WM_SIZE:				return WM_SIZE_(m_hwnd, uMsg, wParam, lParam);
        case WM_LBUTTONDOWN:                    return WM_LBUTTONDOWN_(m_hwnd, uMsg, wParam, lParam);
        case WM_LBUTTONUP:                      return WM_LBUTTONUP_(m_hwnd, uMsg, wParam, lParam);
        default:
            //printf("HandleMessage +def %d\n", uMsg);
            return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
    return TRUE;
}

LRESULT GUIwindow::WM_NCDESTROY_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("\n");
    return DefWindowProc(hw, msg, wp, lp);
}

LRESULT GUIwindow::WM_NCCREATE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("\n");
    return DefWindowProc(hw, msg, wp, lp);
}
LRESULT GUIwindow::WM_NCCALCSIZE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("\n");
    return DefWindowProc(hw, msg, wp, lp);
}
LRESULT GUIwindow::WM_SHOWWINDOW_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("\n");
    return DefWindowProc(hw, msg, wp, lp);
}
LRESULT GUIwindow::WM_WINDOWPOSCHANGING_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("\n");
    return DefWindowProc(hw, msg, wp, lp);
}

LRESULT GUIwindow::WM_GETICON_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("\n");
    return DefWindowProc(hw, msg, wp, lp);
}
LRESULT GUIwindow::WM_QUERYNEWPALETTE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("\n");
    return DefWindowProc(hw, msg, wp, lp);
}
LRESULT GUIwindow::WM_ACTIVATEAPP_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("\n");
    return DefWindowProc(hw, msg, wp, lp);
}
LRESULT GUIwindow::WM_STYLECHANGING_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("\n");
    return DefWindowProc(hw, msg, wp, lp);
}

LRESULT GUIwindow::WM_ACTIVATE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("\n");
    return DefWindowProc(hw, msg, wp, lp);
}
LRESULT GUIwindow::WM_SETFOCUS_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("\n");
    return DefWindowProc(hw, msg, wp, lp);
}
LRESULT GUIwindow::WM_KILLFOCUS_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("\n");
    return DefWindowProc(hw, msg, wp, lp);
}
LRESULT GUIwindow::WM_NCPAINT_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("\n");
    return DefWindowProc(hw, msg, wp, lp);
}

LRESULT GUIwindow::WM_ERASEBKGND_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("\n");
    return DefWindowProc(hw, msg, wp, lp);
}
LRESULT GUIwindow::WM_WINDOWPOSCHANGED_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("\n");
    return DefWindowProc(hw, msg, wp, lp);
}
LRESULT GUIwindow::WM_NCACTIVATE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("\n");
    return DefWindowProc(hw, msg, wp, lp);
}
LRESULT GUIwindow::WM_MOVE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("\n");
    return DefWindowProc(hw, msg, wp, lp);
}


LRESULT GUIwindow::WM_CANCELMODE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("\n");
    return DefWindowProc(hw, msg, wp, lp);
}
LRESULT GUIwindow::WM_QUEUESYNC_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("\n");
    return DefWindowProc(hw, msg, wp, lp);
}
LRESULT GUIwindow::WM_CHILDACTIVATE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("\n");
    return DefWindowProc(hw, msg, wp, lp);
}
LRESULT GUIwindow::WM_MOUSEACTIVATE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("\n");
    return DefWindowProc(hw, msg, wp, lp);
}


LRESULT GUIwindow::WM_MOUSEMOVE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("WM_MOUSEMOVE_\n");
    return DefWindowProc(hw, msg, wp, lp);
}
LRESULT GUIwindow::WM_SETCURSOR_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("\n");
    return DefWindowProc(hw, msg, wp, lp);
}
LRESULT GUIwindow::WM_NCHITTEST_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    //printf("\n");
    //return DefWindowProc(hw, msg, wp, lp);
    return HTCAPTION;
}
LRESULT GUIwindow::WM_GETMINMAXINFO_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("\n");
    return DefWindowProc(hw, msg, wp, lp);
}


LRESULT GUIwindow::WM_RBUTTONDOWN_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("\n");
    return DefWindowProc(hw, msg, wp, lp);
}
LRESULT GUIwindow::WM_RBUTTONUP_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("\n");
    return DefWindowProc(hw, msg, wp, lp);
}
LRESULT GUIwindow::WM_CONTEXTMENU_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("\n");
    return DefWindowProc(hw, msg, wp, lp);
}


LRESULT GUIwindow::WM_SIZING_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("WM_SIZING_ \n");
    return DefWindowProc(hw, msg, wp, lp);
}

LRESULT GUIwindow::WM_SIZE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    uint32_t width = LOWORD(lp);
    uint32_t height = HIWORD(lp);

    printf("WM_SIZE_ %d %d\n", width, height);
    
    rectangle.w = width;
    rectangle.h = height;
    bmInfo->bmiHeader.biHeight = rectangle.geth() * -1;
    bmInfo->bmiHeader.biWidth = rectangle.getw();
    
    event_resize(width, height);
    
    return 0; // Ok
}

LRESULT GUIwindow::WM_LBUTTONDOWN_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("\n");
    return DefWindowProc(hw, msg, wp, lp);
}
LRESULT GUIwindow::WM_LBUTTONUP_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("\n");
    return DefWindowProc(hw, msg, wp, lp);   
}
LRESULT GUIwindow::WM_PAINT_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    
    event_paint();
    
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(m_hwnd, &ps);
    
    int liness = SetDIBitsToDevice(hdc, 0, 0
    , screen->w
    , screen->h
    , 0, 0, 0
    , screen->h
    , screen->buffer 
    , bmInfo.get()
    , DIB_RGB_COLORS); 

    EndPaint(m_hwnd, &ps);
    return 0; // ok  
}
#endif // w32

void GUIwindow::event_resize(uint32_t width, uint32_t height) {
    screen->set_size(width, height);
    
}

void GUIwindow::event_paint() {
    screen->fill_all(0x773377);
    screen->line_h(10, 10, 50, 0xff00aa);
    screen->line_h(screen->w-50, screen->h-1, 40, 0xff00aa);
    screen->print(gui->fonts.roboto220, 50, 50, L"123При");
    screen->paint_items(gui_items);
}