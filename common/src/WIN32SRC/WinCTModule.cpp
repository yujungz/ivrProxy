#include "WinCTModule.h"

TCTModule::TCTModule(CTuint aID, char *aName, TCommManager *aCommManager)
	:m_cRef(0)
{

	assert(aCommManager);
	commManager = aCommManager;
	StationNo = commManager->GetStationNo();
	id = MAKEMODULEGOR(StationNo, aID);
	CommManagerId = MAKEMODULEGOR(StationNo, GOR_CommManager);


	strncpy( name, aName, 63);
	connected = CT_boolFALSE;

	pthread = new CTThread( doRunWorking, this, CREATE_SUSPENDED );
	assert( pthread );
	
};


TCTModule::~TCTModule() 
{
	if (connected)
		Disconnect();

	if ( pthread ) 
	{
		if( m_cRef > 0 ) 
		{
			time_t now=time(0);
			while( time(0) - now < 10 && m_cRef > 0 ) 
			{
				Sleep(200);
			}
		}
		_delete (pthread) ;
	}
};

CTerror  TCTModule::SendMessage(CTuint receiver, CTtranInfo& tranInfo, CTbool hflag)
{
	CTerror ret=CT_errorOK;

	for( int i=0; i<5; i++ )
	{
		ret = commManager->SendMessageToMe( id, receiver, &tranInfo, hflag);
		if( ret != CT_errorQUEUEFULL )
			break;
		Sleep(50);
	}

	return ret;
}

CTbool TCTModule::Run(void)
{
	if( 0xFFFFFFFF==pthread->Continue() )
		return CT_boolFALSE;
    return CT_boolTRUE;
}

CTerror  TCTModule::Stop(void)
{
	CTtranInfo tranInfo(id, id, EvtComm_TELSUN_Stop);
	return SendMessage(id, tranInfo);
}

CTerror  TCTModule::Connect(void)
{
	CTerror ret ;
	ret = commManager->ConnectToMe( id );
	if( ret == CT_errorOK )
		connected = CT_boolTRUE ;

	return ret;
}

CTerror  TCTModule::Disconnect(void)
{	
	connected = CT_boolFALSE;
	return commManager->DisconnectFromMe( id );
}

CTuint   TCTModule::timerClear( CTuint timerID ) 
{
	// zg guiyang 11
	return commManager->timerClear( timerID , GETMODULE(id) );
}

CTuint   TCTModule::timerSet( CTuint value, CTuint lefttime )
{
	return commManager->timerSet( value,GETMODULE(id),  lefttime );
}

void     TCTModule::Working(void)
{
	CTuint sender ;
	CTtranInfo tranInfo ;
	CTbool continueFlag = CT_boolTRUE ;


	AddRef();
	if (Connect()!=CT_errorOK)
	{
		DelRef();
		return ;
	}
	if (OnConnected()!=CT_errorOK)
	{
		DelRef();
		Disconnect();
		return ;
	}

	while(continueFlag)
	{
		if(GetMessage(sender, tranInfo)==CT_errorOK)
			continueFlag = DefaultHandleMessage(sender, tranInfo);
	}

	OnDisconnect() ;

	Disconnect();
	DelRef();
}

CTerror  TCTModule::GetMessage(CTuint& sender, CTtranInfo& tranInfo)
{
	return commManager->GetMessageFromMe( &sender, id, &tranInfo);
}

CTbool   TCTModule::DefaultHandleMessage(CTuint sender, CTtranInfo& tranInfo)
{
	CTbool ret = CT_boolTRUE ;

	switch(tranInfo.messageID)
	{
	case EvtComm_TELSUN_Stop:
		if( sender == CommManagerId || sender == id)
			ret = CT_boolFALSE;
		break;

	case CmdComm_TELSUN_Echo:
		DoEcho(sender);
		break;
	default:
		HandleMessage(sender, tranInfo);
		break;
	}
	return ret;
}

CTerror  TCTModule::DoEcho(CTuint receiver)
{
	CTtranInfo aTranInfo(id, receiver, RespComm_TELSUN_Echo);
	return SendMessage(receiver, aTranInfo);
}

DWORD WINAPI doRunWorking( void* lpParm )
{
	TCTModule * p = (TCTModule*)lpParm ;
	if(p)
		p->Working();
	//pthread->GetRealId());
	return 0 ;
}
