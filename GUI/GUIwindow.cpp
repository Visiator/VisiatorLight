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
    rSizeHints.flags = PPosition | PSize | PMinSize ;//| PMaxSize;
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
, need_stop(false)
, screen(std::make_unique<SCREEN_BUFFER>(&gui->fonts))
, execute_is_run(true)
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
    
    if ((linux_display = XOpenDisplay(getenv("DISPLAY"))) == NULL) {
        wtf("Can't connect X server");//, strerror(errno));
        return;
    }
    linux_screen_id = XDefaultScreen(linux_display);
    
    XLockDisplay(linux_display);
    
    window = XCreateSimpleWindow(linux_display
                                 , RootWindow(linux_display, linux_screen_id)
                                 , rectangle.getx(), rectangle.gety(), rectangle.getw(), rectangle.geth(), 10
                                 , XBlackPixel(linux_display, linux_screen_id)
                                 , XWhitePixel(linux_display, linux_screen_id)
                                );
    
    /* эта переменная будет содержать дескриптор новой пиксельной карты */
    Pixmap pixmap;

    /* эта переменная будет содержать дескриптор корневого окна экрана, */
    /* для которого создадим пиксельную карту          */
    //Window window = DefaultRootWindow(gui->linux.display_);

    /* эта переменная будет содержать глубину цвета создаваемой */
    /* пиксельной карты - количество бит, используемых для   */
    /* представления индекса цвета в палитре (количество цветов */
    /* равно степени двойки глубины)              */
    int depth = DefaultDepth(linux_display, DefaultScreen(linux_display));

    /* создаем новую пиксельную карту шириной 30 и высотой в 40 пикселей */
    pixmap = XCreatePixmap(linux_display, window, 16, 16, depth);

    /* для полноты ощущений нарисуем точку в центре пиксельной карты */
    //XDrawPoint(gui->linux.display_, pixmap, gc, 15, 20);    

    
    char *argv_[] = {(char *)"Visiator", (char *)"", NULL};
    /* Задаем рекомендации для менеджера окон */
    
    SetWindowManagerHints ( linux_display, (char *)"Visiator",
    argv_, 1,
    window, rectangle.getx(), rectangle.gety(), rectangle.getw(), rectangle.geth(),
    rectangle.getw(), rectangle.geth(),
    (char *)"WND_TITLE", (char *)"WND_ICON_TITLE", pixmap
    );
    
    //gui->linux.graph_ctx=XCreateGC(linux_display, window, 0, gui->linux.gc_values);
    
    image = XCreateImage(
            linux_display, 
            DefaultVisual(linux_display, DefaultScreen(linux_display)),
            DefaultDepth(linux_display, DefaultScreen(linux_display)),
            ZPixmap,
            0,
            (char*) screen->buffer,
            rectangle.getw(), 
            rectangle.geth(),
            32,
            (rectangle.getw())*4 );
    
    
    XSelectInput(linux_display, window, ExposureMask | StructureNotifyMask | KeyPressMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask );
    //Показываем окно на экране
    XMapWindow(linux_display, window);
    wm_delete = XInternAtom(linux_display, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(linux_display, window, &wm_delete, 1);
    
    gc = XCreateGC (linux_display, window, 0, 0);
    
    XUnlockDisplay(linux_display);
    
    //gui_items->set_GUIwindow(this);
    
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

    
    execute_thread = new std::thread(&GUIwindow::execute, this);
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
        printf("--------------set m_hwnd {%s}\n", pThis->name.c_str());
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

    printf("HandleMessage [%d]\n", uMsg);

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
        case WM_KEYDOWN:                        return WM_KEYDOWN_(m_hwnd, uMsg, wParam, lParam);
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
    printf("WM_CANCELMODE_\n");
    return DefWindowProc(hw, msg, wp, lp);
}
LRESULT GUIwindow::WM_QUEUESYNC_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("WM_QUEUESYNC_\n");
    return DefWindowProc(hw, msg, wp, lp);
}
LRESULT GUIwindow::WM_CHILDACTIVATE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("WM_CHILDACTIVATE_\n");
    return DefWindowProc(hw, msg, wp, lp);
}
LRESULT GUIwindow::WM_MOUSEACTIVATE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    //printf("WM_MOUSEACTIVATE_\n");
    return DefWindowProc(hw, msg, wp, lp);
}


LRESULT GUIwindow::WM_MOUSEMOVE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    //printf("WM_MOUSEMOVE_\n");
    return DefWindowProc(hw, msg, wp, lp);
}
LRESULT GUIwindow::WM_SETCURSOR_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    //printf("WM_SETCURSOR_\n");
    return DefWindowProc(hw, msg, wp, lp);
}
LRESULT GUIwindow::WM_NCHITTEST_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    //printf("\n");
    //return DefWindowProc(hw, msg, wp, lp);
    
    int mx = (int)LOWORD(lp), my = (int)HIWORD(lp);

    RECT rr;
    if (GetWindowRect(hw, &rr) != TRUE) return 0;
    my -= (int)rr.top;
    mx -= (int)rr.left;

    GUIitem* item = gui_items->get_active_item_by_xy(mx, my);
    if(item != nullptr) {
        printf("AAA");
        return HTCLIENT;
    }
    //printf("nnn %d %d\n", mx, my);
    return HTCAPTION;
}
LRESULT GUIwindow::WM_GETMINMAXINFO_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("WM_GETMINMAXINFO_\n");
    return DefWindowProc(hw, msg, wp, lp);
}


LRESULT GUIwindow::WM_RBUTTONDOWN_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("WM_RBUTTONDOWN_\n");
    return DefWindowProc(hw, msg, wp, lp);
}
LRESULT GUIwindow::WM_RBUTTONUP_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("WM_RBUTTONUP_\n");
    return DefWindowProc(hw, msg, wp, lp);
}
LRESULT GUIwindow::WM_CONTEXTMENU_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("WM_CONTEXTMENU_\n");
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
    printf("GUIwindow::WM_LBUTTONDOWN_\n");
    int mx = (int)LOWORD(lp), my = (int)HIWORD(lp);
    
    window_user_event.type = WindowUserEvent::UserEventType::uev_MousePress;
    window_user_event.x = mx;
    window_user_event.y = my;
    StaticUserInput(parent, &window_user_event);
    return DefWindowProc(hw, msg, wp, lp);
}
LRESULT GUIwindow::WM_LBUTTONUP_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    printf("WM_LBUTTONUP_\n");
    int mx = (int)LOWORD(lp), my = (int)HIWORD(lp);
    window_user_event.type = WindowUserEvent::UserEventType::uev_MouseUnpress;
    window_user_event.x = mx;
    window_user_event.y = my;
    StaticUserInput(parent, &window_user_event);
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

LRESULT GUIwindow::WM_KEYDOWN_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

    printf("18\n");
    
    wchar_t wbuffer[8];
    BYTE lpKeyState[256];
    GetKeyboardState(lpKeyState);
    
    if(ToUnicode(wp, HIWORD(lp) & 0xFF, lpKeyState, wbuffer, 8, 0) == 1) {
        
        window_user_event.type = WindowUserEvent::UserEventType::uev_KeyPress;
        window_user_event.keycode = wbuffer[0];
        StaticUserInput(parent, &window_user_event);    
    } else {
        printf("ToUnicode error\n");
        window_user_event.type = WindowUserEvent::UserEventType::uev_KeyPress;
        window_user_event.keycode = convert_key_from_w32(wp);
        StaticUserInput(parent, &window_user_event);    
    }
    need_refresh();
    
    
    
    //char_keydown(msg, wp, lp, wbuffer);

    return 0;
};

#endif // w32

void GUIwindow::event_resize(uint32_t width, uint32_t height) {
    printf("event_resize() %d:%d\n", width, height);
    this->rectangle.w = width;
    this->rectangle.h = height;
    screen->set_size(width, height);
    for(int i=0; i < gui_items->items.size(); i++) {
        if(gui_items->items[i].type == GUIitem::ItemType::viewer) {
            gui_items->items[i].frame.w = width;
            gui_items->items[i].frame.h = height;
            gui_items->items[i].screen_buf.get()->lock(5000);
            gui_items->items[i].screen_buf.get()->set_size(width, height);
            gui_items->items[i].screen_buf.get()->unlock(5000);
        }
    }
    
    
}



void GUIwindow::event_paint() {
    screen->fill_all(0x773377);
    screen->line_h(10, 10, 50, 0xff00aa);
    screen->line_h(screen->w-50, screen->h-1, 40, 0xff00aa);
    //screen->print(gui->fonts.roboto220, 50, 50, L"123При");
    
    screen->paint_items(gui_items);
    //void print(SCREEN_BUFFER *scr, int x, int y, const wchar_t *text, bool is_pass, uint32_t color, int cursor_pos);
    std::wstring w;
    w = L"(" + std::to_wstring(scr_cr++) + L")";
    screen->fonts->roboto150->print(screen.get(), 50, 50, w, false, 0xff00, -1 );
}

/*void GUIwindow::exit() {
    printf("exit()\n");
#ifdef __linux__
    XFreeGC (linux_display, gc);
    XDestroyWindow(linux_display, window);
#endif
#ifdef _WIN32
    PostQuitMessage(0);
#endif
}*/


bool GUIwindow::window_close() {
    
    return true;
}

void GUIwindow::execute() {

    
#ifdef __linux__
    XEvent event;
    XEvent exppp;
    
    while(need_stop == false) {
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
            
            switch (event.type) 
            {
                case Expose:
                    event_paint();

                    XLockDisplay(linux_display);
                    XPutImage(linux_display, window, gc, image, 0, 0, 0, 0, screen->w, screen->h);//DISPLAY_WIDTH, DISPLAY_HEIGHT);
                    XUnlockDisplay(linux_display);


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
                        XLockDisplay(linux_display);
                        XSendEvent(linux_display, window, False, ExposureMask, &exppp);
                        XUnlockDisplay(linux_display);
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
                    printf("15 ClientMessage\n");

                    if (event.xclient.data.l[0] == wm_delete)
                    {
                        set_need_stop();
                    }

                    break;
                case DestroyNotify:
                    printf("DestroyNotify\n");
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
                    window_user_event.keycode = convert_key_from_linux(event.xkey.keycode);
                    StaticUserInput(parent, &window_user_event);
                    
                    break;
                case ButtonRelease:
                    //printf("19\n");
                    window_user_event.type = WindowUserEvent::UserEventType::uev_MouseUnpress;
                    window_user_event.x = event.xbutton.x;
                    window_user_event.y = event.xbutton.y;
                    StaticUserInput(parent, &window_user_event);
                    break;
                case UnmapNotify:
                    printf("UnmapNotify\n");
                    break;
                default: 
                    printf("other %d\n", event.type);
            } 
            
        }
        else
        {
            if(need_stop == false) {
                /*XEvent      exppp;
                XLockDisplay(linux_display);
                memset(&exppp, 0, sizeof(exppp));
                exppp.type = Expose;
                exppp.xexpose.window = window;
                XSendEvent(linux_display, window, False, ExposureMask, &exppp);
                XUnlockDisplay(linux_display);*/
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
        std::wstring class_name = L"ClassName_";
        for(int i=0;i<name.size();i++) class_name += name[i];
	wc.lpszClassName = class_name.c_str();

	if (!RegisterClassEx(&wc)) {
                printf("RegisterClass error \n");
		fatal_error("RegisterClass error");
		return;
	};
        
    int x, y, w, h;
    x = rectangle.getx(); y = rectangle.gety(); w = rectangle.getw(); h = rectangle.geth();
    printf("CreateWindow %d %d\n", w, h);
    gui->win32_param.p1 = 1001;
    screen->set_size(rectangle.getw(), rectangle.geth());
    viewer_window_hwnd = CreateWindow( class_name.c_str(),  class_name.c_str(), WS_POPUP, 100, 100, w, h, NULL, NULL, gui->hInstance, this);
	//app_attributes.desktop_window_hwnd = CreateWindowEx( 0, L"VISIATOR VIEWER", L"VISIATOR", WS_OVERLAPPEDWINDOW, x, y, w, h, NULL, NULL, app_attributes.hInstance, NULL);

	if (!viewer_window_hwnd) {
        err = GetLastError();
		fatal_error("CreateWindow error++++++++++++++");
		return;
	};
    printf("11111++++ %x %s\n", viewer_window_hwnd, this->name.c_str());
        
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

void GUIwindow::need_refresh() {
    
#ifdef _WIN32
    RECT r;
    r.left = 0;
    r.top = 0;
    r.right = 5000;
    r.bottom = 5000;
    InvalidateRect(viewer_window_hwnd, nullptr, FALSE);
#endif
#ifdef __linux__  
    XEvent      exppp;
    XLockDisplay(linux_display);
    memset(&exppp, 0, sizeof(exppp));
    exppp.type = Expose;
    exppp.xexpose.window = window;
    XSendEvent(linux_display, window, False, ExposureMask, &exppp);
    XUnlockDisplay(linux_display);
    
#endif
}

void GUIwindow::set_size(int w, int h) {
#ifdef _WIN32

#endif
#ifdef __linux__
    XLockDisplay(linux_display);
    XResizeWindow(linux_display, window, w, h);
    XSync(linux_display, False);
    //XFlush(linux_display);
    XUnlockDisplay(linux_display);
#endif
}