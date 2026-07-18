#ifndef __TIMER_H__
#define __TIMER_H__

#include <time.h>
#include <stdio.h>

#include "cttypes.h"
#include "ctsymbol.h"
#include "ctmsg.h"
#include "ctpacket.h"

#include "Queue.h"

#include "KernelObject.h"
#include "Collection.h"
#include "List.h"

#include "Udp.h"

const CTuint constMINTIMESLICE = 100;
const CTuint constTIMEDELTA = constMINTIMESLICE / 10;

DWORD SubTick( DWORD newTick, DWORD oldTick );
//DWORD AddTick( DWORD base, DWORD offset );
int isTickTimeUp( DWORD now, DWORD alarm );

//class TFRAMEQUEUE;
class TPrintLog;

///////////////////////////////////////////////////////////////////////////////
// Adjust the following constants to get maxium efficiency when needed
// 8, 16, 32, 64, 128, 256, 512, 1024 - 2^n
// 7, 13, 31, 61, 127, 251, 503, 1019 - prime of availible choices
#define HASH_TIMER_SESSION 127
#define constMAXTIMERID 0x10000000


///////////////////////////////////////////////////////////////////////////////
// TTimerNode
class CTTimerNode : public CTNode
{
	public:
		CTuint         ownerID;  // == TCB_ID
		CTuint	       type;     // == TCB_W or TCB_P or TCB_B

		CTuint         timerID ;  //used to identify the timenode in timer
		CTint          timeLeft ; //how much time left to trigger in milli-second
		CTint          timeOldLeft ; //total time left in time node list in milli-second

		CTuint          tickCount ;//Time in millisecond to trigger
		CTuint          tickOldCount ;

		//TTimerNode( CTuint id, CTuint ownerid, CTshort t, CTint left )
		CTTimerNode( CTuint id, CTuint ownerid, CTuint t, CTint left )
			: timerID( id ),
			ownerID( ownerid ),
			type( t )						
		{						
			if( left >  constMINTIMESLICE )
				timeLeft =  left ;
			else 
				timeLeft = constMINTIMESLICE; 
			timeOldLeft = timeLeft;

			tickOldCount = GetTickCount();
			tickCount = tickOldCount + timeLeft;
		}
		~CTTimerNode() {}
};

typedef TNSCollection< CTTimerNode >	   TCBCollection;
typedef CTList< CTTimerNode >           TTimeNodeList;

///////////////////////////////////////////////////////////////////////////////
// TTimer
class CTTimer
{
public :
	CTTimer( CTuint aTimeInterval ) ;
	~CTTimer( ) ;

	int  Run( ) ;
	void Working( ) ;
	void Stop() { sysdown=1; }

	// zg guiyang 11
	CTuint   timerClear( CTuint timerID , CTuint gor);
	CTuint   timerClear( CTuint timerID );
	//CTuint   timerSet( CTuint tcb_id, CTshort tcb_type, CTuint tcb_left );
	CTuint   timerSet( CTuint tcb_id, CTuint tcb_type, CTuint tcb_left );
	void 	 printLog( );
	void	 AddRef() { m_cRef++ ;}
	void	 DelRef() { m_cRef -- ;}

	virtual CTerror SendMessage( const CTTimerNode * ) = 0;

private :
	TTimeNodeList   tcbList;
//	CTMutex			lock;
	CTCriticalSection lock;
	CTThread		*pthread;
	CTuint 		    TimeInterval;//Time interval to sleep in millisecond

	CTuint 		    timerIDCount ;
	TCBCollection * bySession[ HASH_TIMER_SESSION ] ;
	int				sysdown;
#ifdef _DEBUG_TIMER 
	TPrintLog		*logfile ;
#endif	

	int		 m_cRef;
};

///////////////////////////////////////////////////////////////////////////////
// TUDPTimer
class CTUDPTimer : public CTTimer
{
	TUDPRecvSocket * theSender;

public:
	CTUDPTimer( CTuint interval, TUDPRecvSocket* q)
		: CTTimer( interval ),theSender(q)
	{		
	}
	~CTUDPTimer() {};

	virtual CTerror SendMessage( const CTTimerNode* );
};

#endif//__TIMER_H__
