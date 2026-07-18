#include <time.h>

#include "Timer.h"
#include "UDPServer.h"


int CompareTimerID( void* data, void* key )
{
	CTTimerNode* rsc = ( CTTimerNode* )data;
	CTuint timerID = *((CTuint*)key);
	return ( rsc->timerID == timerID );
}


DWORD SubTick( DWORD newTick, DWORD oldTick )
{
	if ( newTick>=oldTick )
	{
		return newTick - oldTick;
	}
	else
	{
		return ( 0xFFFFFFFF - oldTick ) + 1 + newTick;
	}
}

DWORD AddTick( DWORD base, DWORD offset )
{
	DWORD negative = 0xFFFFFFFF - base + 1;
	if ( negative > offset )
	{
		return base + offset;
	}
	else
	{
		return negative - offset + 1;
	}
}
	
inline int isTickTimeUp( DWORD now, DWORD alarm )
{
	if ( now > alarm )
	{
		return 1;
	}
	else if ( constTIMEDELTA > ( alarm - now ) )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


DWORD WINAPI RunTimerThread( LPVOID lpvTimerParm )
{
	if( lpvTimerParm )
		( (CTTimer*)lpvTimerParm )->Working();
	return 0;
}

#include "tlog.h"


CTTimer::CTTimer( CTuint aTimeInterval )
	: timerIDCount( 1 ),sysdown(0), m_cRef(0)
{
	if( aTimeInterval > constMINTIMESLICE ) 
		TimeInterval = aTimeInterval ;
	else 
		TimeInterval = constMINTIMESLICE ;
	
	for( int i=0; i<HASH_TIMER_SESSION; i++ )
	{
		bySession[i] = new TCBCollection( 1, 1 ) ;
		assert( bySession[i] ) ;
	}
#ifdef _DEBUG_TIMER
	logfile = new TPrintLog(GOR_NetDevice,"log\\timer.log",3,LOG_TO_FILE);
	assert( logfile ) ;
#endif


	pthread = new CTThread( RunTimerThread, this, CREATE_SUSPENDED );
	assert( pthread );	
}

CTTimer::~CTTimer( )
{	
	int i;

	lock.Lock( ) ;

	for( i=0; i<HASH_TIMER_SESSION; i++ )
	     bySession[ i ]->shutDown( ) ;
	for( i=0; i<HASH_TIMER_SESSION; i++ )
	     delete bySession[ i ] ;

#ifdef _DEBUG_TIMER
	delete logfile;
#endif	
	lock.Unlock();

	if ( pthread ) 
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
		delete pthread ;
		pthread = NULL;
	}
}


CTuint CTTimer::timerSet( CTuint tcb_id, CTuint tcb_type, CTuint tcb_left )
{
	CTTimerNode * prev = 0, * curr = 0, * newTcb = 0 ;
	CTuint       ret ;

	if ( tcb_left < constMINTIMESLICE )
		return 0 ;
	
	lock.Lock();

	newTcb = new CTTimerNode( timerIDCount, tcb_id, tcb_type, tcb_left ) ;
	if( !newTcb ) {
		lock.Unlock();		
		return 0 ;
	}

	CTuint bucket = newTcb->timerID % HASH_TIMER_SESSION ;
	TCBCollection * pCollect = bySession[ bucket ] ;
	pCollect->insert( newTcb ) ;

	ret = timerIDCount ++ ;
	if ( timerIDCount > constMAXTIMERID )
		timerIDCount = 1 ;

#ifdef _DEBUG_TIMER
	logfile->printf( "tID=%d::Set id=0x%X, t=0x%X, left=%d,now=%d\n", 
			timerIDCount, tcb_id, tcb_type, tcb_left, GetTickCount()  ) ;
#endif

	//New list lines
	if ( tcbList.isEmpty( ) )
	{
		tcbList.Add( newTcb ) ;	
		lock.Unlock();
		return ret ;
	}

	//New list lines
	//Find the nearlest larger time node
	for( prev = 0,curr = tcbList.Head(); curr; prev = curr,curr = tcbList.Next(curr) )
	{
		if ( newTcb->timeLeft < curr->timeLeft ) 
			break ;
		newTcb->timeLeft -= curr->timeLeft ;	
	}

	if( prev )
		tcbList.InsertAfter( newTcb, prev ) ;

	else	
		tcbList.Add( newTcb ) ;
	

	if( curr )	
		curr->timeLeft -= newTcb->timeLeft ;
	
	lock.Unlock();
	return ret ;
}

CTerror CTTimer::timerClear( CTuint timerID )
{
	CTTimerNode * next = 0,* curr ;
	CTuint key = timerID, bucket ;
	TCBCollection * pCollect ;

	if ( timerID==0 )
		return CT_errorFAIL ;
	
	lock.Lock( ) ;

	bucket = timerID % HASH_TIMER_SESSION ;
	pCollect = bySession[ bucket ] ;
	curr = pCollect->firstThat( CompareTimerID , (void *)&key ) ;
	if( curr )
	{
#ifdef _DEBUG_TIMER
		logfile->printf( "tID=%d::Clear now=%d\n", timerID, GetTickCount( ) ) ;
#endif
		pCollect->remove( curr ) ;
		next = tcbList.Next( curr ) ;
		if ( next )
		{
			next->timeLeft += curr->timeLeft ;
		}
		tcbList.Del( curr ) ;
	} 
	else 
	{
#ifdef _DEBUG_TIMER
		logfile->printf( "tID=%d::Clear, but not found!\n", timerID );
#endif
	}

	
	lock.Unlock();
	return CT_errorOK ;
}

int CTTimer::Run( )
{
	return 0xFFFFFFFF==pthread->Continue( ) ? 0 : 1 ;
}

void CTTimer::Working( )
{
	#if __TIMER__
	cout << "[Timer] Running..." << endl ;
	#endif

	
	//DWORD tickAfterSleep ;
	DWORD tickNow;
	CTTimerNode * curr ;

	AddRef();

	for( ; ; )
	{
		Sleep( TimeInterval ) ;		
		if( sysdown )
			break ;

		lock.Lock();

		if( tcbList.isEmpty() )
		{			
			lock.Unlock();
			continue ;
		}
//#ifdef _DEBUG_TIMER
	//logfile->printf( "now tick=%d! hID=%d, left=%d\n",GetTickCount( ),tcbList.Head()->timerID,tcbList.Head()->timeLeft    );
//#endif
		curr =  tcbList.Head() ;

		if ( (unsigned long) curr->timeLeft > TimeInterval )
		{
			curr->timeLeft -= TimeInterval ;
		}
		else
		{
			curr->timeLeft = 0 ;
			goto labelREMOVETIMENODE ;
		}

		tickNow = GetTickCount( ) ;

//		tickAfterSleep = GetTickCount( ) ;
		//if ( isTickTimeUp( tickAfterSleep, tcbList.Head()->tickCount ) )
				//else if ( constTIMEDELTA > ( alarm - now ) )

		if( curr->tickCount < tickNow  )
		{
			curr->timeLeft = 0 ;
#ifdef _DEBUG_TIMER
			logfile->printf( "tID=%d::WARN! now=%d, left=%d, tick=%d\n",
						curr->timerID, tickNow, curr->timeLeft,curr->tickCount);
#endif
		}
		else
		{
			curr->timeLeft = curr->timeOldLeft - (tickNow - curr->tickOldCount) ;

/*			DWORD dwRealInterval = SubTick( tickNow, tcbList.Head()->tickOldCount ) ;
			tcbList.Head()->tickOldCount = tickNow ;
			DWORD dwAdjustSlice ;
			if ( dwRealInterval > TimeInterval )
			{
				dwAdjustSlice = dwRealInterval - TimeInterval ;
				if ( tcbList.Head()->timeLeft > dwAdjustSlice )
				{
					tcbList.Head()->timeLeft -= dwAdjustSlice ;
#ifdef _DEBUG_TIMER
			logfile->printf( "adjust:: tID=%d, timeLeft=%d, tickOld=%d, tickNow=%d, OLeft=%d\n",
					curr->timerID, curr->timeLeft, curr->tickOldCount, tickNow, curr->timeOldLeft);
#endif
				}
				else
				{
					tcbList.Head()->timeLeft = 0 ;
				}
			}
			else
			{
				dwAdjustSlice = TimeInterval - dwRealInterval ;
				if ( dwAdjustSlice > constTIMEDELTA )
				{
					tcbList.Head()->timeLeft += dwAdjustSlice ;
#ifdef _DEBUG_TIMER
			logfile->printf( "adjust:: tID=%d, timeLeft=%d, tickOld=%d, tickNow=%d, OLeft=%d\n",
					curr->timerID, curr->timeLeft, curr->tickOldCount, tickNow, curr->timeOldLeft);
#endif
				}
			}
*/
		}

labelREMOVETIMENODE:

		while( tcbList.Head() && (tcbList.Head()->timeLeft<constTIMEDELTA+constTIMEDELTA) )
		{
#ifdef _DEBUG_TIMER 
logfile->printf( "tID=%d::TimeOut  tcbid=%X, type=%x. now=%d\n", curr->timerID, curr->ownerID, curr->type, GetTickCount() );
#endif
			SendMessage( tcbList.Head() ) ;//Trigger timer message
			curr = tcbList.Head() ;//Preserve the old node pointer
			CTuint bucket = curr->timerID  % HASH_TIMER_SESSION ;
			TCBCollection * pCollect = bySession[ bucket ] ;
			pCollect->remove( curr ) ;//Remove the old node from list
			tcbList.Del( curr ) ;
		}//end of while()		
		lock.Unlock();
	}//end of for( ; ; )

	DelRef();
}//End of Working( )

// zg guiyang 11
CTerror CTTimer::timerClear( CTuint timerID , CTuint gor)
{
	CTTimerNode * next = 0,* curr ;
	CTuint key = timerID, bucket ;
	TCBCollection * pCollect ;

	if ( timerID==0 )
		return CT_errorFAIL ;
	
	lock.Lock( ) ;

	bucket = timerID % HASH_TIMER_SESSION ;
	pCollect = bySession[ bucket ] ;
	curr = pCollect->firstThat( CompareTimerID , (void *)&key ) ;
	if( curr )
	{
		if(curr->type!=gor)
		{
			FILE *fp=fopen("ftime.txt","at+");
			if(fp)
			{
				fprintf(fp,"timerid1=%d , gor=%d  , timerid2=%d , type=%d\n", timerID,gor,curr->timerID,curr->type);
				fclose(fp);
			}
			lock.Unlock();
			return CT_errorOK  ;  // zg guiyang 11
		}
#ifdef _DEBUG_TIMER
		logfile->printf( "tID=%d::Clear now=%d\n", timerID, GetTickCount( ) ) ;
#endif
		pCollect->remove( curr ) ;
		next = tcbList.Next( curr ) ;
		if ( next )
		{
			next->timeLeft += curr->timeLeft ;
		}
		tcbList.Del( curr ) ;
	} 
	else 
	{
#ifdef _DEBUG_TIMER
		logfile->printf( "tID=%d::Clear, but not found!\n", timerID );
#endif
	}

	
	lock.Unlock();
	return CT_errorOK ;
}


void CTTimer::printLog( )
{
	FILE * fp ;
	TCBCollection * p ;
	int total = 0, num = 0 ;
	
	lock.Lock();
	fp = fopen( "timer.log", "wt" ) ;
	if( !fp )
	{		
		lock.Unlock();
		return ;
	}
	CTTimerNode * curr = tcbList.Head() ;//CTTimerNode * curr = tcb ;
	while( curr )
	{
		fprintf( fp, "[%p]timerID=%5x userID=%5x Left=%3d [^%5p],[V%5p]\n",			
			curr, curr->ownerID, curr->ownerID, curr->timeLeft, tcbList.Prev(curr), tcbList.Next(curr) ) ;
		curr = tcbList.Next(curr) ;
		total ++ ;
	}
	fprintf( fp, "******** total=%d **********\n", total ) ;

	total = 0 ;
	for( CTint i=0; i<HASH_TIMER_SESSION; i++ )
	{
		p = bySession[i] ;
		assert( p ) ;
		num = p->getCount( ) ;
		total += num ;
		for( CTint j=0; j<num; j++)
		{
			curr = p->at(j) ;
			fprintf( fp, "HASH[%-4d][%-2d] p=%p timerID=%5x userID=%5x Left=%3d\n",
				i, j,curr, curr->timerID, curr->ownerID, curr->timeLeft) ;
		}
	}
	fprintf(fp, "  index by SESSION hash[HASH_SESSION=%d]total: %d.\n",
		HASH_TIMER_SESSION,total ) ;
	fclose( fp ) ;
	
	lock.Unlock();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
CTerror CTUDPTimer::SendMessage( const CTTimerNode * ctn )
{
	assert( ctn ) ;

	if( theSender )
	{
		TUDPFRAME frame;
		frame.Flags = pcTIMER ;        //frame type
		frame.Flags |= ctn->type ;     //timer type
		frame.Seq = ctn->ownerID ;     //which TCB
		theSender->putEvent(frame);
	}
	#ifdef _TRACE
	cout << "[UDPTimer] SendMessage() : type=" << ctn->type
		 << "ownerID=" << ctn->ownerID << endl ;
	#endif
	return CT_errorOK ;
}


