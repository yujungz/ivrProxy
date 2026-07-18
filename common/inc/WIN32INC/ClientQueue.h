#ifndef __ClientQueue_H__
#define __ClientQueue_H__

#include <assert.h>
#include "ctstatus.h"
#include "ctpacket.h"
#include "queue.h"
#include "KernelObject.h"

#define defAPPQUEUESIZE		2048   //old value is 512
typedef TQueue<PACKETDATA> CLIENTQUEUE ;

class CTAppQueue
{
public :
	CTAppQueue() ; 
	~CTAppQueue() ;

	int Put(CTuint gor, PACKETDATA &packet);
	int Get(CTuint gor, PACKETDATA &packet);

	int  TryPut(CTuint gor, PACKETDATA &packet) ;
	int  TryGet(CTuint gor, PACKETDATA &packet);
	int  PutHead(CTuint gor, PACKETDATA &packet);
	int	 Remove(CTuint gor);


	int  GetCount(CTuint gor) ;
	int  IsEmpty(CTuint gor) ;
	int  IsOccupied(CTuint gor);
	//exist: return 0 else return -1;
	int IsExist(CTuint gor);

	int AddQueue(CTuint gor) ;
	int DelQueue(CTuint gor);

private :	
	CLIENTQUEUE		  *AppQueue[constMAXMODULENUM];
	int				  used[constMAXMODULENUM];
	CTCriticalSection m_lock;

};

#endif
