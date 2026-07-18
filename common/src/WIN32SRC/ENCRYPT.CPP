////////////////////////////////////////////////////////////////////////////////
/*      The Simple Encryption   can be reversible for password encypt         */
////////////////////////////////////////////////////////////////////////////////


#include <string.h>

// map A- Z ( 0 - 25) a-z ( 26 - 51 ) 0 - 9 (52-61)

int   map(char c)
{
	int  temp ;
	if( c >= 'A'  && c <= 'Z' )
		temp = c  - 'A' ;
	else if ( c >= 'a' && c <= 'z' )
		temp =  c - 'a' +26;
	else if ( c >= '0' && c<= '9' )
		temp = c - '0' +52 ;
	else
		temp = 127 + c;
	return temp;
}

char unmap ( int num )
{
	char temp ;
	if( num < 0 ) {
		while ( num < 0 ) 
			num = num + 62 ;
	}
	if( num >=  0 && num <=25 )
		temp = num  + 'A' ;
	else if ( num >=26 && num <= 51 )
		temp = num -26 + 'a' ;
	else if (num >= 52 && num <= 61 )
		temp =  num - 52 + '0';
	else
		temp = num -127;
	return temp;
}

void encrypt ( char *password )
{
	if(password[0] == '\0' )
		return ;

	int pwdLen = strlen(password);

	int *mapMatrix = new int[pwdLen];

	for( int i = 0 ;  i < pwdLen ; i++ )
	{
		mapMatrix[i] =  map(password[i]);
		if( mapMatrix[i] < 62  )
			mapMatrix[i] = ( mapMatrix[i]  + 17 * (i+1) + pwdLen*2  ) % 62;
		password[i] = unmap( mapMatrix[i] );
	}
	delete [] mapMatrix;
	return ;
}

void  decrypt (char *password  )
{
	if(password[0] == '\0' )
		return ;

	int pwdLen = strlen(password);

	int *mapMatrix = new int[pwdLen];
	for( int i = 0 ;  i < pwdLen ; i++ )
	{
		mapMatrix[i] =  map(password[i]);
		if( mapMatrix[i] < 62  )
			mapMatrix[i] = mapMatrix[i] - 17 * (i+1) -pwdLen*2;
		password[i] = unmap( mapMatrix[i] );
	}
	delete [] mapMatrix ;
	return ;
}

