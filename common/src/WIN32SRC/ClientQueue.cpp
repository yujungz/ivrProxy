#include "ClientQueue.h"


CTAppQueue::CTAppQueue()  
{
	for( int i=0; i<constMAXMODULENUM; i++)
	{
		AppQueue[i] = new CLIENTQUEUE(defAPPQUEUESIZE);
		assert( AppQueue[i] );
		used[i] = 0 ;
	}

}

CTAppQueue::~CTAppQueue() 
{
	for(int i=0; i<constMAXMODULENUM; i++)						
	{
		used[i] = 0;
		delete AppQueue[i]  ;
		AppQueue[i] = NULL;
	}					
}

int CTAppQueue::Put(CTuint gor, PACKETDATA &packet)
{		
	int ret = E_NOTFOUND ;
	if( gor < constMAXMODULENUM && used[gor])
	{
		if(AppQueue[gor]->Put(packet)==0)
			ret = E_SUCCESS ;
		else
			ret = E_QUEUEFULL ;
	}
	return ret;
}

int CTAppQueue::PutHead(CTuint gor, PACKETDATA &packet)
{
	int ret = E_NOTFOUND ;
	if( gor < constMAXMODULENUM && used[gor])
	{
		if(AppQueue[gor]->PutHead(packet)==0)
			ret = E_SUCCESS ;
		else
			ret = E_QUEUEFULL ;
	}
	return ret;
}


int CTAppQueue::Get(CTuint gor, PACKETDATA &packet)
{
	int ret = E_NOTFOUND ;
	if( gor < constMAXMODULENUM && used[gor])
	{
		if(AppQueue[gor]->Get(packet)==0)
			ret = E_SUCCESS ;
		else
			ret = E_QUEUEEMPTY ;
	}
	return ret;
}

	
//return E_SUCCESS: put success
//		 E_QUEUEFULL: 
//		 E_NOTFOUND
int  CTAppQueue::TryPut(CTuint gor, PACKETDATA &packet) 
{
	int ret = E_NOTFOUND ;

	if( gor < constMAXMODULENUM  && used[gor]) 
	{
		if( AppQueue[gor]->TryPut(packet) == 0 )
			ret = E_SUCCESS ;
		else
			ret = E_QUEUEFULL ;
	}
	return ret;							
}

//Get a data from queue, if queue is empty that return -1 else return 0.
int  CTAppQueue::TryGet(CTuint gor, PACKETDATA &packet)
{
	int ret = E_NOTFOUND ;

	if( gor < constMAXMODULENUM  && used[gor]) 
	{
		if( AppQueue[gor]->TryGet(packet) == 0 )
			ret = E_SUCCESS ;
		else
			ret = E_QUEUEEMPTY ;
	}

	return ret;							
}

int	 CTAppQueue::Remove(CTuint gor)
{
	int ret = -1 ;
	if( gor < constMAXMODULENUM  && used[gor])		
		ret = AppQueue[gor]->Remove();
	
	return ret;
}

int  CTAppQueue::GetCount(CTuint gor) 
{
	int count=0;
	m_lock.Lock();
	if( gor < constMAXMODULENUM  && used[gor]) 
		count=AppQueue[gor]->GetCount();
	m_lock.Unlock();
	return count;
}	

int  CTAppQueue::IsEmpty(CTuint gor) 
{	
	int ret=0;
	m_lock.Lock();
	if( gor < constMAXMODULENUM && used[gor]) 
		ret =  AppQueue[gor]->IsEmpty();
	m_lock.Unlock();
	return ret;
}
	
int  CTAppQueue::IsOccupied(CTuint gor)
{	
	int ret=0;
	m_lock.Lock();
	if( gor < constMAXMODULENUM  && used[gor]) 
		ret = AppQueue[gor]->IsOccupied();
	m_lock.Unlock();
	return ret;
}
	

//exist: return 0 else return -1;
int CTAppQueue::IsExist(CTuint gor)
{
	int ret=0;
	m_lock.Lock();
	if(gor < constMAXMODULENUM  && used[gor]) 
		ret = 1;
	m_lock.Unlock();
	return ret;		
}

int CTAppQueue::AddQueue(CTuint gor) 
{
	int ret=-1 ;
	m_lock.Lock();
	if( gor < constMAXMODULENUM && !used[gor]) 
	{			
		AppQueue[gor]->RemoveAll();
		used[gor] = 1;
		ret = 0;
	}
	m_lock.Unlock();
	return ret;
}

int CTAppQueue::DelQueue(CTuint gor)
{
	m_lock.Lock();
	if( gor < constMAXMODULENUM  && used[gor] ) 
	{		
		AppQueue[gor]->RemoveAll();
		used[gor] = 0;
	}
	m_lock.Unlock();
	return 0;
}



