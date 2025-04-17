
#include "_win32_xlib_common.h"

void x_graph_char(int xc, int yc, char c, int color, int page)
{
    Gdiplus::Bitmap *bitmap_char = new Gdiplus::Bitmap(6, 8);

    int x,y;
    unsigned char far *work;
    unsigned char bitmask=0x80;

    assert(color<NB_MAX_COLORS);
    assert(page<NB_MAX_PAGES);

    work=charset+(((unsigned int)c-32)*6);
    for(y=0;y<8;y++)
    {
        for(x=0;x<6;x++)
        {
            if((*work & bitmask)) {
                bitmap_char->SetPixel(x, y, *xlib_smb_colors[color]);
            }
            bitmask=(bitmask>>1);
            if ( !bitmask ) {
                bitmask = 0x80;
                work++;
            }
        }
    }

    xlib_g[page]->DrawImage(bitmap_char, xc, yc);
}

