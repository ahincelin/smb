
#include "_win32_xlib_common.h"


#define NB_MAX_BITMAPS 256

static Gdiplus::Bitmap * bitmap_buffer[NB_MAX_BITMAPS];
static int index_free_bitmap = 0;


void x_put_masked_pbm(  /* Copy a planar bitmap from SRAM masking */
    WORD x0,    /* only non zero pixels to VRAM           */
    WORD y0,
    WORD page,
    BYTER far * Bitmap)
{
    int index = Bitmap[0];

    assert(index<index_free_bitmap);
    assert(page<NB_MAX_PAGES);

    Gdiplus::Bitmap * curr_bm = bitmap_buffer [ index ];
    xlib_g[page]->DrawImage(curr_bm, (int)x0, (int)y0);
}


int x_bm_to_pbm(           /* Convert from linear bitmap to planar */
    char far * src_bm,
    char far * dest_pbm)
{
    int i, j, index;
    int width = src_bm[0];
    int height = src_bm[1];

    Gdiplus::Bitmap * curr_bm = new Gdiplus::Bitmap(width, height);

    index = 2;
    for(j=0; j<height; j++)
    {
        for(i=0; i<width; i++)
        {
            int color_index = src_bm[index];
            if ( color_index ) {
                assert(xlib_smb_colors[color_index] != NULL);
                curr_bm->SetPixel(i, j, *xlib_smb_colors[color_index] );
            }
            index++;
        }
    }

    dest_pbm[0] = index_free_bitmap;

    bitmap_buffer[index_free_bitmap++] = curr_bm;

    return OK;
}
