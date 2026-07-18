#include "CommManager.h"
CTerror CTCMTimer::SendMessage( const CTTimerNode * ctn )
{	
	if( ctn && theSender )
	{
		CTuint gor = GETMODULE(ctn->type);
		CTuint sender =  MAKEMODULEGOR(0,GOR_CommManager);		
		CTuint receiver =  MAKEMODULEGOR(0,gor);		
		CTtranInfo tranInfo(ctn->timerID,ctn->ownerID,EvtPUBLIC_TELSUN_TimeOut);
		PACKETDATA data(sender,receiver, tranInfo );
		theSender->Put( gor, data );		
	}
	return CT_errorOK ;
};

/////////////////////////////////////////////////////////////////
// TCommManager members

TCommManager::TCommManager(char *profile) :
	sysdown(0), m_cRef(0)
{
	for( int i=0; i<constIPADDRCOUNT; i++ )
		memset( IPAddress[i], 0, constIPADDRBUFSIZE);
	strncpy( ProfileName, profile, MAX_PATH);
	logfile = NULL;	
	UDPServer = NULL ;
	AppTimer= NULL;
	Poller = NULL ;
	IPAddressNum=0;
	iUseIpAddrIndex = 0;
	iIpAddrIndex = GetPrivateProfileInt("LOCALCONFIG","GetLocalAddr",0,ProfileName);
	TCPClient = 0;
}

TCommManager::~TCommManager()
{
   if( Poller )
   {
	   sysdown = 1 ;
	   if( m_cRef > 0 ) 
	   {
			time_t now=time(0);
			while( time(0) - now < 20 && m_cRef > 0 )
			{
				Sleep(200);			
			}
		}

		delete Poller ;
		Poller = NULL;
	}

	if( AppTimer ) 
	{
		delete (AppTimer) ;
		AppTimer = NULL;
	}
    if(TCPClient) {	delete TCPClient; TCPClient = NULL; }

	if( UDPServer )
	{
		delete UDPServer;	
		UDPServer = NULL;
	}

	if( logfile )  
	{
		delete logfile ;
		logfile = NULL;
	}
}

CTbool TCommManager::Run()
{
	Poller = new CTThread( doRunPoll, this, CREATE_SUSPENDED );
	if( !Poller  )
		return CT_boolFALSE ;

#ifdef WIN32
	WORD    wVersionRequested ;
	WSADATA wsaData ;

	wVersionRequested = MAKEWORD( 2, 0 ) ;
	if( WSAStartup( wVersionRequested, &wsaData ) != 0 )
	{
		return CT_boolFALSE ;
	}

	char hostname[256] = "" ;
	if( !gethostname( hostname, 256 ) )
	{
		struct hostent * hp ;
		hp = gethostbyname( hostname ) ;
		if ( hp )
		{
			struct in_addr addr ;
			int nAdapter=0;
			while( hp->h_addr_list[nAdapter] )
			{
				memcpy( (void*)&addr.s_addr, (void*)(hp->h_addr_list[nAdapter]),sizeof(addr.s_addr) ) ;
				nAdapter ++ ;
				if(nAdapter>3)
					break;
				strcpy(IPAddress[nAdapter-1],inet_ntoa(addr));
				IPAddressNum++;
			}
		}
	}

#endif

	if(::GetPrivateProfileInt("TcpProxy","WorkMode",0,ProfileName)!=0)
	{
		TCPClient = new TTCPClient(ProfileName,&AppQueue);
		assert(TCPClient);		
		if(TCPClient->mWorkMode==1)
		{
			if(TCPClient->Run() != CT_boolTRUE)
			{
				delete TCPClient;
				TCPClient = 0;
				WSACleanup();
				logfile->printf("[nw]: Error: Fail to start tcp client !\n");
				return CT_boolFALSE;
			}		
		}
		else
			TCPClient->Run();
	}

	if ( !LoadLocalConfig(  ) )
	{
		return CT_boolFALSE;
	}
	else
	{
		for(int i=0;i<IPAddressNum;i++)
			logfile->printf( "Get Configure:NetCard(%d):%s! Used Index(%d)\n",(i+1),IPAddress[i],iUseIpAddrIndex);
	}

	UDPServer = new TUDPServer(IPAddress[iUseIpAddrIndex], Port, &AppQueue, 0 );
	assert( UDPServer );	
	if( UDPServer->Run()  != CT_boolTRUE)
		return CT_boolFALSE;

	AppTimer = new CTCMTimer( 200, &AppQueue );
	assert( AppTimer );
	if( !AppTimer->Run() )
		return CT_boolFALSE ;

	timerSet(0, GOR_CommManager, 10000);
	timerSet(0, GOR_CommManager, 20000);

	if( 0xFFFFFFFF==Poller->Continue() )
		return CT_boolFALSE;

	SendAllStationSync();

	return CT_boolTRUE;
}

CTbool TCommManager::LoadLocalConfig( )
{
	char comm_item[512],getValue[128];
	char ipaddr[constIPADDRBUFSIZE];
	int	 len=0,i=0;

	memset(ipaddr,0,constIPADDRBUFSIZE);
	memset(comm_item,0,sizeof(comm_item));
	GetPrivateProfileString("LOCALCONFIG","Module","",comm_item,sizeof(comm_item),ProfileName);
	if(1>strparse(comm_item,1,",",getValue))
		StationNo = -1;
	else
		StationNo = atoi(getValue);

	if(2>strparse(comm_item,2,",",getValue))
		Port = -1;
	else
		Port = atoi(getValue);

	if(IPAddressNum==1)  //只有一个IP地址的时候
	{
		if(4>strparse(comm_item,4,",",getValue))
		{
			if(3>strparse(comm_item,3,",",getValue))
				strcpy(ipaddr,"127.0.0.1");
			else
				strcpy(ipaddr,strtrim(getValue));
		}
		else
			strcpy(ipaddr,strtrim(getValue));

		if(strcmp(ipaddr,"127.0.0.1")==0)
		{
			strcpy(IPAddress[0],ipaddr);
			iUseIpAddrIndex = 0;
		}
		else
		{
			strcpy(ipaddr,IPAddress[0]);
			iUseIpAddrIndex = 0;
		}
	}
	else if(iIpAddrIndex==0)
	{
		if(4>strparse(comm_item,4,",",getValue))
		{
			if(3>strparse(comm_item,3,",",getValue))
				strcpy(ipaddr,"127.0.0.1");
			else
				strcpy(ipaddr,strtrim(getValue));
		}
		else
			strcpy(ipaddr,strtrim(getValue));
		if(strcmp(ipaddr,"127.0.0.1")==0)
		{
			strcpy(IPAddress[0],ipaddr);
			iUseIpAddrIndex = 0;
		}
		else
		{
			len = strlen(ipaddr);
			for(i=0;i<IPAddressNum;i++)
			{
				if(strncmp(IPAddress[i],ipaddr,len)==0)
					break;
			}
			if(i>=IPAddressNum)
				return CT_boolFALSE;
			else
				iUseIpAddrIndex = i;
		}
	}
	else
	{
		if(iIpAddrIndex>3)
		{
			strcpy(ipaddr,IPAddress[0]);
			iUseIpAddrIndex = 0;
		}
		else
		{
			iUseIpAddrIndex = iIpAddrIndex-1;
			strcpy(ipaddr,IPAddress[iIpAddrIndex-1]);
		}
	}
	
	if(5>strparse(comm_item,5,",",getValue))
		strcpy(LogfileName,".\\log\\network.log");
	else
		strcpy(LogfileName,strtrim(getValue));
    //{ wy edit
    char sPathLog[250];
    GetPrivateProfileString("LOCALCONFIG", "PathLog", "..\\..\\logs\\", sPathLog, sizeof(sPathLog), ProfileName);
    logfile = new TPrintLog("network.log", sPathLog);
    //}
	//logfile = new TPrintLog(LogfileName, "" ) ;
	assert( logfile ) ;
	logfile->setTraceMode( LOG_TO_FILE ) ;

	if( StationNo != -1 && Port != -1 && IPAddressNum > 0 )
	{
		ClientData.AddStation( StationNo, ipaddr, Port,NULL );
		logfile->printf("Get Configure Ok:StationID[%d] Address[%s] Port[%d]\n",StationNo,ipaddr,Port);
	}
	else
	{		
		logfile->printf( "Get Configure Failed! Read StationID(%d) or Address(%s) or Port(%d) fail!\n",StationNo,ipaddr,Port);
		return CT_boolFALSE;
	}

	CommManagerId = MAKEMODULEGOR( StationNo, GOR_CommManager );
	AppQueue.AddQueue(GOR_CommManager);

	ClientData.AddModule( StationNo, GOR_CommManager );

	if( ReadAdjacentStation( ) != CT_boolTRUE )
		logfile->printf("Get Configure:No Remote Communication Module!\n");		

	return CT_boolTRUE;
}

CTbool TCommManager::ReadAdjacentStation( )
{
	int    r_station=-1, r_port=-1, count;
	char   r_ip[constIPADDRBUFSIZE];
	char   comm_item[512],getValue[128],title1[20];

	int remote_count = GetPrivateProfileInt("REMOTEMODULE","count",0,ProfileName);
	for( count=0;count<remote_count; count++ )
	{
		memset( r_ip, 0, constIPADDRBUFSIZE);
		memset(comm_item,0,sizeof(comm_item));
		sprintf(title1,"Remote%03d",(count+1));
		GetPrivateProfileString("REMOTE_ITEM",title1,"",comm_item,sizeof(comm_item),ProfileName);
		if(1>strparse(comm_item,1,",",getValue))
			r_station = -1;
		else
			r_station = atoi(getValue);
		if(2>strparse(comm_item,2,",",getValue))
			r_port = -1;
		else
			r_port = atoi(getValue);
		if(3>strparse(comm_item,3,",",getValue))
			strcpy(r_ip,"");
		else
		{
			strcpy(r_ip,strtrim(getValue));
		}
		if(4<strparse(comm_item,4,",",getValue))
		{
			strtrim(getValue);
			if(strncmp(getValue,"1",1)==0)
			{
				if(iIpAddrIndex>0 && iIpAddrIndex<=constIPADDRCOUNT)  //使用本机地址
					strcpy(r_ip,IPAddress[iUseIpAddrIndex]);
			}
		}

		if( r_station == -1 || r_port == -1 || strcmp(r_ip, "") == 0 )
			break ;
		ClientData.AddStation( r_station, r_ip, r_port,NULL );
		//ClientData.SetKeepLive(r_station, CT_boolTRUE );
		ClientData.SetKeepLive(r_station, CT_boolFALSE );
		logfile->printf( "Get Configure: Remote StationID[%d] Address[%s] Port[%d]\n", r_station,r_ip,r_port);
	}

	if( count==0 ){
		return CT_boolFALSE ;
	}
	return CT_boolTRUE ;
}


CTbool TCommManager::Stop()
{	
	PEER peer;

	for( int gor=constMINMODULEGOR; gor<constMAXMODULENUM; gor++ ) 
	{
		if( AppQueue.IsExist(gor) )
		{			
			PACKETDATA packet(CommManagerId,MAKEMODULEGOR(StationNo,gor),EvtComm_TELSUN_Stop);
			AppQueue.Put(gor, packet);
		}
	}

	Sleep(300);
	sysdown = 1 ;

	ClientData.DelStation( StationNo );
	SendAllStationSync();
	Sleep(100);

	for( CTuint station=constMINSTATIONNUM; station<=constMAXSTATIONNUM; station++ )	
	{
		if ( station!=StationNo && ClientData.Search( station, peer ) )
		{
			PACKETDATA packet(CommManagerId , 0, EvtPUBLIC_TELSUN_StationDisconnect );
			packet.sender = packet.tranInfo.sender = CommManagerId  ;
			packet.receiver = packet.tranInfo.receiver = MAKEMODULEGOR(station,GOR_CommManager);
			if(TCPClient && TCPClient->IsProxyStation(station))
			{
				TCPClient->SendPacket(packet);
				logfile->printf("[%02X]: Send Tcp EvtStationDisconnect from %X to %X(STATION:%d)\n",GOR_CommManager,CommManagerId,packet.receiver,station);
			}
			else
			{
				UDPServer->PostPacket(peer, packet);
				logfile->printf("[%02X]: Send EvtStationDisconnect from %X to %X(STATION:%d)\n",GOR_CommManager,CommManagerId,packet.receiver,station);
			}
		}
	}
	PACKETDATA mes( CommManagerId, CommManagerId, EvtPUBLIC_TELSUN_PollInterval );
	AppQueue.TryPut(GOR_CommManager,mes);   //stop poll thread

	Sleep(1000);
	if(AppTimer)
		AppTimer->Stop();
	if(TCPClient) TCPClient->Stop();  //必须注意顺序：TcpClient在前面，以便文明释放SOCKET
	if(UDPServer) UDPServer->Stop();
	return CT_boolTRUE;
}


CTbool TCommManager::SendStationSync(CTuint station)
{
	CTuint     receiver=MAKEMODULEGOR( station, GOR_CommManager );
	PEER	   peer;	
	CTbyte     l_module[constSTATIONDATASIZE+1], r_module[constSTATIONDATASIZE+1] ;
	int			ret ;

	if( station == StationNo || !ClientData.Search( station, peer ) )
	{
		logfile->printf("Send Synchronization Message To Remote StationID[%d] Failed!!!\n",	station);
		return CT_boolFALSE;
	}

	memset( l_module, 0, constSTATIONDATASIZE);
	memset( r_module, 0, constSTATIONDATASIZE);
	
	ClientData.GetStationData( StationNo ,l_module );
	ClientData.GetStationData( station ,r_module );

	PACKETDATA packet( CommManagerId, receiver, EvtComm_TELSUN_SynStation );
	packet.receiver = packet.tranInfo.receiver = receiver;
		
	TKVSet&    kvset = packet.tranInfo.GetMessageData();			
	kvset.PutByteArray( PUBLIC_TELSUN_SenderModule, l_module, constSTATIONDATASIZE );
	kvset.PutByteArray( PUBLIC_TELSUN_ReceiverModule, r_module, constSTATIONDATASIZE );
	kvset.PutString( PUBLIC_TELSUN_IP, IPAddress[iUseIpAddrIndex] );
	kvset.PutInt( PUBLIC_TELSUN_Port, Port );

	if(TCPClient && TCPClient->IsProxyStation(GETSTATION(packet.receiver)))
	{
		if(TCPClient->mWorkMode==2)
			ret = TCPClient->SendPacket(packet);
		else if(TCPClient->bRegistered)
			ret = TCPClient->SendPacket(packet);
	}
	else{
		ret = UDPServer->SendPacket(peer,packet);
	}
	
	switch ( ret )
	{
	case E_SUCCESS:		
		logfile->printf("Send Synchronization Message To Remote StationID[%d] OK\n", station );
		return CT_errorOK;

	case E_QUEUEFULL:	
		logfile->printf("Send Synchronization Message To Remote StationID[%d] Failed:Out of Queue!\n", station);
		return CT_errorQUEUEFULL;

	case E_TCBPAUSE:
		logfile->printf("Send Synchronization Message To Remote StationID[%d] Failed:Socket Pause!\n", station) ;
		return CT_errorTCBPAUSE;

	default:
		logfile->printf("Send Synchronization Message To Remote StationID[%d] Failed:0x%x\n", station,ret) ;
		return CT_errorFAIL;
	}
	return (ret==E_SUCCESS?CT_boolTRUE:CT_boolFALSE);
}	

void TCommManager::DoPoll()
{
	PACKETDATA packet;
	AddRef();
	while( !sysdown )
	{
		packet.tranInfo.messageID=0;
		AppQueue.Get(GOR_CommManager,packet );
		HandleMessage( packet );
	}
	DelRef();
}

CTbool TCommManager::HandleMessage( PACKETDATA& aPacket )
{
	CTtranInfo& tranInfo = aPacket.tranInfo;
	CTuint&     sender   = aPacket.sender;
	CTuint&     receiver = aPacket.receiver;	
	CTbool		ret = CT_boolTRUE;

	switch ( tranInfo.messageID )
	{
		case EvtPUBLIC_TELSUN_LoadProfile:			
			OnEvtLoadProfile( sender, tranInfo );
			break;			
		case EvtComm_TELSUN_SynStation:			
			OnEvtSynStation( sender, tranInfo );
			break;			
		case EvtPUBLIC_TELSUN_StationDisconnect:			
			OnEvtStationDisconnect( sender );
			break;			
		case CmdMAINT_TELSUN_Console:
			OnConsole( sender, tranInfo );
			break;
		case EvtPUBLIC_TELSUN_TimeOut:
			OnEvtTimeOut( tranInfo );
			break ;
		case EvtComm_TELSUN_Stop :
			logfile->printf("Recv Remote Module Stop Working or ShutDown!\n");
			ret = CT_boolFALSE;
			break;			
		default:
			logfile->printf("Recv UnDefined PacketInfo(Snd:0x%x,Rcv:0x%x,ID:0x%x) From 0x%x to 0x%x\n",tranInfo.sender,tranInfo.receiver,tranInfo.messageID,sender,receiver);
			break;
	}

	return ret;
}

void TCommManager::SendAllStationSync(int mode)
{
	for(CTuint station=constMINSTATIONNUM;station<=constMAXSTATIONNUM;station++)
		if(station!=StationNo && ClientData.CheckStation(station))
		{
			if(mode==1)
			{
				SendStationSync(station);
				continue;
			}
			if(ClientData.CheckModuleActive(station))
			{
				if(ClientData.GetSyncCount(station)<5)
				{
					ClientData.IncSyncCount(station);
					SendStationSync(station);
				}
			}
			else
				SendStationSync(station);
		}
}

/////////////////////////////////////////////////////////////////
// Message procedures

CTbool TCommManager::OnEvtTimeOut( CTtranInfo& tranInfo )
{
	SendAllStationSync(0);	
	/*time_t tm1, tm2;
	for(CTuint gor=0; gor<constMAXMODULENUM; gor++)
	{
		tm1 = ClientData.GetTM1(gor);
		tm2= ClientData.GetTM2(gor);
		if( tm2 > tm1 + 120 )
			logfile->printf("Warning: Find Module[0x%x] Out Of Working! TimeTick1=%d, TimeTik2=%d\n",gor, tm1, tm2);
	}
	*/
	timerSet(0, GOR_CommManager, POLL_TIME );
	return CT_boolTRUE;
}


CTbool TCommManager::OnEvtSynStation( CTuint sender, CTtranInfo& tranInfo )
{
	CTuint  station = GETSTATION( sender );
	TKVSet& kvset = tranInfo.GetMessageData();
	CTuint  moduleSize;
	CTbyte* module;
	CTbool retval=CT_boolFALSE, changed=CT_boolFALSE;
	PEER peer;
	char *ip;
	CTint port ;

	if( StationNo == station ) 
	{
		logfile->printf("Recv Synchronization Message From 0x%x. Me-To_Me\n", sender);
		return CT_boolFALSE;
	}

	if( CT_errorOK !=tranInfo.GetMessageData().GetString( PUBLIC_TELSUN_IP, &ip ) ||
		CT_errorOK != tranInfo.GetMessageData().GetInt( PUBLIC_TELSUN_Port, &port ) )
	{
		logfile->printf("Recv Synchronization Message From 0x%x. Get Key Error.\n", sender);
		return CT_boolFALSE;
	}

	int retf=ClientData.Search( station, ip, port) ;
	if( retf == -1 ) //not find
	{
		logfile->printf("Recv Synchronization Message From 0x%x. Auto Added\n",sender);
		ClientData.AddStation( station, ip, port, NULL );
		changed = CT_boolTRUE;
	}
	else if (retf == 1 ) //found. but ip or port not same
	    logfile->printf("Recv Synchronization Message From 0x%x. But Address[%s] or Port[%d] Is Not Same As Configure[StationID=%d]\n",sender,ip?ip:"",port,station);
	else
		logfile->printf("Recv Synchronization Message From 0x%x!\n",sender);	


	if( CT_errorOK == kvset.GetByteArray( PUBLIC_TELSUN_SenderModule, &module, &moduleSize ) &&
		moduleSize >= constSTATIONDATASIZE )
	{
		retval = ClientData.AddStation( station, ip, port, 0);
		for( int gor=constMINCLIENTMODULEGOR; gor<constMAXMODULENUM; gor++ )
		{
			if( ::GetGorFromClientData(gor, module) && !(ClientData.CheckModule(station,gor)) ) 
			{//module active
				 ClientData.AddModule(station,gor);  //!!!!增加这一行
				 changed = CT_boolTRUE ;
				 logfile->printf("Remote Module(0x%x:0x%x) Is Actived!\n", station, gor);
				 for( int module=constMINCLIENTMODULEGOR; module<constMAXMODULENUM; module++ )					 
				 {
					 if( AppQueue.IsExist(module ) ) //found
					 {
						 PACKETDATA packet(0, 0, EvtPUBLIC_TELSUN_ModuleActivate);
						 packet.sender = packet.tranInfo.sender = MAKEMODULEGOR(station,gor);
						 packet.receiver = packet.tranInfo.receiver = MAKEMODULEGOR(StationNo,module);
						 AppQueue.Put(module,packet);
					 }
				 }
			} 
			else if ( !(::GetGorFromClientData(gor, module)) && ClientData.CheckModule(station, gor) &&
						moduleSize >= constSTATIONDATASIZE ) 
			{//module deactive
				 changed = CT_boolTRUE ;
				 logfile->printf("Remote Mudule(0x%x:0x%x) is Disconnected!\n", station, gor);
				 for( int module=constMINCLIENTMODULEGOR; module<constMAXMODULENUM; module++ )					 
				 {
					 if( AppQueue.IsExist(module ) ) //found
					 {
						 PACKETDATA packet(0, 0, EvtPUBLIC_TELSUN_ModuleDeactivate);
						 packet.sender = packet.tranInfo.sender = MAKEMODULEGOR(station,gor);
						 packet.receiver = packet.tranInfo.receiver = MAKEMODULEGOR(StationNo,module);
						 AppQueue.Put(module,packet);
					 }
				 }
			}
		}
		//retval = ClientData.AddStation( station, ip, port, module );
	}

	if( CT_errorOK == kvset.GetByteArray( PUBLIC_TELSUN_ReceiverModule, &module, &moduleSize ) )
	{
	    if( moduleSize ==  constSTATIONDATASIZE &&
			ClientData.CheckStationData(StationNo,module) == CT_boolFALSE ) 
		{
			SendStationSync(station);		
		}
	}	

	return retval ;
}

void TCommManager::OnEvtStationDisconnect( CTuint sender )
{
	CTuint station = GETSTATION( sender );

	if( StationNo == station ) 
	{
		logfile->printf("Recv Remote Module Disconnect Message From 0x%x. Me-To-Me\n",sender);
		return ;
	}

    if( !ClientData.IsKeepLive( station )  )
    {
        ClientData.DelStation( station );
		logfile->printf("Recv Remote Module Disconnect Message From 0x%x.Delete It\n",sender);
    }
    else
		logfile->printf("Recv Remote Module Disconnect Message From 0x%x.Keep It!\n",sender);
}


CTerror TCommManager::ConnectToMe(CTuint aClientId)
{
	CTuint station = GETSTATION(aClientId);
	CTuint gor = GETMODULE(aClientId);
	if(gor>=constMAXMODULENUM ||station!=StationNo)
	{
		logfile->printf("Recv Poll Message For Module(0x%x). Not Poll Me(0x%x).\n", aClientId,StationNo);
		return CT_errorFAIL;
	}

	if( sysdown  ) 
	{
		logfile->printf("Recv Poll Message To Me(0x%x) But Socket Link Down!\n",aClientId);
		return CT_errorFAIL;
	}

	if(AppQueue.IsExist(gor) ) //found
	{
		logfile->printf("Recv Poll Message  Me-To-Me(0x%x). Ignored It!\n",aClientId);
		return CT_errorFAIL;
	}
	ClientData.AddModule( StationNo, gor );
	AppQueue.AddQueue(gor);
		
	logfile->printf("Recv Remote Module Active Message From (0x%x:0x%x)!\n",station,gor ) ;

	SendAllStationSync();

	return CT_errorOK;
}

CTerror TCommManager::DisconnectFromMe(CTuint aClientId)
{
	CTuint station = GETSTATION(aClientId);
	CTuint gor = GETMODULE(aClientId);

	if(gor > constMAXMODULEGOR ||station!=StationNo)
		return CT_errorFAIL;
	
	AppQueue.DelQueue(gor);
	ClientData.DelModule( aClientId );
	logfile->printf("Recv Remote Module Disconnect Message From (0x%x:0x%x)!\n",station,gor ) ;

	SendAllStationSync();

	return CT_errorOK;
}

CTerror TCommManager::SendMessageToMe( CTuint sender, CTuint receiver, CTtranInfo* tranInfo, CTbool hflag )
{
	CTuint station = GETSTATION(receiver);
	CTuint gor = GETMODULE(receiver);
	int ret=0;

	if(TCPClient && TCPClient->IsProxyStation(station))
	{
		PACKETDATA packet(sender,receiver,*tranInfo);
		ret = TCPClient->SendPacket(packet);
		logfile->printf( "[%02X]: Send Tcp tranInfo(%X,%X,ID:%X,%X) from %X to %X ret=0x%x\n",
			GETMODULE(sender),tranInfo->sender,tranInfo->receiver,tranInfo->messageID,tranInfo->status,sender,receiver,ret);
		return ret;
	}

	if(sysdown)
	{
		logfile->printf("Send PacketInfo(Snd:0x%x Rcv:0x%x ID:0x%x ST:%d) from 0x%x to 0x%x Error. Socket Link Down. QueueCount=%d\n", 
				tranInfo->sender,tranInfo->receiver,tranInfo->messageID,tranInfo->status,sender,receiver,AppQueue.GetCount(gor));
		return CT_errorFAIL;		
	}

	PACKETDATA packet(sender, receiver, *tranInfo);
	if( StationNo == station )
	{
		if( !ClientData.CheckModule( station, gor ) )
		{
			logfile->printf("Send PacketInfo(Snd:0x%x Rcv:0x%x ID:0x%x ST:%d) from 0x%x to 0x%x Error. Can't Find Module[%d]. QueueCount=%d\n", 
					tranInfo->sender,tranInfo->receiver,tranInfo->messageID,tranInfo->status,sender,receiver,gor,AppQueue.GetCount(gor));
			return CT_errorBadReceiver;
		}
    
		if( hflag == CT_boolTRUE )
		{
			ret = AppQueue.PutHead(gor, packet);
		}
		else
		{
			ret = AppQueue.TryPut(gor, packet);
		}
		if(ret==0)
		{
			logfile->printf("Send PacketInfo(Snd:0x%x Rcv:0x%x ID:0x%x ST:%d) from 0x%x to 0x%x OK. QueueCount=%d\n", 
					tranInfo->sender,tranInfo->receiver,tranInfo->messageID,tranInfo->status,sender,receiver,AppQueue.GetCount(gor));
			return CT_errorOK;
		}
		else
		{
			logfile->printf("Send PacketInfo(Snd:0x%x Rcv:0x%x ID:0x%x ST:%d) from 0x%x to 0x%x Error. Out Of Queue. QueueCount=%d\n", 
					tranInfo->sender,tranInfo->receiver,tranInfo->messageID,tranInfo->status,sender,receiver,AppQueue.GetCount(gor));
			return CT_errorQUEUEFULL;
		}
	}

	PEER peer;
	if( !ClientData.Search( station, peer ) )
	{
		logfile->printf("Send PacketInfo(Snd:0x%x Rcv:0x%x ID:0x%x ST:%d) from 0x%x to 0x%x Failed. Dest Not Exist. QueueCount=%d\n", 
				tranInfo->sender,tranInfo->receiver,tranInfo->messageID,tranInfo->status,sender,receiver,AppQueue.GetCount(gor));
		return CT_errorFAIL;
	}

	switch ( (ret=UDPServer->SendPacket( peer, packet )) )
	{
	case E_SUCCESS:		
		logfile->printf("Send PacketInfo(Snd:0x%x Rcv:0x%x ID:0x%x ST:%d) from 0x%x to 0x%x Success!\n", 
				tranInfo->sender,tranInfo->receiver,tranInfo->messageID,tranInfo->status,sender,receiver,AppQueue.GetCount(gor));
		return CT_errorOK;
	case E_QUEUEFULL:	
		logfile->printf("Send PacketInfo(Snd:0x%x Rcv:0x%x ID:0x%x ST:%d) from 0x%x to 0x%x Failed! Out of Queue. QueueCount=%d \n", 
				tranInfo->sender,tranInfo->receiver,tranInfo->messageID,tranInfo->status,sender,receiver,AppQueue.GetCount(gor));
		return CT_errorQUEUEFULL;
	case E_TCBPAUSE:
		logfile->printf("Send PacketInfo(Snd:0x%x Rcv:0x%x ID:0x%x ST:%d) from 0x%x to 0x%x Failed! Socket Error\n", 
				tranInfo->sender,tranInfo->receiver,tranInfo->messageID,tranInfo->status,sender,receiver);
		return CT_errorTCBPAUSE;
	default:
		logfile->printf("Send PacketInfo(Snd:0x%x Rcv:0x%x ID:0x%x ST:%d) from 0x%x to 0x%x Failed(0x%x)!\n",
				tranInfo->sender,tranInfo->receiver,tranInfo->messageID,tranInfo->status,sender,receiver,ret);
		return CT_errorFAIL;
	}
}


CTerror TCommManager::GetMessageFromMe( CTuint* sender, CTuint receiver, CTtranInfo* tranInfo )
{
	CTuint station = GETSTATION(receiver);
	CTuint gor = GETMODULE(receiver);


	if(gor>constMAXMODULEGOR || station!=StationNo)
		return CT_errorFAIL;	

	int ret=0;	
	PACKETDATA packet;
	ClientData.SetTM1(gor);
	ret = AppQueue.TryGet(gor, packet );
	ClientData.SetTM2(gor);
	if(ret==E_SUCCESS)
	{
		packet.DePacket(*sender, receiver, *tranInfo);
		logfile->printf("Recv PacketInfo(Snd:0x%x Rcv:0x%x ID:0x%x ST:%d) from 0x%x. QueueCount=%d \n", 
				tranInfo->sender,tranInfo->receiver,tranInfo->messageID,tranInfo->status, *sender,AppQueue.GetCount(gor));
		return CT_errorOK;
	}
	else
		return CT_errorFAIL;
}


void TCommManager::Backup( char* source, char* backup )
{
	ifstream sourcefile( source );
	ofstream backupfile( backup );
	char buf = !EOF;
	while( !sourcefile.eof() && EOF!=buf )
	{
		sourcefile.get(buf);
		backupfile.put(buf);
	}
	backupfile.close();
	sourcefile.close();
}

CTbool TCommManager::Restore( char* backup, char* target )
{
	ifstream backupfile( backup );
	ofstream targetfile( target );
	char buf = !EOF;
	if ( targetfile.good() && backupfile.good() )
	{
		while( backupfile.eof() && EOF!=buf )
		{
			backupfile.get(buf);
			targetfile.put(buf);
		}
		backupfile.close();
		targetfile.close();
		return CT_boolTRUE;
	}
	backupfile.close();
	targetfile.close();
	return CT_boolFALSE;
}


void TCommManager::OnEvtLoadProfile( CTuint sender, CTtranInfo& tranInfo )
{
	PACKETDATA error( CommManagerId, sender, EvtPUBLIC_TELSUN_Error );
	char buf[MAX_PATH+64];
	TKVSet& kvset = tranInfo.GetMessageData();
	char* newfile;

	logfile->printf("Recv ReLoad Configure From 0x%x\n",sender);

	if( CT_errorOK == kvset.GetString( PUBLIC_TELSUN_ProfileName, &newfile ) )
		strcpy( ProfileName, newfile );
		CTbyte flag;
		if( CT_errorOK==tranInfo.GetMessageData().GetByte(PUBLIC_TELSUN_HowToLoadProfile, &flag) )
			switch( flag )
			{
			case Console_LoadAdjacent:
				if( ReadAdjacentStation( ))
					sprintf( buf, "[%lu] LoadProfile Success!", StationNo );
				else
					sprintf( buf, "[%lu] Syntax error in [AdjacentStation]!", StationNo );
				break;
			default:
				sprintf( buf, "[%lu] Unknown flag of PUBLIC_TELSUN_HowToLoadProfile!", StationNo );
				break;
			}
		else
			sprintf( buf, "[%lu] EvtPUBLIC_SUTNEK_LoadProfile missing PUBLIC_TELSUN_HowToLoadProfile!", StationNo );

	error.tranInfo.GetMessageData().PutString(PUBLIC_TELSUN_ErrorMessage,buf);
	SendPacket( error );
}

void TCommManager::OnConsole( CTuint receiver, CTtranInfo& tranInfo )
{
	CTint ival1=-1,ival2=0,cmd_code=0;
	char *sval1=NULL,*sval2=NULL,msg[8096]="", tmpstr1[128],tmpstr2[64];
	STATIONDATA **sdata=NULL;
	int count=0, module, station;

	TKVSet &kvset = tranInfo.GetMessageData();
	kvset.GetInt(MAINT_TELSUN_CommandCode,&cmd_code);
	kvset.GetInt(MAINT_TELSUN_InputInt1,&ival1);
	kvset.GetInt(MAINT_TELSUN_InputInt2,&ival2);
	kvset.GetString(MAINT_TELSUN_InputString1,&sval1);	
	kvset.GetString(MAINT_TELSUN_InputString2,&sval2);

	CTtranInfo aTranInfo(0,0,RespMAINT_TELSUN_Console);
	TKVSet &akvset = aTranInfo.GetMessageData();
	akvset.PutInt(MAINT_TELSUN_ResultStatus,0);

	switch( cmd_code ) // resever
	{
	case 0 :
		akvset.PutString(MAINT_TELSUN_ResultString,
            (char*)"CommMain help\n"
				"cmd 0 : help\n"
				"cmd 1 : list communication information\n"
				"cmd 2 : list module queue length\n"
				"cmd 3 : list all TCB\n"
				"cmd 31 tcbid:  start trace  udp communication from TCBID(tcbid)\n"	
				"cmd 32 tcbid:  stop trace udp communication from TCBID(tcbid)\n"
				"cmd 33:  display trace udp communication information\n"
			);	
		SendMessageToMe(CommManagerId, receiver,&aTranInfo);
		return;

	case 1 :	
		sdata = ClientData.GetStationData();
		for( station=0; sdata && station<constMAXSTATIONNUM; station++ ) 
		{
			if( sdata[station] )
			{
				sprintf( tmpstr1, "%02d) Station(%d,H'%X) Address(%s,%d)\n", 
					count++,
					sdata[station]->GetStation(),sdata[station]->GetStation(),
					inet_ntoa(sdata[station]->GetAddress().addr.sin_addr),
					ntohs(sdata[station]->GetAddress().addr.sin_port));

				strcat( tmpstr1, "    active module(" );
				for( int gor=0; gor<constMAXMODULENUM; gor++ )
					if( sdata[station]->GetGorFromClientData(gor) )
					{
						sprintf( tmpstr2, "%X ", gor) ;
						strcat( tmpstr1, tmpstr2);						
					}
				strcat( tmpstr1, ")\n" );
				strcat( msg, tmpstr1);
			}
		}
		break;

	case 2 :
		for( module=0; module<constMAXMODULENUM; module++ )					 
		{
			 if( AppQueue.IsExist(module ) ) //found
			 {
				 sprintf( tmpstr1, "GOR(%d, H'%X) queue len is %d\n",
					 module, module, AppQueue.GetCount(module));
				strcat( msg, tmpstr1);				 
			 }

		}
		break;

	case 3 :
		UDPServer->TCBInfo(msg, 8096);
		break;

	case 31:
		if( UDPServer->StartTrace(ival1) >= 0 )		
			sprintf( msg, "start trace udp(TCB:%d) communication ok...\ntrace log file  name is tcb%02d.log",
						 ival1, ival1);
		else 
			sprintf( msg, "start trace udp(TCB:%d) communication fail!\n", ival1);		
		break;

	case 32:
		if( UDPServer->StopTrace(ival1) >= 0 )		
			sprintf( msg, "stop trace udp(TCB:%d) communication ok...\n", ival1);
		else 
			sprintf( msg, "stop trace udp(TCB:%d) communication fail!\n", ival1);		
		break;

	case 33 :
		strcpy( msg, "current had been traced TCBID is:");
		UDPServer->TraceInfo(msg+strlen(msg), 4096);
		break;

	default :
		sprintf( msg, "cmd code(%d) invaild.\n", cmd_code);
		break;
	}

	akvset.PutString(MAINT_TELSUN_ResultString,msg);	
	SendMessageToMe(CommManagerId, receiver,&aTranInfo);

}

CTbool TCommManager::AddStation( CTuint stationNo, const char *ip, int port)
{
	CTbool ret ;

	if( !ip )
		return CT_boolFALSE;

	ret = ClientData.AddStation( stationNo, ip, port,NULL );
	logfile->printf("Insert Module %s: StationID[%d] Adddress[%s] Port[%d]\n",ret==CT_boolTRUE?"Ok":"Fail",stationNo,ip,port);

	if( ret == CT_boolTRUE ) 
	{
		SendStationSync(stationNo );
	}

	return ret;
}

CTbool TCommManager::ReloadProfile()
{
	ReadAdjacentStation();
	SendAllStationSync();
	return CT_boolTRUE;
}

int TCommManager::strparse(const char* parameter, int index, const char* separator, char* section)
{
	char temp[4096+1];
	char* p;
	size_t len;

	strcpy(temp, parameter);
	p = temp;
	*section = '\0';

	int i = 1;
	while (*p)
	{
		len = strcspn(p, separator);
		if ( i == index )
		{
			strncpy(section, p, len);
			section[len] = '\0';
		}
		p += len;
		if (*p)
			p++;
		i++;
	}
	return i-1;
}

char* TCommManager::strtrim(char *p)
{
    char *pstr=p;
    if(!p)
        return NULL;
    if(strlen(p)<1)
        return p;
    int i=0,len=0;
    while(pstr[i]==' ')
    {
        i++;
    }
	len = strlen(pstr+i);
    memmove(p,(pstr+i),len);
    len = strlen(p)- 1;
    while(len>=0 && p[len]==' ')
    {
        p[len] = '\0';
        len--;
    }
    return p;
}

DWORD WINAPI doRunPoll( void* lpParm )
{
	TCommManager * p = (TCommManager*)lpParm ;
	if(p)
		p->DoPoll();
	return 0 ;
}

