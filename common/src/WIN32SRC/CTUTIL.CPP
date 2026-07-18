#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#ifndef WIN32
#include <stdlib.h>
#include <unistd.h>
#else
#include <winsock.h>
#include <io.h>
#endif

#include "ctutil.h"

char * newStr(const char *pStr)
{
	if(!pStr)
		return NULL;
	int n = strlen(pStr);
	char *s = new char[n+1];
	strncpy(s, pStr, n);
	s[n] = '\0';
	return s;
}

void  CTshort2CTbyte( CTshort Number, char *szString )
{
   if( !szString ) 
	return ;

   int len=sizeof(CTshort);
   CTshort_byte val( Number ) ;
#ifdef __i386
   for( int i=0; i<len ; i++ ) 
     szString[i] = val.str[ len-i-1 ] ;
#else
   for( int i=0; i<len ; i++ ) 
     szString[i] = val.str[i] ;
#endif

}

CTshort  CTbyte2CTshort(  const char *szString, CTshort *number )
{
  if( !szString ) 
	return 0 ;

  int len=sizeof(CTshort);
  CTshort value ;

#ifdef __i386
  char str[8] ;
  for( int i=0; i<len; i++ ) 
     str[i] = szString[ len-i-1 ] ;
  CTshort_byte val( str ) ;
  value = val.value ;
#else
  CTshort_byte val( (char *)szString ) ;
  value = val.value ;
#endif
  
  if( number ) 
     *number = value ;

  return value ;
}

void  CTushort2CTbyte( CTushort Number, char *szString )
{
   if( !szString ) 
	return ;

   int len=sizeof(CTushort);
   CTushort_byte val( Number ) ;
#ifdef __i386
   for( int i=0; i<len ; i++ ) 
     szString[i] = val.str[ len-i-1 ] ;
#else
   for( int i=0; i<len ; i++ ) 
     szString[i] = val.str[i] ;
#endif

}

CTushort  CTbyte2CTushort(  const char *szString, CTushort *number )
{
  if( !szString ) 
	return 0 ;

  int len=sizeof(CTushort);
  CTushort value ;

#ifdef __i386
  char str[8] ;
  for( int i=0; i<len; i++ ) 
     str[i] = szString[ len-i-1 ] ;
  CTushort_byte val( str ) ;
  value = val.value ;
#else
  CTushort_byte val( (char *)szString ) ;
  value = val.value ;
#endif
  
  if( number ) 
     *number = value ;

  return value ;
}

void  CTuint2CTbyte( CTuint Size, char *szString )
{
  int len = sizeof( CTuint ) ;
  if( !szString ) 
     return ;
 
  CTuint_byte val( Size ) ;
#ifdef __i386
  for( int i=0; i<len; i++ ) 
         szString[i] = val.str[len-i-1] ;
#else
  for( int i=0; i<len; i++ ) 
         szString[i] = val.str[i] ;
#endif
}


/*
CTuint  CTbyte2CTuint(  const char *szString, CTuint *Size )
{
  char str2[1024];
  char str1[4] ;

  memcpy( str1, szString, 4 ) ;
printf( "str[0] = %x %x %x %x\n", str1[0], str1[1], str1[2], str1[3] ) ;
printf( "enter byte2uint string=%p\n", szString ) ;
   CTuint m= *( (CTuint *)str1) ;
printf( "m=%d\n" , m) ;
   CTuint n= *( (CTuint *)szString ) ;
printf( "n=%d11\n", n ) ;
#ifdef __i386
   str[0] = szString[3] ;
   str[1] = szString[2] ;
   str[2] = szString[1] ;
   str[3] = szString[0] ;
   *Size = *( (CTuint *)str ) ;
#else
   *Size = *( (CTuint *)str1); 
#endif

   printf( "CTbyte2CTuint str[0]=%x %x %x %x value=%d\n", *Size, szString[0], szString[1], szString[2], szString[3] ) ; 
   return *Size;
}
*/ 

CTuint  CTbyte2CTuint(  const char *szString, CTuint *Size )
{
  if( !szString ) 
     return 0 ;
  int len=sizeof(CTuint) ;
  CTuint value ;

#ifdef __i386
   char str[8] ;
   for( int i=0; i<len; i++ ) 
      str[i] = szString[ len-i-1 ] ;

   CTuint_byte val( str ) ;
   value = val.value ;
#else
   CTuint_byte val( (char *)szString ) ;
   value = val.value ;
#endif

   if( Size ) 
      *Size = value ;

   return value;
}

void  CTfloat2CTbyte( CTfloat Size, char *szString )
{
  if( !szString ) 
     return ;

  int len=sizeof( CTfloat ) ;

  CTfloat_byte val( Size ) ;
#ifdef __i386
  for( int i=0; i<len ; i++ ) 
     szString[i] = val.str[len-i-1] ;
#else
  for( int i=0; i<len; i++ ) 
     szString[i] = val.str[i] ;
#endif
}


CTfloat  CTbyte2CTfloat( const char *szString, CTfloat *Size )
{
  if( !szString ) 
      return 0 ;
  int len = sizeof( CTfloat ) ;
  CTfloat value ;

#ifdef __i386
   char str[8] ;
   for( int i=0; i<len; i++ ) 
      str[i] = szString[len-i-1] ;
   CTfloat_byte val( str ) ;
   value = val.value ;
#else
   CTfloat_byte val( (char *)szString ) ;
   value = val.value ;
#endif
  
   if( Size ) 
     *Size = value ;

   return value ;
}

unsigned int uint_min(unsigned int a, unsigned int b)
{
	return (a<b ? a : b);
}


unsigned int uint_max(unsigned int a, unsigned int b)
{
	return ( a>b ? a : b);
}

int GetFilePos(int fd)
{
#ifdef WIN32
	return _lseek(fd, 0, SEEK_CUR);
#else
	return lseek(fd, 0, SEEK_CUR);
#endif
}

#ifndef WIN32
int     min(int a, int b)
{
	return (a<b ? a: b);
}

int     max(int a, int b)
{
	return ( a>b ? a : b);
}

CTimer::CTimer()
{
	time_val.tv_sec = 0;
	time_val.tv_usec = 0;
}

CTimer::CTimer(long sec,long usec)
{
	time_val.tv_sec = sec;
	time_val.tv_usec = usec;
}

CTimer::~CTimer()
{
}

void CTimer::SetTimer(long sec,long usec)
{
	time_val.tv_sec = sec;
	time_val.tv_usec = usec;
}

int CTimer::Wait()
{
	return (select (0, (fd_set *)0, (fd_set *) 0,
			(fd_set*)0, &time_val) );
}
#endif

#ifdef WIN32
char * getmyipaddr(void)
{
	const int constBUFFERSIZE = 256;
	static char hostname[constBUFFERSIZE] = "" ;

	if( !(*hostname) && !gethostname( hostname, constBUFFERSIZE ) )
	{
		struct hostent * hp ;
		hp = gethostbyname( hostname ) ;
		if ( hp )
		{
			struct in_addr addr ;
			memcpy( (void*)&addr.s_addr, (void*)(*(hp->h_addr_list)),
					sizeof(addr.s_addr) ) ;
			strcpy( hostname, inet_ntoa(addr) ) ;
		}
	}
	return hostname ;
}
#else // WIN32

#ifndef _UNUSE_GETMYIPADDR_   //Qingxiao, 1998-09-30, for sometimes I don't want to link "-lnsl" and "-lsocket"
char * getmyipaddr(void)
{
 static char hostname[256] = "";

 if( !*hostname && sysinfo(SI_HOSTNAME, hostname, 256)>0){
 struct hostent *hp;
 hp = gethostbyname(hostname);
 if (hp) {
                        struct in_addr addr;
                        memcpy((void*)&addr.s_addr,
                                (void*)(*(hp->h_addr_list)),
                                sizeof(addr.s_addr) );
                        strcpy( hostname, inet_ntoa(addr) );
                }
        }
        return hostname;
}
#endif
#endif // WIN32

#ifndef _UNUSE_TCPIP_

//Qingxiao, 1998-09-30, for UNIXWARE have not nanosleep function
#ifdef _UNIXWARE
int __nanosleep(struct timespec *rqtp, struct timespec* )
{
	struct timeval time_val;

	time_val.tv_sec = rqtp->tv_sec;
	time_val.tv_usec = rqtp->tv_nsec/1000;

	return (select (0, (fd_set *)0, (fd_set *) 0,
			(fd_set*)0, &time_val) );
}
#endif

void MYTRACE(char *format,...)
{	
	return ;
	va_list marker;
	
	va_start(marker,format);    
	    
	FILE *fp=fopen( "mtrace.log", "at");
	if( fp )
	{
		fprintf(fp, "T%d) ",::GetTickCount() );
		vfprintf(fp, format, marker ) ;
		fclose(fp);
	}

	va_end(marker);
}


#endif
