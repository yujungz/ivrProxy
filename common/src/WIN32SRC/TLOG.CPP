#include <string.h>
#include <io.h>
#include "tlog.h"

#define LOG_VALID	1 //决定不写LOG  1---写,0--不写  ivr.printmsg.cpp 也要改

TPrintLog::TPrintLog() : logvalid(LOG_VALID),
	init_level(9), trace_mode(LOG_TO_NULL), source(0), 
	run_count(0),maxloglen(MAX_FILE_LINES),flushflag(1)
{
	memset( sysPath, 0, 80);
	memset( sysName, 0, 64);
	memset( trace_filename, 0, 128);
	memset( error_filename, 0, 128);
	runfp = NULL;
}

TPrintLog::TPrintLog(CTobj SourceID,char *runlog,int level,int out) :
	logvalid(LOG_VALID), init_level(level), trace_mode(out), source(SourceID), 
	run_count(0),maxloglen(MAX_FILE_LINES),flushflag(1)
{	
	strcpy( sysName, "log" ) ;
	strcpy( sysPath, "" ) ;
	strcpy( error_filename, sysPath);
	strcat( error_filename, sysName ); 
	strcat( error_filename, ".err" );

	strcpy(trace_filename,runlog);
	runfp = NULL;
	
	if(logvalid == 0) return;

	if( access( trace_filename,0 ) != -1 ) {

		char filename1[60];
		int len=0;
		char *p=strrchr(trace_filename,'.');
		memset(filename1,0,60);
		if(p)
		{
			len = strlen(p);
			len = strlen(trace_filename)-len;
		}
		if(len>0)
			strncpy(filename1,trace_filename,len);
		else
			strcpy(filename1,trace_filename);

		char backup_filename[128] ;
		memset(backup_filename,0,128);
		//sprintf( backup_filename, "%s_bak", trace_filename ) ;
		char buf[20];
		memset(buf,0,20);
		time_t now=time(NULL);	
		struct tm *ts=localtime(&now);
		if (ts)
			sprintf( buf, "%04d%02d%02d%02d%02d.log",(ts->tm_year+1900), ts->tm_mon+1, ts->tm_mday,ts->tm_hour,ts->tm_min);
		sprintf( backup_filename, "%s%s", filename1,buf) ;

		unlink( backup_filename ) ;
		rename( trace_filename, backup_filename ) ;
	}
	
	runfp=fopen(trace_filename,"w");	
}

TPrintLog::TPrintLog(const char *name,  const char *path)  :
	logvalid(LOG_VALID), init_level(9), trace_mode(LOG_TO_FILE), source(0), 
	run_count(0),maxloglen(MAX_FILE_LINES),flushflag(1)
{
	if( name && strlen(name )  )
		strcpy( sysName, name ) ;
	else
		strcpy( sysName, "log" ) ;

	strcpy( sysPath, path ) ;
	strcpy( error_filename, sysPath);
	strcat( error_filename, sysName ); 
	strcat( error_filename, ".err" );

	strcpy( trace_filename, sysPath);
	strcat( trace_filename, sysName ); 	
	runfp = NULL;

	if(logvalid == 0) return;
	
	runfp=fopen(trace_filename,"wt");	
}

TPrintLog::~TPrintLog()
{		
	if(runfp)
	{
		fclose(runfp);
		runfp = NULL;
	}
}

FILE *TPrintLog::Open(const char *name,  const char *path, int mode) 
{
	if(logvalid == 0) return NULL;

	trace_mode = mode ;
	if( mode != LOG_TO_NULL && mode != LOG_TO_FILE)
			trace_mode = LOG_TO_FILE;

	if( name && strlen(name )  )
		strcpy( sysName, name ) ;
	else
		strcpy( sysName, "log" ) ;

	strcpy( sysPath, path ) ;
	strcpy( error_filename, sysPath);
	strcat( error_filename, sysName ); 
	strcat( error_filename, ".err" );

	strcpy( trace_filename, sysPath);
	strcat( trace_filename, sysName ); 	
	if( mode == LOG_TO_FILE)
	{
		if( runfp )
			fclose(runfp);

		runfp=fopen(trace_filename,"wt");
		return runfp;
	}

	return NULL;
}

int	TPrintLog::writeB(int level,unsigned char *buf,int len)
{
	if(logvalid == 0 || runfp == 0 ) return 0;

	if( level>init_level || trace_mode == LOG_TO_NULL )
		return 1;
	char outbuf[4096];
	char *p = outbuf;
	int count = 0;
	p[0] = 0;
	sprintf(p," ");
	p+=1;
	for(int i=0;i<len;i++)
	{
		sprintf(p," %02x",buf[i]);
		p += 3;
		count ++;
		if((count & 0x1f) == 0 )
		{
			sprintf(p,"\n ");
			p += 2;
		}
	}
	sprintf(p,"\n");
	return writeRNoTime(level,(char*)"%s",outbuf);
}

int TPrintLog::writeR(int level,char *format,...)
{
	if(logvalid == 0 || runfp == 0) return 0;

	if( level>init_level || trace_mode == LOG_TO_NULL )
		return 1;

	theLock.Lock();    
	va_list marker;	
	va_start(marker,format);
    	
    if(trace_mode == LOG_TO_FILE) 
	{	
		time_t now=time(0);	
        struct tm *ts=localtime(&now);
		if (ts)
           fprintf( runfp, "%04d-%02d-%02d %02d:%02d:%02d ", ts->tm_year+1900, ts->tm_mon+1, ts->tm_mday, ts->tm_hour, ts->tm_min, ts->tm_sec );
	
		vfprintf(runfp, format, marker ) ;

        run_count++;
        if( run_count >= maxloglen ) 
		{
            fclose( runfp );
            run_count = 0;

			char filename1[60];
			int len=0;
			char *p=strrchr(trace_filename,'.');
			memset(filename1,0,60);
			if(p)
			{
				len = strlen(p);
				len = strlen(trace_filename)-len;
			}
			if(len>0)
				strncpy(filename1,trace_filename,len);
			else
				strcpy(filename1,trace_filename);
            
			char backup_filename[128];
			//sprintf( backup_filename, "%s_bak", trace_filename ) ;            
			char buf[20];
			memset(buf,0,20);
			time_t now=time(NULL);	
			struct tm *ts=localtime(&now);
			if (ts)
				sprintf( buf, "%04d%02d%02d%02d%02d.log",(ts->tm_year+1900), ts->tm_mon+1, ts->tm_mday,ts->tm_hour,ts->tm_min);
			sprintf( backup_filename, "%s%s", filename1,buf) ;

			unlink(backup_filename);
            rename( trace_filename, backup_filename);			
			runfp = fopen( trace_filename, "wt" );			
       }
       if( flushflag )
			fflush(runfp);
	}

	va_end(marker);
	theLock.Unlock();   
		
	return 1;
}
		
int TPrintLog::writeRNoTime(int level,char *format,...)
{
	if(logvalid == 0 || runfp == 0) return 0;

	if( level>init_level || trace_mode == LOG_TO_NULL )
		return 1;
	
	theLock.Lock();    
	va_list marker;	
	va_start(marker,format);
    	
    if(trace_mode == LOG_TO_FILE) 
	{	
		vfprintf(runfp, format, marker ) ;

        run_count++;
        if( run_count >= maxloglen ) 
		{
            fclose( runfp );
            run_count = 0;
			char filename1[60];
			int len=0;
			char *p=strrchr(trace_filename,'.');
			memset(filename1,0,60);
			if(p)
			{
				len = strlen(p);
				len = strlen(trace_filename)-len;
			}
			if(len>0)
				strncpy(filename1,trace_filename,len);
			else
				strcpy(filename1,trace_filename);

            char backup_filename[128];
			memset(backup_filename,0,128);
			//sprintf( backup_filename, "%s_bak", trace_filename ) ;            
			char buf[20];
			memset(buf,0,20);
			time_t now=time(NULL);	
			struct tm *ts=localtime(&now);
			if (ts)
				sprintf( buf, "%04d%02d%02d%02d%02d.log",(ts->tm_year+1900), ts->tm_mon+1, ts->tm_mday,ts->tm_hour,ts->tm_min);
			sprintf( backup_filename, "%s%s", filename1,buf) ;

			unlink(backup_filename);
            rename( trace_filename, backup_filename);			
			runfp = fopen( trace_filename, "wt" );			
       }
       if( flushflag )
			fflush(runfp);
	}

	va_end(marker);
	theLock.Unlock();   
		
	return 1;
}

int TPrintLog::printf(const char *format,...)
{	
	if(logvalid == 0 || runfp == 0) return 0;

	if( trace_mode == LOG_TO_NULL )
		return 1;

	theLock.Lock();    
	va_list marker;
	
	va_start(marker,format);    
	    
    if(trace_mode == LOG_TO_FILE) 
	{	
		time_t now=time(0);	
        struct tm *ts=localtime(&now);
		if (ts)
           fprintf( runfp, "%4d/%02d/%02d %02d:%02d:%02d ", ts->tm_year+1900, ts->tm_mon+1, ts->tm_mday, ts->tm_hour, ts->tm_min, ts->tm_sec );
		
		vfprintf(runfp, format, marker ) ;

        run_count++;
        if( run_count >= maxloglen ) 
		{
            fclose( runfp );
            run_count = 0;
			char filename1[60];
			int len=0;
			char *p=strrchr(trace_filename,'.');
			memset(filename1,0,60);
			if(p)
			{
				len = strlen(p);
				len = strlen(trace_filename)-len;
			}
			if(len>0)
				strncpy(filename1,trace_filename,len);
			else
				strcpy(filename1,trace_filename);

            char backup_filename[128];
			memset(backup_filename,0,128);
			//sprintf( backup_filename, "%s_bak", trace_filename ) ;            

			char buf[20];
			memset(buf,0,20);
			time_t now=time(NULL);	
			struct tm *ts=localtime(&now);
			if (ts)
				sprintf( buf, "%04d%02d%02d%02d%02d.log",(ts->tm_year+1900), ts->tm_mon+1, ts->tm_mday,ts->tm_hour,ts->tm_min);
			sprintf( backup_filename, "%s%s", filename1,buf) ;
            
			unlink(backup_filename);
            rename( trace_filename, backup_filename);			
			runfp = fopen( trace_filename, "wt" );			
       }
       if( flushflag )
			fflush(runfp);
	}

	va_end(marker);
	theLock.Unlock();   
		
	return 1;
}

int TPrintLog::printb(char *msg, int len)
{
	if(logvalid == 0 || runfp == 0) return 0;

	char buffer[4000]="", tmp[10];

	for( int i=0; i<len&&i<1024; i++ ) {
		sprintf(tmp, "02x ", (unsigned char)(msg[i]));
		strcat( buffer, tmp);
	}
	strcat( buffer, "\n"  );
	return printf(buffer);
}

int TPrintLog::perror( char *fmt, ...)
{
	if(logvalid == 0) return 0;

    theLock.Lock();
    FILE *fp=fopen(error_filename, "at+");
    if( fp ) 
	{  		
		time_t now=time(0);
        struct tm *ts = localtime(&now);
        if (ts) 
			fprintf( fp, "%04d/%02d/%02d %02d:%02d:%02d ", 
					ts->tm_year+1900, ts->tm_mon+1, ts->tm_mday, ts->tm_hour, ts->tm_min, ts->tm_sec ); 
    	va_list argptr;
    	va_start(argptr, fmt);
    	vfprintf( fp, fmt, argptr ) ;
    	va_end(argptr);
		fclose(fp);
    }
    theLock.Unlock();

	return 0 ;
}

void TPrintLog::setTraceMode(int mode)
{
	if(logvalid == 0) return;

	theLock.Lock();

	if( trace_mode == LOG_TO_FILE && runfp)	
		fprintf( runfp, "SET TRACE MODE(%d)\n", mode ) ;

	trace_mode = mode;	
	if( mode == LOG_TO_NULL) 
	{
		if( runfp )
		{
			fprintf( runfp, "TRACE OFF\n" ) ;
			fclose(runfp);
			runfp = NULL;
		}
	}
	else if( mode == LOG_TO_FILE )
	{
		if( !runfp ) 
			runfp=fopen(trace_filename,"at+");	
		if( runfp )
		{
			fprintf( runfp, "TRACE ON\n" ) ;
			fflush(runfp);
		}
	}

	theLock.Unlock();
}
