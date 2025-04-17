
#include <windows.h>
#include <time.h>
#include <stdio.h>

/* fonctions importées */
extern void smb_main();

struct smb_time
{
    int ti_sec;
} t;

/* fonctions exportées */
extern void run_smb();
extern void stop_smb();
extern void os_task_yield();
extern void os_sleep(int sec);
extern void gettime(struct smb_time * t);
extern int fgetc_no_cr(FILE *stream);


static HANDLE thrHdle = 0;
static DWORD  thrId = 0;

DWORD WINAPI smb_thread( LPVOID lpParam ) 
{
    smb_main();
    return 0;
}

void run_smb()
{
    thrHdle = CreateThread( 
        NULL,                   // default security attributes
        0,                      // use default stack size  
        smb_thread,             // thread function name
        NULL,                   // argument to thread function 
        0,                      // use default creation flags 
        &thrId);                // returns the thread identifier 
}

void stop_smb()
{
    if ( thrHdle )
        TerminateThread(thrHdle, 0);
}

void os_task_yield()
{
    Sleep(8);
}

void gettime(struct smb_time * t)
{
    time_t val = 0;
    time(&val);
    if (t) t->ti_sec = (int)val;
}

void os_sleep(int sec)
{
    Sleep(sec * 1000);
}

void os_notify(char * msg)
{
    printf("%s\n", msg);
}

int fgetc_no_cr(FILE *stream)
{
    int val;

    /* "eat" carriage return */
    do {
        val = fgetc(stream);
    } while ( val == '\r' );

    return val;
}

