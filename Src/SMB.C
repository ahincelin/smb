/****************************************************************************
*Super Mario Bros.							    *
*Copyright 1985 Nintendo						    *
*Coded by:								    *
*  William Merrill Buerger						    *
*  Buergerw@warp.msoe.edu						    *
*  http://www.msoe.edu/~buergerw					    *
*With help from:							    *
*  John Ratke		-Keyboard Handler				    *
*  Themie Gouthas	-XLIB graphics stuff				    *
****************************************************************************/

#define interrupt
#define far
#define farmalloc malloc
#define farfree free

#ifdef __VXWORKS__
extern char * itoa(int value, char * str, int base);
#endif

#include "smb.h"			//Main Header file (f:\smb\ = path of smb.h)//

extern void gettime(struct smb_time * t);
extern int fgetc_no_cr(FILE *stream);

#include "smbfunc.c"		//Function file//

//MAIN///////////////////////////////////////////////////////////////////////
void smb_main()
{
  int move;			//DEFINE & INITIALIZE LOCAL VARIABLES//
  int up;
  int down=0;
  int finish=0;
  int jumploop=0;
  int numplayers;
  int palloop;
  colorpal tempc;

  init();
  curplay=0;
  getmario();
  getinputs();
  os_notify("Display start screen");
  numplayers=startscreen();
  if (numplayers==1)
    player[1].lives=0;

//MAIN GAME LOOP/////////////////////////////////////////////////////////////
  while(finish!=3)
  {
    setcolors();
    times[0]=201;
    os_notify("Display level screen");
    levelscreen();
    marioy=getinputs();
    if (player[curplay].screenx>=data.startpos[0]*16)
      player[curplay].screenx=data.startpos[0]*16;
    else
      player[curplay].screenx=0;
    if (player[curplay].screenx>0)
      marioy=data.startpos[1];
    mariox=40;
    up=0;
    move=0;
    down=0;
    palloop=0;
    tempc=data.palette[21];
    data.sprite=6;
    gettime(&t);
    times[1]=t.ti_sec;
    times[2]=times[1]-1;
    os_notify("Entering level and player loop");
    fillscreen(BACKGROUND);
    while(!finish)			//CURRENT LEVEL AND PLAYER LOOP//
    {
      os_task_yield();
      xlib_batch_start();
      x_page_flip(0,0);
      xlib_batch_end();
      rotatepal(&palloop,&tempc);
      xlib_batch_start();
      fillscreen(BACKGROUND);
      drawscreen(player[curplay].screenx);
      drawmario(mariox,marioy);
      xlib_batch_end();
      gettime(&t);
      times[1]=t.ti_sec;
      if (marioy<207)
	check_keybuf(&move,&finish,&up,&down,&jumploop);
      animate_mario(move);
      move_mario(&move);
      check_jump(&up,&down,&jumploop);
      check_blocks();
      check_finish(&finish);
    }

//REACT TO END OF CURRENT PLAYER LOOP////////////////////////////////////////
    switch (finish)
    {
      case 1:
	player[curplay].lives--;
	finish=0;
	if (numplayers==2)
	{
	  if (curplay==0 && player[1].lives!=0)
	    curplay=1;
	  else
	    if (curplay==1 && player[0].lives!=0)
	      curplay=0;
	}
	if (player[0].lives==0 && player[1].lives==0)
	  finish=3;
	break;
      case 2:
	player[curplay].level++;
	if (player[curplay].level%5==0)
	  player[curplay].level++;
	player[curplay].screenx=0;
	finish=0;
	break;
      case 3:
	graphstring(130,100,"Game Over",TEXTCOLOR,VisiblePageOffs);
//	while(!keybuf[KEY_ENTER]);
	break;
    }
  }

//RETURN SCREEN AND KEYBOARD TO NORMAL///////////////////////////////////////
  exitgame();
}
//END////////////////////////////////////////////////////////////////////////
