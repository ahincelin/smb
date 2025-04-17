#include<stdio.h>
#include<stdlib.h>
//#include<conio.h>
//#include<mem.h>
//#include<dos.h>
#include<string.h>
//#include<alloc.h>
#include<time.h>
#include "key.c"		//KEYBOARD HANDLER FILE
				//(f:\smb\ = directory of file key.c)

#include "xlib.h"
#include "xline.h"        //Xlib Header files for Mode X graphics
#include "xrect.h" 	//Functions!
#include "xpoint.h"	//(f:\smb\ = directory of x*.h)
#include "xvsync.h"
#include "xpbitmap.h"
#include "xbmtools.h"

#define TEXTCOLOR  35
#define BACKGROUND 32

typedef struct
{
  char red;
  char green;
  char blue;
}colorpal;

//DEFINE GRAPHICS ARRAYS/////////////////////////////////////////////////////
typedef struct
{
  char mario[7][16][16];
  char far *block[52];
  char world[14][217];
  char far *coin[3];
  short int bumped[2];
  short int bumploop;
  short int sprite;
  short int direction;
  short int maxlength;
  short int startpos[2];
  colorpal palette[256];
  colorpal marioc[2];
  colorpal luigic[2];
} graphics;

//PLAYER INFO STRUCTURE//////////////////////////////////////////////////////
typedef struct
{
  unsigned int score[2];
  short int coins[2];
  short int level;
  short int lives;
  short int screenx;
} stats;

typedef struct ct
{
  short int coinx;
  short int coiny;
  short int coinloop;
  short int dy;
  struct ct *next;
}cointype;

typedef struct st
{
  short int scorex;
  short int scorey;
  short int scoreloop;
  char num[5];
  struct st *next;
}scoretype;

//GLOBAL VARIABLES///////////////////////////////////////////////////////////
extern unsigned char far *charset;
graphics data;
stats player[2];
struct smb_time
{
	int ti_sec;
} t;
unsigned int times[3];
short int curplay;
int mariox;
int marioy;
short int landed;
cointype *bcoin;
scoretype *bscore;

//FUNCTION PROTOTYPES////////////////////////////////////////////////////////
void init();
void fillscreen(int color);
int  getinputs();
void getmario();
char *getpcx(FILE *file, char *temp, int length);
void setpalette(int index, colorpal color);
void graphchar(int xc, int yc, char c,int color, unsigned offset);
void drawscreen(int x);
void graphstring(int x,int y,char *string,int color,unsigned offset);
void drawmario(int x, int y);
void status();
int startscreen();
void levelscreen();
void setcolors();
void rotatepal(int *palloopp, colorpal *tempcp);
void check_keybuf(int *movep, int *finishp, int *upp, int *downp, int *jumploop);
void animate_mario(int move);
void move_mario(int *movep);
void addcoin(int x,int y);
void removecoin();
void drawcoin(int x);
void addscore(int x,int y, char *pscore);
void removescore();
void drawscore(int x);
void exitgame();
void check_jump(int *upp, int *downp, int *jumploopp);
void check_blocks();
void check_finish(int *finishp);



