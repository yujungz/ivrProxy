#ifndef __PIG_H__
#define __PIG_H__

#ifdef WIN32
//#include <afxwin.h>
#else
#include <sys/time.h>
#include <sys/systeminfo.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#include <time.h>
#include <string.h>

#ifdef WIN32
	#define PIGTHREADHEAD       DWORD WINAPI
	#define PIGTHREADTAIL       LPVOID
	#define PIGTHREADBEGIN      {
	#define PIGTHREADEND        return 0; }
	#define PIGASSERT(arg)         ASSERT(arg)
	#define PIGSOCKET           SOCKET
	#define PigCloseSocket      closesocket
	#define PIGBOOL				BOOL
	#define PIGTRUE				TRUE
	#define PIGFALSE			FALSE
	#define PIG_CREATE_SUSPENDED	CREATE_SUSPENDED
	typedef DWORD ( WINAPI * PIGTHREAD1ARG )( LPVOID ) ;
	typedef LPVOID              PIGTHREAD2ARG;
#else
	#define PIGTHREADHEAD       void
	#define PIGTHREADTAIL       void*
	#define PIGTHREADBEGIN      {
	#define PIGTHREADEND        }
	#define PIGASSERT(arg)    ;
	#define PIGSOCKET         int
	#define PigCloseSocket      close
	#define PIGBOOL				int
	#define PIGTRUE				1
	#define PIGFALSE			0
	#define PIG_CREATE_SUSPENDED	THR_SUSPENDED
	typedef void*       (*PIGTHREAD1ARG)(void*);
	typedef void*         PIGTHREAD2ARG;
#endif

void PigSleep(unsigned long milSec);
int  PigIsdigit(char ch);
char* PigItoa(int value, char *string, int radix=10 );
int   PigMakeLower(char* s);
int   PigStrAlNum(char* s);

#endif
