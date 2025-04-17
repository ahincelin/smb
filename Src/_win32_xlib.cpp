
#include "_win32_xlib_common.h"

/* SMB color range is 0..63 for each RGB value */
/* multiplying this value by COLOR_FACTOR allows to have higher brightness */
#define COLOR_FACTOR 4

#define PALETTE_SIZE 1024

static Gdiplus::GdiplusStartupInput gdiplusStartupInput;
static ULONG_PTR           gdiplusToken;
static HDC dc;
static Gdiplus::Bitmap *hiddenPageBitmap;
static Gdiplus::Color * xlib_palette[PALETTE_SIZE];

Gdiplus::Graphics* xlib_g[NB_MAX_PAGES];
Gdiplus::Color * xlib_smb_colors[NB_MAX_COLORS];


void xlib_start(HWND hwnd)
{
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    /* set up visible page */
    dc = GetDC(hwnd);
    xlib_g[VisiblePageOffs] = Gdiplus::Graphics::FromHDC( dc );

    /* set up hidden page */
    RECT rc;
    GetClientRect(hwnd, &rc);
    hiddenPageBitmap = new Gdiplus::Bitmap(rc.right, rc.bottom);
    xlib_g[HiddenPageOffs] = new Gdiplus::Graphics(hiddenPageBitmap);

    for (int i=0; i<PALETTE_SIZE; i++)
        xlib_palette[i] = NULL;
}

void xlib_stop()
{
    for (int i=0; i<PALETTE_SIZE; i++)
        if ( xlib_palette[i] )
            delete xlib_palette[i];
    delete xlib_g[HiddenPageOffs];
    delete hiddenPageBitmap;

    Gdiplus::GdiplusShutdown(gdiplusToken);
}

void x_setpalette_old(int index, char red, char green, char blue)
{
    assert(index<NB_MAX_COLORS);

    if ( xlib_palette[index] != NULL ) {
        delete xlib_palette[index];
        xlib_palette[index] = NULL;
    }
    xlib_palette[index] = new Gdiplus::Color(red * COLOR_FACTOR, green * COLOR_FACTOR, blue * COLOR_FACTOR);

    xlib_smb_colors[index] = xlib_palette[index];
}

void x_setpalette(int smb_index, char red, char green, char blue)
{
	int xlib_index;
	Gdiplus::ARGB rgbColor;

    assert(smb_index<NB_MAX_COLORS);

	// first, find if color has already been allocated in palette
	// if not, allocate.
    rgbColor = Gdiplus::Color::MakeARGB(255, red * COLOR_FACTOR, green * COLOR_FACTOR, blue * COLOR_FACTOR);
	xlib_index = 0;
	while ( xlib_palette[xlib_index] != NULL)
	{
		if ( xlib_palette[xlib_index]->GetValue() == rgbColor )
			break;
		xlib_index++;
	}
	if ( (xlib_palette[xlib_index] == NULL) && (xlib_index<PALETTE_SIZE) )
	{
        xlib_palette[xlib_index] = new Gdiplus::Color(rgbColor);
	}
	
	// assign color in palette to smb index
	if ( xlib_index<PALETTE_SIZE )
		xlib_smb_colors[smb_index] = xlib_palette[xlib_index];
}

WORD x_set_mode(
    WORD mode,                /* Initialize x mode           */
    WORD WidthInPixels)
{
    return OK;
}

WORD x_set_tripplebuffer(         /* Enable tripple buffering with a    */
    WORD PageHeight) /* specified page height             */
{
    return OK;
}


void x_page_flip(        /* Swap visible page (only when double   */
    WORD X,   /* buffering is active) & sets virt start*/
    WORD Y)
{
    xlib_g[VisiblePageOffs]->DrawImage(hiddenPageBitmap, 0, 0);
}

void x_text_mode(void)   /* return to text mode */
{
}

void x_rect_fill(         /* draw a single colour filled rectangle */
    WORD x0,
    WORD y0,
    WORD x1,
    WORD y1,
    WORD page,
    WORD color)
{
    assert(color<NB_MAX_COLORS);
    assert(page<NB_MAX_PAGES);

    Gdiplus::SolidBrush brush(*xlib_smb_colors[color]);
    xlib_g[page]->FillRectangle(&brush, (int)x0, (int)y0, (int)(x1+1-x0), (int)(y1+1-y0));
}

void x_put_pix(           /* Draw a pixel on the screen */
    WORD x0,
    WORD y0,
    WORD page,
    WORD color)
{
    assert(color<NB_MAX_COLORS);
    assert(page<NB_MAX_PAGES);

    Gdiplus::SolidBrush brush(*xlib_smb_colors[color]);
    xlib_g[page]->FillRectangle(&brush, x0, y0, 1, 1);
}

void x_line(     /* Draw a line, what else */
    WORD x0,
    WORD y0,
    WORD x1,
    WORD y1,
    WORD color,
    WORD page)
{
    assert(color<NB_MAX_COLORS);
    assert(page<NB_MAX_PAGES);

    Gdiplus::Pen p(*xlib_smb_colors[color]);
    xlib_g[page]->DrawLine(&p, (int)x0, (int)y0, (int)x1, (int)y1);
}

void x_install_vsync_handler(int skipcount)
{
}

void x_remove_vsync_handler(void)
{
}

void xlib_batch_start()
{
}

void xlib_batch_end()
{
}
