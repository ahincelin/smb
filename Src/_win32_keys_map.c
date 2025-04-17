
#include <windows.h>

#define far

#include "KEY.H"
#include "XLIB.H"

/* fonctions exportées */
extern int handle_key_down(WPARAM wParam);
extern int handle_key_up(WPARAM wParam);


/* fonctions importées */
extern volatile BYTER far *keybuf;


static int get_smb_key(int win_key)
{
    int smb_key;

    switch(win_key)
    {
    case VK_LEFT:
        smb_key = KEY_LEFT;
        break;
    case VK_RIGHT:
        smb_key = KEY_RIGHT;
        break;
    case VK_UP:
        smb_key = KEY_UP;
        break;
    case VK_DOWN:
        smb_key = KEY_DOWN;
        break;
    case VK_RETURN:
        smb_key = KEY_ENTER;
        break;
    case VK_CONTROL:
        smb_key = KEY_CTRL;
        break;
    case VK_SPACE:
        smb_key = KEY_ALT;
        break;
    case 'P':
        smb_key = KEY_P;
        break;
    case 'Q':
        smb_key = KEY_Q;
        break;
    case 'Y':
        smb_key = KEY_Y;
        break;
    case 'N':
        smb_key = KEY_N;
        break;
    default:
        smb_key = -1;
        break;
    }

    return smb_key;
}


int handle_key_down(WPARAM wParam)
{
    int rv = 0;
    int smb_key = get_smb_key(wParam);

    if ( smb_key != -1 )
    {
        if ( keybuf ) {
            keybuf[smb_key] = 1;
            keybuf[KEY_WAS_PRESSED+smb_key] = 1;
        }
        rv = 1;
    }

    return rv;
}

int handle_key_up(WPARAM wParam)
{
    int rv = 0;
    int smb_key = get_smb_key(wParam);

    if ( smb_key != -1 )
    {
        if ( keybuf ) {
            keybuf[smb_key] = 0;
        }
        rv = 1;
    }

    return rv;
}
