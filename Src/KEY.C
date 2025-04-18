/*
  file: KEY.C

  purpose:
    new keyboard interrupt handler and various keyboard routines
    based on code from ZKEY.ASM

  date: 11/4/95

  note:
    the key interrupts are not passed down or "chained" to the previous
    interrupt handler.
*/
#include "key.h"
#include "xlib.h"
#include "gamedefs.h"

#define KEYBOARD_PORT     0x60
#define KEYBOARD_XT_CLEAR 0x61

void far interrupt new_int09_handler(void);
void far interrupt (*old_int09_handler)();

volatile BYTER far *keybuf = NULL;

static int new_key_handler_installed = FALSE;

void install_new_key_handler(void)
{
  keybuf = (unsigned char *) farmalloc(KEY_WAS_PRESSED * 2);
  clear_key_buffer();

#if 0
  old_int09_handler = getvect(9);
  setvect(9, new_int09_handler);
#endif
  new_key_handler_installed = TRUE;
}

#if 0
void far interrupt new_int09_handler(void)
{
  BYTER scan_code;
  BYTER temp;

  /* read scan code */
  scan_code = inportb(KEYBOARD_PORT);

  /* clear keyboard controller on XT machines */
  temp = inportb(KEYBOARD_XT_CLEAR);
  outportb(KEYBOARD_XT_CLEAR, temp | 0x80);
  outportb(KEYBOARD_XT_CLEAR, temp & 0x7F);

  if (scan_code & 0x80) {
    /* key up */
    scan_code &= 0x7F;
    keybuf[scan_code] = 0;
  }
  else {
    /* key down */
    keybuf[scan_code] = 1;
    keybuf[KEY_WAS_PRESSED+scan_code] = 1;
  }

  outportb(PIC, NONSPECIFIC_EOI);
}
#endif

void clear_key_buffer(void)
{
  int i;
  for (i = 0; i < KEY_WAS_PRESSED * 2; i++)
    keybuf[i] = 0;
}

void uninstall_new_key_handler(void)
{
  if (new_key_handler_installed) {
    farfree((void*)keybuf);
    keybuf = NULL;
#if 0
    setvect(9, old_int09_handler);
#endif
  }
}
