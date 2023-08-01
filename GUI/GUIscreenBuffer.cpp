/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */

#include "GUIscreenBuffer.h"
#include "GUIitem.h"
#include "../font/FONT.h"

#include "../tools.h"

void SCREEN_BUFFER::print(FONT *fnt, unsigned int x, unsigned int y, const wchar_t *text, bool is_pass, uint32_t color, int cursor_pos) {
    if(fnt == nullptr) return;
    fnt->print(this, x, y, text, is_pass, color, cursor_pos);
}

void SCREEN_BUFFER::paint_item_editpass16(GUIitem& item) {
    int x = item.get_gx(), y = item.get_gy();
    //rectangle(x, y, item.frame.w, item.frame.h, 0xffccff);

    FONT *font;
    font = fonts->roboto220;
    int text_w = font->text_width(item.edit_text);
    int text_h = font->text_height(item.edit_text);
    uint32_t c = 0x007788;
    if(item.is_edit_begin) {
        c = 0x00ffff;
        font->print(this, x, y + (item.frame.h - text_h)/2, item.edit_text, !item.is_visible_pass, c, item.edit_text_cursor_pos);
    } else {
        font->print(this, x, y + (item.frame.h - text_h)/2, item.edit_text, !item.is_visible_pass, c, -1);
    }
}

void SCREEN_BUFFER::paint_items(GUIitems* items) {
    if(items == nullptr) return;
    for(int i=0; i < items->items.size(); i++) {
        paint_item(items->items[i]);
    }
}

void SCREEN_BUFFER::paint_item_edittext(GUIitem& item) {
    int x = item.get_gx(), y = item.get_gy();
    //rectangle(x, y, item.frame.w, item.frame.h, 0xffccff);

    FONT *font;
    font = fonts->roboto220;
    int text_w = font->text_width(item.edit_text);
    int text_h = font->text_height(item.edit_text);
    uint32_t c = 0x007788;
    if(item.is_edit_begin) {
        c = 0x00ffff;
        font->print(this, x, y + (item.frame.h - text_h)/2, item.edit_text, false, c, item.edit_text_cursor_pos);
    } else {
        font->print(this, x, y + (item.frame.h - text_h)/2, item.edit_text, false, c, -1);
    }
}

void SCREEN_BUFFER::paint_item_editid(GUIitem& item) {
    int x = item.get_gx(), y = item.get_gy();
    //rectangle(x, y, item.frame.w, item.frame.h, 0xffccff);

    FONT *font;
    font = fonts->roboto220;

    int text_w = font->text_width(item.edit_text);
    int text_h = font->text_height(item.edit_text);
    uint32_t c = 0x007788;
    if(item.is_edit_begin) {
        c = 0xff0000;
        font->print(this, x, y + (item.frame.h - text_h)/2, item.edit_text, false, 0x007788, item.edit_text_cursor_pos);
    } else {
        font->print(this, x, y + (item.frame.h - text_h)/2, item.edit_text, false, 0x007788, -1);
    }
}

void SCREEN_BUFFER::paint_item_viewer(GUIitem& item) {
    
    item.screen_buf.get()->lock(1000);
    int ii;
    
    if(item.screen_buf.get()->w < w) { ii = item.screen_buf.get()->w; }
    else { ii = w; };
    
    for(int j=0;j<item.frame.h;j++) {
        for(int i=0;i<ii;i++) {
            this->set_pix(i, j, item.screen_buf.get()->get_pix(i, j));
        }
    }
    
    item.screen_buf.get()->unlock(1000);
}

void SCREEN_BUFFER::paint_item(GUIitem& item) {
    if(item.get_is_visible() == false) {
        return;
    }
    int x = item.get_gx(), y = item.get_gy();
    //rectangle(x, y, item.frame.w, item.frame.h, 0xff00ff);

    if(item.type == GUIitem::ItemType::viewer) {
        paint_item_viewer(item);
        return;
    }
    
    if(item.type == GUIitem::ItemType::editpass16) {
        paint_item_editpass16(item);
        return;
    }
    if(item.type == GUIitem::ItemType::edittext) {
        paint_item_edittext(item);
        return;
    }

    if(item.type == GUIitem::ItemType::editid) {
        paint_item_editid(item);
        return;
    }

    if(item.textura != nullptr) {
        paint_textura(x, y, item.textura);
    }

}

void SCREEN_BUFFER::paint_textura(unsigned int x, unsigned int y, GUItextura* textura) {
    if(textura == nullptr ||
       x < 0 || x + textura->w > w ||
       y < 0 || y + textura->h > h) return;         

    int idx = 0;
    for(int yy = 0; yy < textura->h; yy++) {
        for(int xx = 0; xx < textura->w; xx++) {
            set_pix(x+xx, y+yy, textura->buf[idx++]);
        }
    }
}

void SCREEN_BUFFER::resize_from(SCREEN_BUFFER* src) {
    int ww, hh, xx, yy;
    if(this->w < src->w) { ww = w; } else { ww = src->w; };
    if(this->h < src->h) { hh = h; } else { hh = src->h; };
    
    /*yy = 0;
    while(yy < hh) {
        xx = 0;
        while(xx < ww) {
            uint32_t c;
            c = src->get_pix(xx, yy);
            set_pix(xx, yy, c);
            xx++;
        }
        yy++;
    }*/
    
    tmp_buffer.resize(src->w * src->h);
    
    ////////////////////////////////////////////////////////////////////////////
    
    int dest_w, dest_h;

    float left_dogon, dogon_right;

    int dogon, j_max;

    float koef_w, koef_h;

    int new_window_w = ww;
    int new_window_h = hh;

    // 2023 if (new_window_w > src->w) new_window_w = src->w;
    // 2023 if (new_window_h > src->h) new_window_h = src->h;

    /*if (low_level->window_w == 0) {
            koef_w = 1;
            koef_h = 1;
    }
    else {
            koef_w = (float)this->w / (float)new_window_w;
            koef_h = (float)this->h / (float)new_window_h;
    };*/

    koef_w = (float)src->w / (float)new_window_w;
    koef_h = (float)src->h / (float)new_window_h;
    
    //if (koef_w > 1) koef_w = 1;
    //if (koef_h > 1) koef_h = 1;


    left_dogon = 0.0;
    dogon = (int)koef_w;
    dogon_right = koef_w - (float)dogon;

    dest_w = (int)(src->w / koef_w);
    dest_h = (int)(src->h / koef_h);

    unsigned int *d, *s;
    
    d = tmp_buffer.data();// src->buffer;
    s = src->buffer;
    int x, y;

    float count;

    yy = src->h;
    y = 0;

    int r, g, b, j;

    j_max = src->w * src->h;
    
    while (y < yy) {

        left_dogon = 0.0;
        dogon = (int)koef_w;
        dogon_right = koef_w - (float)dogon;

        d = tmp_buffer.data();//src->buffer;
        s = src->buffer;

        d += y * src->w;
        s += y * src->w;

        x = 0;
        xx = dest_w;
        while (x < xx) {
                r = 0;
                g = 0;
                b = 0;

                j = (int)((float)x * koef_w);

                if (left_dogon != 0) {
                        if (j < j_max) {
                                r = (int)((float)(s[j] & 0x0000ff) * left_dogon);
                                g = (int)((float)((s[j] & 0x00ff00) >> 8) * left_dogon);
                                b = (int)((float)((s[j] & 0xff0000) >> 16) * left_dogon);
                        }
                        j++;
                };

                for (int i = 0; i < dogon; i++) {
                        if (j < j_max) {
                                r += (s[j] & 0x0000ff);
                                g += ((s[j] & 0x00ff00) >> 8);
                                b += ((s[j] & 0xff0000) >> 16);
                        };
                        j++;
                };

                if (dogon_right > 0) {
                        if (j < j_max) {
                                r += (int)((float)(s[j] & 0x0000ff) * dogon_right);
                                g += (int)((float)((s[j] & 0x00ff00) >> 8) * dogon_right);
                                b += (int)((float)((s[j] & 0xff0000) >> 16) * dogon_right);
                        };
                };
                r = (int)(r / koef_w);

                if (r > 255) {
                        r = 255;
                }


                g = (int)(g / koef_w);
                if (g > 255) {
                        g = 255;
                }
                g = g << 8;

                //b = b >> 16;
                b = (int)(b / koef_w);
                if (b > 255) {
                        b = 255;
                }
                b = b << 16;

                count = left_dogon;

                count += dogon;

                count += dogon_right;

                left_dogon = 1 - dogon_right;
                dogon = (int)(koef_w - left_dogon);
                dogon_right = koef_w - (float)dogon - left_dogon;

                *d = r | g | b;
                d++;

                x++;
        }

        y++;
    }
    
    /*yy = 0;
    while(yy < 150) {
        xx = 0;
        while(xx < 200) {
            uint32_t c;
            c = src->get_pix(xx, yy);
            set_pix(xx, yy, c);
            xx++;
        }
        yy++;
    }*/
    
    
    
    	x = 0;
	while (x < dest_w) {

		left_dogon = 0.0;
		dogon = (int)koef_h;
		dogon_right = koef_h - (float)dogon;

		d = this->buffer;
		s = tmp_buffer.data();//src->buffer;

		d += x;
		s += x;

		j_max = src->w * src->h;

		y = 0;
		while (y < dest_h) {

			r = 0;
			g = 0;
			b = 0;

			j = (int)((float)y * koef_h) * src->w;

			if (left_dogon != 0) {
				if (j < j_max) {
					r = (int)((float)(s[j] & 0x0000ff) * left_dogon);
					g = (int)((float)((s[j] & 0x00ff00) >> 8) * left_dogon);
					b = (int)((float)((s[j] & 0xff0000) >> 16) * left_dogon);
				}
				else {
					r = 255;
					g = 0;
					b = 0;
				}
				j += src->w;
			};

			for (int i = 0; i < dogon; i++) {
				if (j < j_max) {
					r += (s[j] & 0x0000ff);
					g += ((s[j] & 0x00ff00) >> 8);
					b += ((s[j] & 0xff0000) >> 16);
				}
				else {
					r = 255;
					g = 0;
					b = 0;
				};
				j += src->w;
			};

			if (dogon_right > 0) {
				if (j < j_max) {
					r += (int)((float)(s[j] & 0x0000ff) * dogon_right);
					g += (int)((float)((s[j] & 0x00ff00) >> 8) * dogon_right);
					b += (int)((float)((s[j] & 0xff0000) >> 16) * dogon_right);
				}
				else {
					r = 255;
					g = 0;
					b = 0;
				};
			};

			r = (int)(r / koef_h);
			g = (int)(g / koef_h);
			g = g << 8;
			b = (int)(b / koef_h);
			b = b << 16;

			left_dogon = 1 - dogon_right;
			dogon = (int)(koef_h - left_dogon);
			dogon_right = koef_h - (float)dogon - left_dogon;

			*d = r | g | b;
			d += this->w;

			y++;
		}

		x++;
	}

    
}


void SCREEN_BUFFER::lock(int v) {
    while(mutexx.try_lock() == false) {
        
        sleep_();
    }
    lock_owner = v;
}

void SCREEN_BUFFER::unlock(int v) {
    if(lock_owner != v) {
        printf("WTF?\n");
    }
    mutexx.unlock();
    lock_owner = 0;
}