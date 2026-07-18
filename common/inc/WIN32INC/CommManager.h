#ifndef __COMMMANAGER_H__
#define __COMMMANAGER_H__

#include "cttypes.h"
#include "ctkvset.h"
#include "ctsymbol.h"
#include "ctmsg.h"
#include "ctpacket.h"

#include "Udp.h"
#include "Timer.h"
#include "UDPServer.h"

#include "ClientData.h"
//#include "Profile.h"
#include "tlog.h"
#include "tcpclient.h"

#define POLL_TIME		           60000
#define Console_LoadAdjacent       0x1

/////////////////////////////////////////////////////////////////////
// Constant

const int      constIPADDRBUFSIZE       = 20;
const int      constIPADDRCOUNT         = 3;


#define        stringLOGFILENAME          "CommMain.log";
#define        stringBROADCASTADDRESS     "255.255.255.255";

//const int      sliceTIMERINTERVAL       = 200;            // 0.2 sec

// Send Mode, flags used in SendPacket()

const int      flagCHECKMODULE          = 0x00000001;
const int      flagSENDTOMAINTAIN       = 0x00000002;

#define CHECKMODULE(flag)                 (flagCHECKMODULE & flag)
#define SENDTOMAINTAIN(flag)              (flagSENDTOMAINTAIN & flag)


class CTCMTimer : public CTTimer
{
	CTAppQueue	* theSender;

public:

	CTCMTimer( CTuint interval, CTAppQueue* aSender)
		: CTTimer( interval )
	{
		theSender = aSender;	
	}
	~CTCMTimer() {};

	virtual CTerror SendMessage( const CTTimerNode* );
};


/////////////////////////////////////////////////////////////////////
// TCommManager

class TCommManager
{
	// Local configurations.
	CTuint             StationNo;
	int                Port;
	
	CTuint             CommManagerId;
	int                IPAddressNum;
	int				   iIpAddrIndex;
	int				   iUseIpAddrIndex;
	char               IPAddress[constIPADDRCOUNT][constIPADDRBUFSIZE];

	CTClientData       ClientData;
	CTAppQueue		   AppQueue;
	TUDPServer*        UDPServer;

	CTThread           *Poller;
	CTCMTimer		   *AppTimer;

	char               ProfileName[MAX_PATH];
	char               LogfileName[MAX_PATH];
	TPrintLog		   *logfile;

public:
	TTCPClient*  TCPClient;  //add 2010.03.29

public:
	TCommManager(char *profile);
	~TCommManager();

	CTuint GetID() { return CommManagerId; }
	CTuint GetStationNo() { return StationNo; }	
	int	   GetPort() { return Port; }	
	char*  GetIPAddress( int i )	
	{
		if( i >= 0 && i < constIPADDRCOUNT )
			return IPAddress[ i ];
		else
			return NULL;
	}

	void   DoPoll();

	CTbool Run();
	CTbool Stop();

	CTerror ConnectToMe(CTuint aClientId);
	CTerror DisconnectFromMe(CTuint AClientId);
	CTerror SendMessageToMe( CTuint sender, CTuint receiver, CTtranInfo* tranInfo, CTbool hflag=CT_boolFALSE);
	CTerror GetMessageFromMe( CTuint* sender, CTuint receiver, CTtranInfo* tranInfo );

	//add by wjf 2004/01/06
	int strparse(const char* parameter, int index, const char* separator, char* section);
	char* strtrim(char *src); 
	//add end
	CTuint   timerClear( CTuint timerID , CTuint gor ) {return AppTimer->timerClear(timerID,gor);}
	CTuint   timerSet( CTuint value, CTuint gor, CTuint lefttime ){ return AppTimer->timerSet(value, gor, lefttime);}

	void	TraceOn() 
			{ 
				if( logfile )				
					logfile->setTraceMode(LOG_TO_FILE); 								
			}
	void    TraceOff()
			{
				if( logfile )				
					logfile->setTraceMode(LOG_TO_NULL); 						
			}
	CTbool AddStation( CTuint stationNo, const char *ip, int port);			
	CTbool ReloadProfile();
	

private:

	CTbool LoadLocalConfig( );
	CTbool ConnectToAll( CTuint& lastUnconnectedStation, CTbool delConnetion = CT_boolFALSE );
	CTbool ReadAdjacentStation( );


	CTbool OnEvtTimeOut( CTtranInfo& tranInfo );
	CTbool OnEvtSynStation( CTuint sender, CTtranInfo& tranInfo );
	void   OnEvtStationDisconnect( CTuint sender );
	void   OnEvtLoadProfile( CTuint sender, CTtranInfo& tranInfo );

	CTbool HandleMessage( PACKETDATA& aPacket );
	inline CTbool SendPacket( PACKETDATA& aPacket, int aFlag = flagCHECKMODULE|flagSENDTOMAINTAIN );
	//inline void   GetPacket ( PACKETDATA& aPacket );

	CTbool SendStationSync(CTuint station);
	void   SendAllStationSync(int mode=1);

	void   Backup( char* source, char* backup );
	CTbool Restore( char* backup, char* target );

	void   OnConsole( CTuint sender, CTtranInfo& tranInfo );

	void	AddRef() { m_cRef++ ;}
	void	DelRef() { m_cRef -- ;}

private:	
	int	   m_cRef;
	int    sysdown;
};

DWORD WINAPI doRunPoll( void* lpParm );

inline CTbool TCommManager::SendPacket( PACKETDATA& aPacket, int aFlag )
{
	CTbool	ret=CT_boolFALSE;
	PEER   peer;
	CTuint station = GETSTATION( aPacket.receiver );
	CTuint module = GETMODULE( aPacket.receiver );

	if( CHECKMODULE(aFlag) )
		if( !ClientData.CheckModule( station, module ) )
		{
			logfile->printf( "Send PacketInfo(Snd:0x%x Rcv:0x%x ID:0x%x ST:0x%x) From 0x%x To 0x%x Error! Remote Module Not Find\n",
				aPacket.tranInfo.sender,aPacket.tranInfo.receiver,aPacket.tranInfo.messageID,aPacket.tranInfo.status,aPacket.sender,aPacket.receiver);
			return CT_boolFALSE;
		}

	if( StationNo == station )
	{
		AppQueue.Put(module, aPacket);
		logfile->printf( "Send PacketInfo(Snd:0x%x Rcv:0x%x ID:0x%x ST:0x%x) From 0x%x To 0x%x OK!\n",
			aPacket.tranInfo.sender,aPacket.tranInfo.receiver,aPacket.tranInfo.messageID,aPacket.tranInfo.status,aPacket.sender,aPacket.receiver);
		return CT_boolTRUE;
	}

	//if( StationTable.Search( station, peer ) )
	if( ClientData.Search( station, peer ) )
	{
		switch ( UDPServer->SendPacket( peer, aPacket ) )
		{
		case E_SUCCESS:
			logfile->printf( "Send PacketInfo(Snd:0x%x Rcv:0x%x ID:0x%x ST:0x%x) From 0x%x To 0x%x Success!\n",
				aPacket.tranInfo.sender,aPacket.tranInfo.receiver,aPacket.tranInfo.messageID,aPacket.tranInfo.status,aPacket.sender,aPacket.receiver);
			ret = CT_boolTRUE ;
			break ;
			
		case E_QUEUEFULL:
			logfile->printf( "Send PacketInfo(Snd:0x%x Rcv:0x%x ID:0x%x ST:0x%x) From 0x%x To 0x%x Failed! Out Of Queue\n",
				aPacket.tranInfo.sender,aPacket.tranInfo.receiver,aPacket.tranInfo.messageID,aPacket.tranInfo.status,aPacket.sender,aPacket.receiver);
			break;	

		case E_TCBPAUSE:
			logfile->printf( "Send PacketInfo(Snd:0x%x Rcv:0x%x ID:0x%x ST:0x%x) From 0x%x To 0x%x Failed! Remote Socket Down\n",
				aPacket.tranInfo.sender,aPacket.tranInfo.receiver,aPacket.tranInfo.messageID,aPacket.tranInfo.status,aPacket.sender,aPacket.receiver);
			break;

		default:			
			logfile->printf( "Send PacketInfo(Snd:0x%x Rcv:0x%x ID:0x%x ST:0x%x) From 0x%x To 0x%x Failed!\n",
				aPacket.tranInfo.sender,aPacket.tranInfo.receiver,aPacket.tranInfo.messageID,aPacket.tranInfo.status,aPacket.sender,aPacket.receiver);
			break ;
		}
	}
	else	
		logfile->printf( "Send PacketInfo(Snd:0x%x Rcv:0x%x ID:0x%x ST:0x%x) From 0x%x To 0x%x Failed! Can't Find Remote Module\n",
			aPacket.tranInfo.sender,aPacket.tranInfo.receiver,aPacket.tranInfo.messageID,aPacket.tranInfo.status,aPacket.sender,aPacket.receiver);
	
	return ret ;
}

#endif // __CommManager_H__
