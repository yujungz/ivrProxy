#ifndef __UDP_H__
#define __UDP_H__

#include <iostream.h>
#include <windows.h>
#include <winsock.h>

#include "cttypes.h"
#include "ctkvset.h"
#include "ctpacket.h"
#include "KernelObject.h"

#include "Queue.h"

extern char * getmyipaddr( ) ;
const int constTRANSFRAMEHEADSIZE = sizeof(sockaddr_in)*2 + sizeof(CTbyte) + sizeof(CTuint)*2 + sizeof(CTushort);
const int constMAXTRANSFRAMELEN = constTRANSFRAMEHEADSIZE + constMAXPACKETLEN ;

struct PEER
{
	sockaddr_in addr ;

	PEER( int family = AF_INET ) ;
	~PEER( ) ;

	PEER( const char * addr, int port ) ;
	PEER( const sockaddr_in & saddr ) ;
	PEER( const PEER & ) ;
	PEER & operator = ( const PEER & ) ;
	int operator == ( const PEER & ) const ;
	int operator != ( const PEER & ) const ;

friend ostream & operator << ( ostream & o, PEER & p ) ;

} ;

struct TRANSFRAME
{
	sockaddr_in   SourAddress ;
	sockaddr_in   DestAddress ;
	CTbyte        Flags ;
	CTuint        Seq ;
	CTuint        Ack ;
	CTushort      Window ;
	CTbyte        Occupy;
	PACKETDATA    Data;

	TRANSFRAME( )
		: Flags( 0 ), Seq( 0 ), Ack( 0 ), Window( 0 )
	{
		memset( (char *)&SourAddress, 0, sizeof(sockaddr_in) ) ;
		memset( (char *)&DestAddress, 0, sizeof(sockaddr_in) ) ;
		Occupy = 0;
	}
	TRANSFRAME( sockaddr_in src, sockaddr_in dest, CTbyte type, CTuint seqno, CTuint ackno,
				CTushort winno, const PACKETDATA * packet ) ;

	TRANSFRAME ( TRANSFRAME & frame ) ;
	~TRANSFRAME( ){}

	TRANSFRAME & operator = ( const TRANSFRAME & frame ) ;

	int toString( char * str, int maxlen ) ;
	CTerror fromString( const char * str, int len ) ;

} ;

typedef TRANSFRAME TUDPFRAME ;
typedef TQueue<PACKETDATA> TPACKETQUEUE;//(UDP_RECVQUEUESIZE,0) ;
typedef TQueue<TUDPFRAME> TFRAMEQUEUE;
class TUDPSocket
{
	private:
		SOCKET socketID ;
		PEER  localAddr ;
		int   errNo;
		char  errMsg[128];

	public :
		TUDPSocket( int port, const char * addr = getmyipaddr() ) ;
		~TUDPSocket( ) ;

		TUDPSocket( const TUDPSocket & ) ;
		TUDPSocket & operator = ( const TUDPSocket & ) ;
		int operator == ( const TUDPSocket & ) ;

		int RecvMsg( void * data, int size ) ;
		int SendMsg( const sockaddr_in * remote, const void * data, int size ) ;

		int GetLastError( ) const { return ::WSAGetLastError( ) ; }
		int isValid( ) const { return socketID != INVALID_SOCKET ? 1 : 0 ; }
		SOCKET GetSocket( ) const { return socketID ; }

		int  GetErrNo(){ return errNo;}
		char *GetErrMsg();

		//return 0 : open success
		//return -1 : open error 
		int    Open();
		void   Close();
} ;

class TUDPSendSocket : public TUDPSocket
{
public:
		TUDPSendSocket( int aPort) ;
		~TUDPSendSocket( ) ;
		int SendFrame( TUDPFRAME & f) ;
//			{ theSendFrameQueue.Put( f ); return 0; }

} ;

class TUDPRecvSocket : public TUDPSocket
{
public:
		TUDPRecvSocket( int aPort) ;
		~TUDPRecvSocket( ) ;

		CTbool Run(void) ;
		void Stop() ;
		void doRecv();

		void RecvFrame( TUDPFRAME & ) ;
		void putEvent(TUDPFRAME& f){ theRecvFrameQueue.Put(f); }		

		void	 AddRef() { m_cRef++ ;}
		void	 DelRef() { m_cRef -- ;}
		
friend DWORD WINAPI Do_RecvFrame( LPVOID ) ;

private:
		CTThread *theRECVThread;
		TFRAMEQUEUE theRecvFrameQueue;
		int  sysdown ;
		int  m_cRef ;
} ;
#endif //__UDP_H__

