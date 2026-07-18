#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <iostream.h>

#ifndef WIN32
#include <stdlib.h>
#include <unistd.h>
#ifndef _UNIXWARE
#include <sys/varargs.h>
#else
#include <stdarg.h>
#endif
#else
#include <winsock.h>
#include <io.h>
#endif

#include "pig.h"

void PigSleep(unsigned long milSec)
{
#ifdef WIN32
    ::Sleep(milSec);
#else
    struct timeval time_val;
    time_val.tv_sec = milSec / 1000 ;
    time_val.tv_usec = (milSec % 1000) * 1000000;
    select (0, (fd_set *)0, (fd_set *) 0,(fd_set*)0, &time_val) ;
#endif
}

int  PigIsdigit(char ch)
{
#ifdef WIN32
    return ::isdigit(ch);
#else
    if((ch<0x0030)||(ch>0x0039))
	return 0;  // false
    else
	return 1;  // true
#endif
}

char* PigItoa(int value, char *string, int radix )
{
#ifdef WIN32
	return _itoa(value,string,radix);
#else
	if(radix==10)
	{
		sprintf(string,"%d",value);
	}
	else if(radix==16)
	{
		sprintf(string,"%x",value);
	}
	return NULL;
#endif
}


int   PigMakeLower(char* s)
{
	while(*s)
	{
		if(*s>=0x41&&*s<=0x5a)
				*s=*s+0x20;
		s++;
	}
	return 1;
}
int   PigStrAlNum(char* s)
{
	char *p=s,*ps;
	int ok=0;
	while(*p)
	{
		if(*p>=0x21&&*p<=0x7e)
		{
			ok=1;
			break;
		}
		p++;
	}
	if(!ok)
		return 0;
	ps=p;
	while(*p)
	{
		if(*p<0x21||*p>0x7e)
		{
			*p='\0';
			strcpy(s,ps);
			return 1;
		}
		p++;
	}
	return 1;
}