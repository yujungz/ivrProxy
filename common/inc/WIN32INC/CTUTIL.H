#ifndef __UTIL_H__
#define __UTIL_H__

#ifndef WIN32
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

#include "cttypes.h"

union CTshort_byte {
  CTshort value ;
  char    str[2] ;
  CTshort_byte( CTshort val) { value = val; }
  CTshort_byte( char *string) 
	{	
	 if( string ) 
  		memcpy( (void *)str, ( void *)string, sizeof(CTshort) ) ;
	}
};

union CTushort_byte {
  CTushort value ;
  char    str[2] ;
  CTushort_byte( CTushort val) { value = val; }
  CTushort_byte( char *string) 
	{	
	 if( string ) 
  		memcpy( (void *)str, ( void *)string, sizeof(CTushort) ) ;
	}
};

union CTuint_byte {
  CTuint  value ;
  char    str[4];
  CTuint_byte( CTuint val) { value=val; } 
  CTuint_byte( char *string) 
	{
 	 if( string )
		memcpy( (void *)str, (void *)string, sizeof( CTuint) ) ;
	}
};

union CTfloat_byte {
  CTfloat value ;
  char    str[4] ;
  CTfloat_byte( CTfloat val) { value = val ; } 
  CTfloat_byte( char *string) 
	{
	if( string ) 
		memcpy( (void *)str, (void *)string, sizeof( CTfloat) ) ;
	}
};
 
char*   newStr(const char *pStr);
void    CTshort2CTbyte( CTshort number, char *szString );
CTshort CTbyte2CTshort( const char *szString, CTshort *number );
void    CTushort2CTbyte( CTushort number, char *szString );
CTushort CTbyte2CTushort( const char *szString, CTushort *number );
void    CTuint2CTbyte( CTuint number, char *szString );
CTuint  CTbyte2CTuint(  const char *szString, CTuint *number );
void    CTfloat2CTbyte( CTfloat Number, char *szString ) ;
CTfloat CTbyte2CTfloat( const char *szString, CTfloat *number ) ; 
unsigned int uint_min(unsigned int a, unsigned int b);
unsigned int uint_max(unsigned int a, unsigned int b);
int     GetFilePos(int fd);

#ifndef WIN32
int     min(int a, int b);
int     max(int a, int b);

class CTimer{
	private:
		struct timeval    time_val;

	public:
                CTimer();
                CTimer(long sec,long usec);
                ~CTimer();
		void SetTimer(long sec,long usec);
		int Wait();
};
#endif

char* getmyipaddr(void);

//Qingxiao, 1998-09-30, for UNIXWARE have not nanosleep function
#ifdef _UNIXWARE
#define nanosleep __nanosleep
int     __nanosleep(struct timespec *rqtp, struct timespec *rmtp); 
#endif

void MYTRACE(char *format,...);

#endif
