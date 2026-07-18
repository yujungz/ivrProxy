#ifndef __WINCTMODULE_H__
#define __WINCTMODULE_H__

#include <string.h>
#include <assert.h>
#include <iostream.h>
#include <stdio.h>

#include "ctgor.h"
#include "cttypes.h"
#include "ctkvset.h"
#include "ctpacket.h"
#include "CommManager.h"

DWORD WINAPI doRunWorking( void* lpParm );

class TCTModule
{
	protected:
		CTuint id ;
		CTuint StationNo ;
		CTuint CommManagerId ;
		char   name[64] ;
		CTThread *pthread ;

	private:
		TCommManager *commManager;
		CTbool connected;

	public:
		TCTModule(CTuint aID, char *aName, TCommManager *aCommManager);
		~TCTModule() ;

		HANDLE GetHandle() { return  pthread->GetHandle(); };
		CTbool   isConnected() { return connected; }
		CTuint   GetID() { return id; };
		char*    GetName() { return name; };
		CTuint   GetStationNo() { return StationNo; }
//if sendmessage to self and hflag equl CT_boolTRUE . then put message in the head of queue.

		CTerror  SendMessage(CTuint receiver, CTtranInfo& tranInfo, CTbool hflag=CT_boolFALSE);
		virtual  CTbool Run(void);
		CTerror  Stop(void);
		virtual  void     Working(void);

		virtual CTuint   timerClear( CTuint timerID ) ;
		virtual CTuint   timerSet( CTuint value, CTuint lefttime );

		void	AddRef() { m_cRef++ ;}
		void	DelRef() { m_cRef -- ;}

	private:
		CTbool   DefaultHandleMessage(CTuint sender, CTtranInfo& tranInfo);

	protected:
		CTerror  Connect(void);
		CTerror  Disconnect(void);
		CTerror  GetMessage(CTuint& sender, CTtranInfo& tranInfo);
		virtual  CTbool   HandleMessage(CTuint sender, CTtranInfo& tranInfo)
		{ return CT_boolTRUE ; };
		virtual  CTerror  OnConnected(void) 
		{ return CT_errorOK; };
		virtual  void OnDisconnect(void) 
		{ return ; };


	private:
		CTerror  DoEcho(CTuint receiver);
		int		 m_cRef;
} ;

#endif
