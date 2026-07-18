#ifndef _SC_LOG_H
#define _SC_LOG_H

#include <string.h>
#include <stdio.h>
#include "KernelObject.h"
#include "ctgor.h"


#define LOG_TO_NULL		0
#define LOG_TO_FILE		1
#define LOG_TO_SCREEN	2
#define LOG_TO_SCNFILE	3

#define MAX_FILE_LINES	200000

typedef enum {
	Event_Error=0,
	Event_Warning=1,
	Event_Info=2
} EventType;


class TPrintLog
{
public:
		TPrintLog();
		TPrintLog(CTobj SourceID,char *runlog,int level=0, int mode=LOG_TO_FILE);
		TPrintLog(const char *name,  const char *path="");		
		~TPrintLog() ;

		FILE *Open(const char *name,  const char *path, int mode=LOG_TO_FILE) ;	
				
		int  writeR(int level,char *format,...);
		int  writeRNoTime(int level,char *format,...);		
		int	 writeB(int level,unsigned char *buf,int len);

		int  printf(const char *format,...);
		int  printb( char *buff, int len);
		int  perror( char *fmt, ...);

		void flush() { if (runfp) fflush(runfp); }
		
		int  getTraceMode() { return trace_mode; }
		int  getTraceLevel() { return init_level; }
		int  getFlushFlag() { return flushflag; }
		char *getFilename() { return trace_filename; }

		void setTraceLevel(int level) { init_level = level;	}
		void setTraceMode(int mode);
		void setFlushFlag(int mode){ flushflag=mode;}
		void setMaxLogLen(int length) { if( length > 100 ) maxloglen=length;}

private:
		CTobj source;
		CTCriticalSection theLock;

		char sysPath[80];
		char sysName[64];
		char trace_filename[128];
		char error_filename[128];
		int  run_count;
		FILE *runfp;
		int	 trace_mode;	// LOG_TO_FILE,LOG_TO_SCREEN,NO_LOG
		int  init_level;		
		int  maxloglen;
		int  flushflag;
		int  logvalid;
};

#endif
