#ifndef __UDPSERVER_H_
#define __UDPSERVER_H_

#include <iostream.h>
#include <windows.h>
#include <winsock.h>

#include "TCB.h"
#include "Queue.h"
#include "Udp.h"
#include "Timer.h"

//
//TUDPServer defination
//
const int UDP_RECVQUEUESIZE = 256 ;
#define BROADCASTADDRESS      "255.255.255.255"

extern char * getmyipaddr(void) ;

class TTCB ;
class CTUDPTimer;
class TTCBLink
{
public :
	TTCBLink() ;
	~TTCBLink() ;

	TTCB * findTCB( const sockaddr_in& addr ) ;
	TTCB * findTCB( unsigned long id ) ;
	int    insertTCB( TTCB * tcbp ) ;
	int    getCount( ) { return TCBsum ; }
	//void   printLog( ) ;
	void   TCBInfo(char *buffer, int maxsize);
	void   TraceInfo(char *buffer, int maxsize);

private :
	TTCB * tcbfront ;
	TTCB * tcbrear ;
	TTCB * tcbcurr ;
	int    TCBsum ;
};

class TUDPServer
{
public:	
	TUDPServer( const char *ip, int aPort, CTAppQueue *queue,int start = 0 ) ;
	~TUDPServer( ) ;

	PEER & getAddress( )
		{ return localAddr ; }
	TUDPSendSocket & getSendSocket( )
		{ return theNSQ ; }

	int SendPacket( const PEER & dest, const PACKETDATA & data )
		{ return doSendPacket( dest, data ) ; }
//	int RecvPacket( PACKETDATA & data )
//		{ recvQueue.Get( data ) ; return 1; }
/*
	CTbool SendToSelf( PACKETDATA & data )
		{ recvQueue.Put( data ) ; return CT_boolTRUE ; }
		{ recvQueue->Put(GETMODULE(data.receiver), data ) ; return CT_boolTRUE ; }
	CTbool SendTimerFrame( TUDPFRAME& frame );
*/

	inline void Broadcast( int port, const PACKETDATA & data ) ;
	int  PostPacket( const PEER & dest, const PACKETDATA & data );

	void TUDPServer::SendDirect( const PEER & dest, const PACKETDATA & data )
		{ TRANSFRAME frame( localAddr.addr, dest.addr, 0, 0, 0, 0, &data ) ; theNSQ.SendFrame( frame ) ; }

	CTbool Run( ) ;//must called the function to active the thread!!

	void Stop();
	//void printLog( ) ;
	void  TCBInfo( char *buffer, int maxsize);

	int   StartTrace( CTuint tcbid) ;
	int   StopTrace( CTuint tcbid) ;
	void  TraceInfo(char *buffer, int maxsize) ;

friend DWORD WINAPI Do_RecvFromNQ( LPVOID ) ;

private:
	void *         PoolWQ( ) ;
	int            doSendPacket( const PEER& dest, const PACKETDATA & data ) ;
//	inline CTbool  Init( ) ;
	inline CTbool  Cleanup( ) ;

	void	 AddRef() { m_cRef++ ;}
	void	 DelRef() { m_cRef -- ;}

private:
	//int            invalidSocket ;
	TTCBLink       theTCBLink ;
	CTThread       *theRecThread ;//pool theNetWQ, put packet to recvQueue!

//	CTSemaphore     theBigLock ;
	CTCriticalSection  theBigLock ;
	
	CTAppQueue     *theAppQueue;

	TUDPSendSocket theNSQ ;
	TUDPRecvSocket theNRQ ;

	CTUDPTimer * MainTimer ;

	PEER           localAddr ;
	int			   sysdown;
	int			   m_cRef;
} ;

#endif // __UDPSERVER_H_
