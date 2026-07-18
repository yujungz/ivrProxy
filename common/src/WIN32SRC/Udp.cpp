#include <iostream.h>
#include <string.h>
#include <errno.h>

#include "ctutil.h"
#include "Udp.h"


#define NOERROR 0

const int NETSENDQUEUESIZE = 128*2 ;
const int NETRECVQUEUESIZE = 128*2 ;

PEER::PEER( int family )
{
	memset( &addr, 0, sizeof(sockaddr_in) ) ;
	addr.sin_family = family ;
}

PEER::PEER( const sockaddr_in & saddr )
{
	memmove( &addr, &saddr, sizeof(sockaddr_in) ) ;
	addr.sin_family	= AF_INET ;
}

PEER::PEER( const PEER & p )
{
	memmove( &addr, &p.addr, sizeof(sockaddr_in) ) ;
}

PEER & PEER::operator = ( const PEER & p )
{
	memmove( &addr, &p.addr, sizeof(sockaddr_in) ) ;
	return * this ;
}

int PEER::operator == ( const PEER & p ) const
{
	if( addr.sin_addr.s_addr == p.addr.sin_addr.s_addr &&
		addr.sin_port == p.addr.sin_port )
		return 1 ;
	return 0;
}

int PEER::operator != ( const PEER & p ) const
{
	if( addr.sin_addr.s_addr == p.addr.sin_addr.s_addr &&
		addr.sin_port == p.addr.sin_port )
		return 0 ;
	return 1 ;
}

PEER::PEER(const char *address, int port)
{
	if( address )
	{
		memset( &addr, 0, sizeof(addr) ) ;
		addr.sin_family	= AF_INET ;
		addr.sin_port	= htons(port) ;
		addr.sin_addr.s_addr = inet_addr((char*)address) ;
	}
}

PEER::~PEER( )
{
}

// New friend
ostream & operator << ( ostream & o, PEER & p )
{
	o << inet_ntoa( p.addr.sin_addr ) << ',' << dec << p.addr.sin_port ;
	return o ;
}

///////////////////////////////////////////////////////////////////////////////
TRANSFRAME::TRANSFRAME( sockaddr_in src, sockaddr_in dest,
					   CTbyte type, CTuint seqno, CTuint ackno, CTushort winno,				
					   const PACKETDATA * packet )
	: Flags( type ),
	Seq( seqno ),
	Ack( ackno ),
	Window( winno )
{
	memcpy( (void *)&SourAddress, (void *)&src, sizeof(sockaddr_in) ) ;
	memcpy( (void *)&DestAddress, (void *)&dest, sizeof(sockaddr_in) ) ;
	if( packet )
	{
		Data = *packet;
		Occupy = 1;
	}
	else 
		Occupy = 0;
}

TRANSFRAME::TRANSFRAME( TRANSFRAME & frame )
{
	*this = frame;
}

TRANSFRAME & TRANSFRAME::operator = ( const TRANSFRAME & frame )
{
	memcpy( (void *)&SourAddress, (void *)&frame.SourAddress, sizeof(sockaddr_in) ) ;
	memcpy( (void *)&DestAddress, (void *)&frame.DestAddress, sizeof(sockaddr_in) ) ;
	Flags = frame.Flags;
	Seq = frame.Seq;
	Ack = frame.Ack;
	Window = frame.Window;
	Occupy = frame.Occupy;
	Data = frame.Data;
	return * this ;
}

int TRANSFRAME::toString( char * str, int maxlen )
{
	int len = 0 ;

	if( maxlen < constTRANSFRAMEHEADSIZE )
		return 0 ;

	char * p = str ;
	memcpy( p, (void *)&SourAddress, sizeof(sockaddr_in) ) ;
	p += sizeof(sockaddr_in) ;
	memcpy( p, (void *)&DestAddress, sizeof(sockaddr_in) ) ;
	p += sizeof(sockaddr_in) ;

	*p ++ = Flags ;

	CTuint2CTbyte( (CTuint)Seq, p ) ;
	p += sizeof(CTuint) ;
	CTuint2CTbyte( (CTuint)Ack, p ) ;
	p += sizeof(CTuint) ;

	CTshort2CTbyte( (CTshort)Window, p ) ;
	p += sizeof(CTshort) ;

	if(Occupy)
	{
		len = Data.toString(str+constTRANSFRAMEHEADSIZE,maxlen-constTRANSFRAMEHEADSIZE);
		if(len<0)
			return -1;
	}
	return len + (constTRANSFRAMEHEADSIZE) ;
}

CTerror TRANSFRAME::fromString( const char * str, int len )
{
	int retval = CT_errorOK ;

	if( len < constTRANSFRAMEHEADSIZE )
		return CT_errorFAIL ;

	char *p = (char *)str ;
	memcpy( (void*)&SourAddress, p, sizeof(sockaddr_in) ) ;
	p += sizeof(sockaddr_in) ;
	memcpy( (void*)&DestAddress, p, sizeof(sockaddr_in) ) ;
	p += sizeof(sockaddr_in) ;
	Flags = *p ++ ;
	CTbyte2CTuint( p, (CTuint*)&Seq ) ;
	p += sizeof(CTuint) ;
	CTbyte2CTuint( p, (CTuint*)&Ack ) ;
	p += sizeof(CTuint) ;
	CTbyte2CTshort( p, (CTshort*)&Window ) ;
	p += sizeof(CTshort) ;

	if (len>constTRANSFRAMEHEADSIZE)
	{
		Occupy = 1;
		retval = Data.fromString(p,len-constTRANSFRAMEHEADSIZE);
	}
	else 
		Occupy = 0;

	return retval ;
}


//
//TUDPSocket functions:
//
TUDPSocket::TUDPSocket( int port, const char * addr )
	: localAddr( addr, port )
{
	errNo = 0 ;
	memset( errMsg, 0, 128);

	socketID = ::socket( AF_INET, SOCK_DGRAM, 0 ) ;
	assert( INVALID_SOCKET!=socketID ) ;

	int recvbuff = 20*1024 ;
	int sendbuff = 10*1024 ;
	int gate =10;

	setsockopt( socketID, SOL_SOCKET, SO_RCVBUF, (char *)&recvbuff, sizeof(recvbuff) ) ;
	setsockopt( socketID, SOL_SOCKET, SO_SNDBUF, (char *)&sendbuff, sizeof(sendbuff) ) ;
	//setsockopt( socketID, SOL_SOCKET,SO_SNDLOWAT,(char *)&gate,sizeof(int));
	setsockopt( socketID, SOL_SOCKET,SO_RCVLOWAT,(char *)&gate,sizeof(int));
	//BOOL bOption = TRUE ;//
	BOOL bOption = FALSE ;//
	if( SOCKET_ERROR == setsockopt( socketID,SOL_SOCKET,SO_REUSEADDR,(char *)&bOption, sizeof(BOOL) ))
		errNo = WSAGetLastError();
	bOption = TRUE ;
	if ( SOCKET_ERROR==setsockopt( socketID,SOL_SOCKET,SO_BROADCAST,(char *)&bOption,sizeof(BOOL) ) )
		errNo = WSAGetLastError();

	ioctlsocket( socketID, FIONBIO, 0 ) ;
}


int  TUDPSocket::Open()
{
	sockaddr_in address ;

	memset( &address , 0 , sizeof(sockaddr_in) ) ;
	address.sin_family      = AF_INET;
	address.sin_port        = localAddr.addr.sin_port ;
	address.sin_addr.s_addr	= INADDR_ANY ;
	if( ::bind (socketID, (struct sockaddr *)&address, sizeof(sockaddr)) < 0 )
	{
		errNo = WSAGetLastError();
		perror("[UDP BIND] error---") ;
		Close( ) ;	
		return -1;
	}

	return 0;
}

void TUDPSocket::Close( )
{
	if( socketID != INVALID_SOCKET )
	{
		closesocket( socketID ) ;
		socketID = INVALID_SOCKET ;
	}
}

TUDPSocket::~TUDPSocket( )
{
	Close( ) ;
}

TUDPSocket::TUDPSocket( const TUDPSocket & s )
{
	* this = s ;
}

TUDPSocket & TUDPSocket::operator = ( const TUDPSocket & s )
{
	socketID = s.socketID ;

	localAddr = s.localAddr ;
	return * this ;
}

int TUDPSocket::operator == ( const TUDPSocket & s )
{
	return socketID==s.socketID ? localAddr==s.localAddr : 0 ;
}

int TUDPSocket::RecvMsg( void * data, int maxsize )
{
	if( !isValid() )
	{
		return 0 ;//no data receive
	}

	sockaddr remote ;
	int sz = sizeof(sockaddr) ;
	int len = recvfrom( socketID, (char *)data, maxsize,0,&remote, &sz ) ;
	if ( SOCKET_ERROR == len )
	{
		errNo = WSAGetLastError();
//	 	perror( "[UDP RecvMsg] error---" ) ;
		//Close( ) ;
		return SOCKET_ERROR  ;
	}
	return len ;
}

int TUDPSocket::SendMsg( const sockaddr_in * remote, const void * data, int size )
{	
	if( !isValid() )
		return SOCKET_ERROR  ;

	if( !remote  || size <= 0 ) 
		return 0 ;

	int len = sendto( socketID, (char*)data, size, 0, (struct sockaddr*)remote, sizeof(sockaddr_in) ) ;
	if ( SOCKET_ERROR == len )
	{
		errNo = WSAGetLastError();
//		perror("[UDP SEND] error---") ;
		//Close ();
		return SOCKET_ERROR ;
	}
	return len ;
}

char *TUDPSocket::GetErrMsg()
{
	switch ( errNo )
	{
		case WSANOTINITIALISED:
			strcpy( errMsg,  "A successful WSAStartup must occur before using this function." );
			break ;
		case WSAENETDOWN:
			strcpy( errMsg, "The network subsystem has failed." ) ;
			break ;
		case WSAEFAULT:
			strcpy(errMsg, "optval is not in a valid part of the process address space or optlen parameter is too small." );
			break ;
		case WSAEINPROGRESS:
			strcpy(errMsg, "A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function." );
			break ;
		case WSAEINVAL:
			strcpy( errMsg, "level is not valid, or the information in optval is not valid." );
			break ;
		case WSAENETRESET:
			strcpy(errMsg, "Connection has timed out when SO_KEEPALIVE is set." ) ;
			break ;
		case WSAENOPROTOOPT:
			strcpy( errMsg, "The option is unknown or unsupported for the specified provider." ) ;
			break ;
		case WSAENOTCONN:
			strcpy( errMsg,  "Connection has been reset when SO_KEEPALIVE is set." ) ;
			break ;
		case WSAENOTSOCK:
			strcpy(errMsg, "The descriptor is not a socket.");
			break ;

		default :
			strcpy( errMsg, "no error" );
			break;
	}

	return errMsg;
}

////////////////////////////////////////////////////////////

TUDPSendSocket::TUDPSendSocket( int UDPPort)
	:TUDPSocket( UDPPort )
{
	#if __UDPSocket__
	cout << "[UDPSendSocket] At Port "<< UDPPort << " With Queue(" << NETSENDQUEUESIZE << ")" << endl ;
	#endif
}

TUDPSendSocket::~TUDPSendSocket( )
{
}


int TUDPSendSocket::SendFrame( TUDPFRAME & frame )
{
	char buffer[constMAXTRANSFRAMELEN] ;
	int len = frame.toString( buffer, constMAXTRANSFRAMELEN ) ;
	if( len>0 )
	{
		#if __UDPSocket__
		cout <<"[UDPSendSocket] SendMsg()" << endl ;
		#endif
		SendMsg( &frame.DestAddress, buffer, len ) ;
	}
	return 0 ;
}


DWORD WINAPI Do_RecvFrame( LPVOID arg )
{
	assert( arg );
	((TUDPRecvSocket *)arg)->doRecv( ) ;
	return 0 ;
}

TUDPRecvSocket::TUDPRecvSocket( int UDPPort)
	: TUDPSocket( UDPPort ),theRecvFrameQueue(1024),
	sysdown(0),m_cRef(0)

{
	#if __UDPSocket__
	cout << "[UDPRecvSocket] At Port "<< UDPPort << " With Queue(" << NETRECVQUEUESIZE << ")" << endl ;
	#endif


	theRECVThread  = new CTThread( Do_RecvFrame, this, CREATE_SUSPENDED );
	assert( theRECVThread  );		
}

TUDPRecvSocket::~TUDPRecvSocket( )
{
	if ( theRECVThread  ) 
	{
		sysdown = 1 ;
		if( m_cRef > 0 ) 
		{
			time_t now=time(0);
			while( time(0) - now < 10 && m_cRef > 0 ) 
			{
				Sleep(200);
			}
		}
		delete theRECVThread ;
		theRECVThread = NULL;
	}
}

CTbool TUDPRecvSocket::Run(void) 
{
	if( 0xFFFFFFFF==theRECVThread->Continue() )
		return CT_boolFALSE;
	return CT_boolTRUE;
}

void TUDPRecvSocket::Stop() 
{
	sysdown = 1 ;
}

void TUDPRecvSocket::doRecv()
{
	char buffer[constMAXTRANSFRAMELEN] ;
	TUDPFRAME frame;

	#if __UDPSocket__
	cout <<"[UDPRecvSocket] Working..." << endl ;
	#endif

	AddRef();
	while(1)
	{
		int len = RecvMsg( buffer, constMAXTRANSFRAMELEN ) ;
		if( len>0 )
		{
			#if __UDPSocket__
			cout <<"[UDPRecvSocket] RecvMsg()" << endl ;
			#endif
			frame.fromString( buffer, len) ;
            if(frame.Data.tranInfo.version == constTHISVERSION)
				theRecvFrameQueue.Put(frame);
		}
		if( sysdown )
			break ;
	}
	DelRef();
}

void TUDPRecvSocket::RecvFrame( TUDPFRAME & frame )
{
	theRecvFrameQueue.Get(frame);
}

