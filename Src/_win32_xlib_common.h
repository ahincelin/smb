
#ifndef WIN32_XLIB_COMMON_H
#define WIN32_XLIB_COMMON_H

#include <Windows.h>
#include <GdiPlus.h>
//#include "stdafx.h"
#include <assert.h>

#define far

#include "XLIB.H"
#include "XRECT.H"
#include "XLINE.H"
#include "XPOINT.H"
#include "XBMTOOLS.H"
#include "XPBITMAP.H"
#include "XVSYNC.H"

#define NB_MAX_COLORS 256
#define NB_MAX_PAGES 2

extern Gdiplus::Graphics* xlib_g[];
extern Gdiplus::Color * xlib_smb_colors[];

extern "C" unsigned char far *charset;



#endif /* WIN32_XLIB_COMMON_H */
