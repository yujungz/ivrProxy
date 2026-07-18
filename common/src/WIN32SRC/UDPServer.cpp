#include <iostream.h>
#include <fstream.h>

#include "ctgor.h"
#include "TCB.h"
#include "udp.h"
#include "UDPServer.h"
#include "Timer.h"


//
//TTCBLink operations:
//

TTCBLink::TTCBLink( )
	: TCBsum( 0 ),
	tcbfront( 0 ),
	tcbcurr( 0 ),
	tcbrear( 0 )
{
}

TTCBLink::~TTCBLink( )
{
	TTCB * p;
	while( tcbfront )
	{	
		p = tcbfront ;
		tcbfront = tcbfront->next ;
		delete p ;		
	}
	tcbfront =  tcbcurr = tcbrear = NULL ;
	
}

TTCB * TTCBLink::findTCB( const sockaddr_in & address )
{
	TTCB * p = tcbfront ;
	while( p )
	{
		if ( p->isMember(address) )
		{
			break;
		}
		p = p->next ;
	}
	return p ;
}

TTCB * TTCBLink::findTCB( CTuint id )
{
	TTCB * p = tcbfront ;
	while( p )
	{
		if ( p->isMember( id ) )
			break;
		p = p->next ;
	}
	return p ;
}

int TTCBLink::insertTCB( TTCB * tcbp )
{
	if( !tcbp )
		return -1 ;

	if( !tcbfront )
	{
		tcbfront = tcbp ;
		tcbrear  = tcbp ;
		tcbcurr  = tcbp ;
	}
	else
	{
		tcbrear->next = tcbp ;
		tcbrear = tcbp ;
	}
	TCBsum ++ ;
	return TCBsum;
}


/*
void TTCBLink::printLog()
{
	ofstream * fp = new ofstream( "udpsrv.log" ) ;

	TTCB * p = tcbfront ;
	while( fp && p )
	{
		//p->printLog( fp ) ;
		p->TCBInfo(fp);
		p = p->next ;
	}
	if( fp )
		delete fp ;
}
*/

void TTCBLink::TCBInfo(char *buffer, int maxsize)
{
	if( !buffer || maxsize <= 0 )
		return ;

	
	char info[1024]="";	
	buffer[0]='\0';
	TTCB * p = tcbfront ;
	while( p )
	{		
		p->TCBInfo(info);
		if( strlen(buffer)< maxsize-strlen(info))
		{
			strcat(buffer, info);
		}		

		p = p->next ;
	}
}

void TTCBLink::TraceInfo(char *buffer, int maxsize)
{
	if( !buffer || maxsize <= 0 )
		return ;	
	char info[1024]="";	

	buffer[0]='\0';

	TTCB * p = tcbfront ;
	while( p )
	{		
		if( p->isTrace() == CT_boolTRUE ) 
		{
			sprintf( info, "%d  " , p->getHandle() );
			strcat(buffer, info);
		}
		p = p->next ;
	}

}



//
//TUDPServer functions:
//
DWORD WINAPI Do_RecvFromNQ( LPVOID arg )
{
	assert( arg );
	((TUDPServer *)arg)->PoolWQ( ) ;
	return 0 ;
}

TUDPServer::TUDPServer( const char *aIP, int aPort, CTAppQueue *queue, int start )
	//: invalidSocket( Init() ),	
	:theNSQ( aPort+1 ),//-->the queue send to the net!
	theNRQ( aPort ),//-->the queue receive from the net!
	localAddr( aIP, aPort ),
	MainTimer( 0 ),sysdown(0), m_cRef(0),theRecThread(NULL),
	theAppQueue(queue)
{
	if( start )
	{
		Run( ) ;
	}
}

TUDPServer::~TUDPServer( )
{
	if(theRecThread )
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

		delete theRecThread ;
		theRecThread = NULL;
	}

	if( MainTimer )
	{
		delete MainTimer  ;
		MainTimer = NULL;
	}

	Cleanup( ) ;

}

inline CTbool TUDPServer::Cleanup( )
{
	return WSACleanup( ) ? CT_boolFALSE : CT_boolTRUE ;
}

int TUDPServer::doSendPacket( const PEER & dest, const PACKETDATA & data )
{
	int retval = -1 ;

	theBigLock.Lock();

	TTCB * tcb = theTCBLink.findTCB( dest.addr ) ;
	if( !tcb )
	{
		if( theTCBLink.getCount( ) <= MAX_TCBSUM )
		{
			//Not found and have enough resource, then new one!!!
			tcb = new TTCB( theAppQueue, MainTimer, &theNSQ, theTCBLink.getCount(), dest, localAddr ) ;
			assert( tcb ) ;
			theTCBLink.insertTCB( tcb ) ;
			tcb->Resume( ) ;			

			int  trycount=20;
			time_t now=time(0);
			while( time(0) - now < 1 && trycount )
			{
				if( tcb->getState() == stWork ) 
					break; 
				Sleep(1);
				trycount -- ;
			}
			
			retval = tcb->SendPacket( data ) ;
		}
		else
			cerr << "Too much TCB, packet lost!!" << endl ;
	}
	else
	{
		retval = tcb->SendPacket( data ) ;
	}

	theBigLock.Unlock();

	return retval ;
}

CTbool TUDPServer::Run( ) 
{
	if( theNSQ.Open() == -1 ||	theNRQ.Open() == -1  )
		return CT_boolFALSE;

	theRecThread = new CTThread( Do_RecvFromNQ, this, CREATE_SUSPENDED );
	
	if(theRecThread == NULL )
		return CT_boolFALSE;

	MainTimer = new CTUDPTimer( 100, &theNRQ);
	
	if(MainTimer == NULL )
		return CT_boolFALSE;

	if( 0xFFFFFFFF != theRecThread->Continue( ) &&
		MainTimer->Run( ) )
	{
		theNRQ.Run();
		return CT_boolTRUE ;
	}
	return CT_boolFALSE ;
}

void * TUDPServer::PoolWQ( )//theRecThread Main function!!
{
	//cout << "UDPServer thread initialize..." << endl;
	TUDPFRAME frame ;
	TTCB * workingTCB  = 0 ;

	AddRef() ;
	for( ; ; )
	{
		theNRQ.RecvFrame( frame ) ;
		if( sysdown )
			break ;
		if ( !frame.Flags&&frame.Occupy )
		{
			//recvQueue->Put( frame.Data) ;			
			theAppQueue->Put(GETMODULE(frame.Data.receiver),frame.Data);

			continue ;
		}

		theBigLock.Lock();
		if((frame.Flags&pcTIMER)==pcTIMER)
		{
			workingTCB = theTCBLink.findTCB( frame.Seq) ;
		}
		else
		{
			workingTCB = theTCBLink.findTCB( frame.SourAddress ) ;
			//if( !workingTCB  )
			if( !workingTCB  && ((frame.Flags  == (pcCONTROL|ptSYN)) ||
				(frame.Flags  == (pcCONTROL|ptPOOL)) ) )
			{
				PEER dest( frame.SourAddress ) ;
				workingTCB = new TTCB( theAppQueue, MainTimer, &theNSQ, theTCBLink.getCount(), dest, localAddr ) ;
				if( workingTCB )
					theTCBLink.insertTCB( workingTCB ) ;
			}
		}

		if( workingTCB )
		{
			workingTCB->HandleEvent( frame ) ;
		}

		theBigLock.Unlock();

	}//end of for( ; ; )

	DelRef() ;

	return 0 ;
}

void TUDPServer::TCBInfo(char *buffer, int maxsize )
{
	theBigLock.Lock( ) ;
	theTCBLink.TCBInfo(buffer, maxsize ) ;
	theBigLock.Unlock( ) ;
}

void TUDPServer::TraceInfo(char *buffer, int maxsize )
{
	theBigLock.Lock( ) ;	
	theTCBLink.TraceInfo(buffer, maxsize ) ;
	theBigLock.Unlock( ) ;
}


inline void TUDPServer::Broadcast( int port, const PACKETDATA & data )
{
	PEER       broadcastAddr( BROADCASTADDRESS, port ) ;
	TRANSFRAME frame( localAddr.addr, broadcastAddr.addr, 0, 0, 0, 0, &data ) ;
	theNSQ.SendFrame( frame ) ;
}

int TUDPServer::PostPacket( const PEER & dest, const PACKETDATA & data )
{
    TRANSFRAME frame( localAddr.addr, dest.addr, 0, 0, 0, 0, &data ) ; 
	return (theNSQ.SendFrame( frame )) ; 
}

void TUDPServer::Stop( )
{
	
	theNRQ.Stop();
	MainTimer->Stop();
	sysdown = 1;

	PACKETDATA packet(0,0,0/*EvtPUBLIC_TELSUN_UDPServerShutdown*/);
	TRANSFRAME frame( localAddr.addr, localAddr.addr, 0, 0, 0, 0, &packet ) ;
	theNSQ.SendFrame( frame ) ;	// exit for  udp thread

}

int  TUDPServer::StartTrace( CTuint tcbid) 
{
	int ret=-1 ;

	theBigLock.Lock();
	TTCB *tcb = theTCBLink.findTCB( tcbid ) ;
	if( tcb ) 
	{
		ret = 0 ;
		tcb->StartTrace();
	}

	theBigLock.Unlock();
	return ret ;
}

int TUDPServer::StopTrace( CTuint tcbid) 
{
	int ret=-1 ;

	theBigLock.Lock();
	TTCB *tcb = theTCBLink.findTCB( tcbid ) ;

	if( tcb ) 
	{
		ret = 0 ;
		tcb->StopTrace();
	}
	theBigLock.Unlock();

	return ret ;
}


