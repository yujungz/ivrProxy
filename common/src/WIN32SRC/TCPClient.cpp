#include "CtPacket.h"
#include "ClientQueue.h"
#include "Tlog.h"
#include "TCPClient.h"
#include "CtMsg.h"
#include "ctutil.h"
#include <tchar.h>
#include <direct.h>

#define E_TCP_ERROR		-1002		// TCP错误
#define E_CREATE_FILE	-1003		// 在发送消息时打开数据文件出错,或在接收消息时,创建数据文件出错
#define E_TIMEOUT		-1004		// 操作超时

#define YES		0
#define NO		-1

#define TCPBufLen_Max	1024*8

#ifdef _WIN32
	#define bzero(p, size)	memset(p, 0, size)
	#define ioctl(s, p, v)	ioctlsocket(s,p,(unsigned long*)v)
	#define ERRNO	WSAGetLastError()
	#define EINPROGRESS2	WSAEWOULDBLOCK
	typedef int socklen_t;
#endif

#ifndef SEND_ONCE
    #define SEND_ONCE(x) ((x) > 1024 ? 1024 : (x))
#endif

TTCPClient::TTCPClient(char *profilename,CTAppQueue *queue)
    : theAppQueue(queue)
{
	int i=0,j=0;
	for(i=0;i<MAXSTATIONCOUNT;i++)
		mStationUsed[i]=0;

	strcpy(profile,profilename);
	bRegistered = false;
	m_fd = INVALID_SOCKET;
	mWorkMode = ::GetPrivateProfileInt("TcpProxy","WorkMode",0,profile);

	char comm_item[128]="",str[8]="";
	memset(comm_item,0,128);
	memset(str,0,8);
	GetPrivateProfileString("LOCALCONFIG","Module","",comm_item,127,profile);
	char *p = strstr(comm_item,",");
	if(p)
	{
		memcpy(str,comm_item,p-comm_item);
		StationNo = atoi(str);
	}
	else
		StationNo = 99;

    mTcp_CurrentLen=0;
	mTcp_NeedLen=0;
	memset(mTcp_Bufffer,0,MAX_TCP_LEN);
	mID=0;
	memset(m_buf,0,MAX_TCP_LEN);
	m_iWaitRet = 0;
	m_bWaitRet=false;
	m_RunFlag=false;
	m_StopFlag = false;

	memset(mIP,0,32);
    ::GetPrivateProfileString("TcpProxy","TcpAddr","127.0.0.1",mIP,30,profile);
	mPort=::GetPrivateProfileInt("TcpProxy","TcpPort",8005,profile);

	//分析代理站点范围
	char buf[256]="",str1[64]="",str2[64]="";
    ::GetPrivateProfileString("TcpProxy","ModuleRange","",buf,255,profile);
	if(strlen(buf)>0)
	{
		char *p1=buf;
		char *p2=strstr(p1,",");
		while(p2)
		{
			memset(str1,0,64);
			memcpy(str1,p1,p2-p1);

			char *p3=strstr(str1,"-");
			if(p3)
			{
				memset(str2,0,64);
				memcpy(str2,str1,p3-str1);
				i = atoi(str2);
				j = atoi(p3+1);
				for(int m=i;m<j;m++)
				{
					if(m<0)
						continue;
					else if(m>=MAXSTATIONCOUNT)
						break;
					else
						mStationUsed[m] = 1;
				}
			}
			else
			{
				i = atoi(str1);
				if(i>=0 && i<MAXSTATIONCOUNT)
					mStationUsed[i] = 1;
			}

			p1 = p2+1;
			p2 = strstr(p1,",");
		}
		if(p1 && strlen(p1)>0)
		{
			char *p2=strstr(p1,"-");
			if(p2)
			{
				memset(str1,0,64);
				memcpy(str1,p1,p2-p1);
				i = atoi(str1);
				j = atoi(p2+1);
				for(int m=i;m<j;m++)
				{
					if(m<0)
						continue;
					else if(m>=MAXSTATIONCOUNT)
						break;
					else
						mStationUsed[m] = 1;
				}
			}
			else
			{
				i = atoi(p1);
				if(i>=0 && i<MAXSTATIONCOUNT)
					mStationUsed[i] = 1;
			}
		}
	}
    mkdir("log");
	logfile = new TPrintLog(".\\log\\TcpClient.log", "" ) ;
	assert(logfile);
	logfile->setTraceMode( LOG_TO_FILE ) ;
	logfile->printf("TTCPClient...\n");
}

TTCPClient::~TTCPClient(void)
{
	Stop();
	if(logfile)
	{
    	logfile->printf(_T("~TTCPClient!\n"));
		delete logfile; logfile = NULL ;
	}
}

VOID  TTCPClient::Stop()
{
	if(m_StopFlag)
		return;
    this->CloseSocket();
	m_RunFlag  = FALSE ;
	DWORD tickNow = GetTickCount();
	while(!m_StopFlag && GetTickCount() - tickNow<(DWORD)10*1000 ){
	    Sleep(5);
	}
	delete theRecThread; theRecThread = NULL ;

}

inline void TTCPClient::setIOMode(int iNonblocking)
{
	assert(m_fd != INVALID_SOCKET);
	ioctl(m_fd, FIONBIO, &iNonblocking);
}

DWORD WINAPI Do_RecvFromProxy(LPVOID arg)
{
	assert(arg);
	((TTCPClient *)arg)->DoPacket();
	return 0;
}

CTbool TTCPClient::Run()
{         
	int run_count=0;
	theRecThread = new CTThread(Do_RecvFromProxy,this,CREATE_SUSPENDED);
	assert(theRecThread);

	if((DWORD)~0 == theRecThread->Continue())
	{
		logfile->printf(_T("Create Send Thread fail!\n"));
		return CT_boolFALSE;
	}
	else
		m_StopFlag = false;

	while(!bRegistered)
	{
		Sleep(10);
		run_count++;
		if(run_count>10000)
		{
			logfile->printf(_T("Receive Resp Registered Timeout!\n"));
			return CT_boolFALSE;
		}
	}
	return CT_boolTRUE;
}

void TTCPClient::DoPacket()
{
	m_RunFlag = TRUE ;
	char rcvbuf[TCPBufLen_Max]={0};
	int iRet;
	BOOL bFirst = TRUE ;
	PACKETDATA  Data;

	while(m_RunFlag==TRUE){
		Sleep(1);
		if (m_fd == INVALID_SOCKET){
			if(YES == OpenSocket(mIP, (short)mPort,1))
			{
				if(bFirst)
				{
					logfile->printf(_T("connect Socket succ!\n"));
					bFirst = FALSE ;
				}
				else
				{
				    logfile->printf(_T("reconnect Socket succ!\n"));
				}
				//发COMMAIN同步包
				//PACKETDATA packet(m_id,0,EvtComm_TELSUN_SynStation);
				//SendPacket(packet);
			}
			else
			{
				//Sleep(100);
				Sleep(1000*5);  //停五秒重连
				continue ;
			}
		}
		iRet =  recv(m_fd, (char*)rcvbuf, TCPBufLen_Max, 0);
        if(iRet == NO )
		{
			Sleep(1000);
			logfile->printf(_T("Socket connect is fail!\n"));
			CloseSocket() ;
			continue ;
		}
        else if(iRet == YES)
		{
			Sleep(1000);
			logfile->printf(_T("Socket connect is broked,system is reconnect now,please wait...\n"));
			CloseSocket() ;
			continue ;
		}
        //HdlRcvPacket(rcvbuf,iRet);
    	//解包,判断是否有残余包
        if(mTcp_NeedLen>0)
        {
            //logfile->writeB(1,rcvbuf,iRet);
    		int jj=0,nUseDataLen=0;
	    	for(jj=0;jj<iRet;jj++)
		    {
			    if((unsigned char)rcvbuf[jj]==TCPHEAD0)
    			{
	    			if(iRet>(jj+1))
		    		{
			    		if((unsigned char)rcvbuf[jj+1] ==TCPHEAD1)
				    		break;
    				}
	    		}
		    }
		    if(jj>=iRet)
		    {
    			if(jj==mTcp_NeedLen)
	    		{
		    		//logfile->printf("Handle PreSave Data(Length=%d) When Recv Info %d Bytes.\n",mTcp_CurrentLen,mTcp_NeedLen);
                    memcpy(mTcp_Bufffer+mTcp_CurrentLen,rcvbuf,jj);
                    nUseDataLen = mTcp_CurrentLen+jj;
                    mTcp_NeedLen = 0;
                    mTcp_CurrentLen = 0;
                    HdlRcvPacket(mTcp_Bufffer,nUseDataLen);
                    memset(mTcp_Bufffer,0,MAX_TCP_LEN);
		    		continue;
			    }
    			else
	    		{
		    		logfile->printf("Can'f Find Packet Header Marked(PreSaveBuf) When Recv %d Bytes. Saved Data(Current=%d,Need=%d)\n",jj,mTcp_CurrentLen,mTcp_NeedLen);
                    continue;
    			}
	    	}

    		if(jj!=mTcp_NeedLen)
	    	{
		    	logfile->printf("GiveUp %d Bytes(Need %d) for PrePacket When Recv Data.\n",jj,mTcp_NeedLen);
                memset(mTcp_Bufffer,0,MAX_TCP_LEN);
                mTcp_NeedLen = 0;
                mTcp_CurrentLen = 0;

                if(iRet>jj)
                    HdlRcvPacket(rcvbuf+jj,iRet-jj);
		    }
    		else
	    	{
		    	//logfile->printf("Handle PreSave Data(Length=%d) When Recv %d Bytes Data.\n",mTcp_CurrentLen,mTcp_NeedLen);
			    //处理之前残余包
    			memcpy(mTcp_Bufffer+mTcp_CurrentLen,rcvbuf,jj);
                nUseDataLen = mTcp_CurrentLen+jj;
                mTcp_NeedLen = 0;
                mTcp_CurrentLen = 0;
	    		HdlRcvPacket(mTcp_Bufffer,nUseDataLen);
                memset(mTcp_Bufffer,0,MAX_TCP_LEN);

		    	//处理后余数据包
                if(iRet>jj)
    			    HdlRcvPacket(rcvbuf+jj,iRet-jj);
    		}
        }
        else
        {
            //logfile->writeB(1,rcvbuf,iRet);
            HdlRcvPacket(rcvbuf,iRet);
        }
	}
	m_StopFlag = TRUE ;
}

//包头加数据长度
CTbool TTCPClient::HdlRcvPacket(LPCTSTR rcvBuf,int rcvLen)
{
    int jj=0,nUseDataLen=0;
    for(jj=0;jj<rcvLen;jj++)
   	{
    	if((unsigned char)rcvBuf[jj]==TCPHEAD0)
	    {
		    if(rcvLen>(jj+1))
   			{
    			if((unsigned char)rcvBuf[jj+1] ==TCPHEAD1)
	    			break;
		    }
   		}
    }
   	if(jj>=rcvLen)
    {
        if(mTcp_CurrentLen>0 && rcvLen== mTcp_NeedLen)
        {
    	    //处理之前残余包
	    	//logfile->printf("HdlRcvPacket==>PreSave Data(Length=%d) When Handle %d Bytes Data.\n",mTcp_CurrentLen,mTcp_NeedLen);
   			memcpy(mTcp_Bufffer+mTcp_CurrentLen,rcvBuf,rcvLen);
            nUseDataLen = mTcp_CurrentLen+rcvLen;
            mTcp_NeedLen = 0;
            mTcp_CurrentLen = 0;
    		HdlRcvPacket(mTcp_Bufffer,nUseDataLen);
            memset(mTcp_Bufffer,0,MAX_TCP_LEN);
      	    return CT_boolTRUE ;
        }
        else if(mTcp_CurrentLen>0 && rcvLen<mTcp_NeedLen)
        {
            logfile->printf("HdlRcvPacket==>Data Length Too Small. Save Next.PreData[Cur=%d,Need=%d]. Get %d Bytes.\n",mTcp_CurrentLen,mTcp_NeedLen,rcvLen);
   			memcpy(mTcp_Bufffer+mTcp_CurrentLen,rcvBuf,rcvLen);
            mTcp_CurrentLen += rcvLen;
            mTcp_NeedLen = mTcp_NeedLen -rcvLen;
	        return CT_boolFALSE ;
        }
        else
        {
            logfile->printf("HdlRcvPacket==>Receive Packet Format Error Len(%d). Not Find Head Mark(0xFFEE). \n",rcvLen);
	        return CT_boolFALSE ;
        }
    }
    else
    {
        if(mTcp_CurrentLen>0 && jj== mTcp_NeedLen)
        {
    	    //处理之前残余包
	    	//logfile->printf("HdlRcvPacket==>PreSave Data(Length=%d) When Handle %d Bytes Data.\n",mTcp_CurrentLen,mTcp_NeedLen);
   			memcpy(mTcp_Bufffer+mTcp_CurrentLen,rcvBuf,jj);
            nUseDataLen = mTcp_CurrentLen+rcvLen;
            mTcp_NeedLen = 0;
            mTcp_CurrentLen = 0;
    		HdlRcvPacket(mTcp_Bufffer,nUseDataLen);
            memset(mTcp_Bufffer,0,MAX_TCP_LEN);

    		HdlRcvPacket(rcvBuf+jj,rcvLen-jj);
      	    return CT_boolTRUE ;
        }
        else if(mTcp_CurrentLen>0)
        {
            logfile->printf("HdlRcvPacket==>PreSave Data Failed. CurrentCount=%d,NeedCount=%d,Get %d Bytes. Ignore Current Data And Continue.\n",mTcp_CurrentLen,mTcp_NeedLen,jj);
            memset(mTcp_Bufffer,0,MAX_TCP_LEN);
            mTcp_NeedLen = 0;
            mTcp_CurrentLen = 0;

    		HdlRcvPacket(rcvBuf+jj,rcvLen-jj);
     	    return CT_boolTRUE ;
        }
        else
        {
            unsigned long mdatalen=0;
 	        CTbyte2CTuint(rcvBuf,&mdatalen);
   	        mdatalen = mdatalen & 0x0000ffff; //注意！！！
            if(rcvLen< mdatalen)
            {
                memset(mTcp_Bufffer,0,MAX_TCP_LEN);
                mTcp_CurrentLen = rcvLen;
                mTcp_NeedLen =  mdatalen -  rcvLen;
                memcpy(mTcp_Bufffer,rcvBuf,rcvLen);
    	        logfile->printf("HdlRcvPacket==>Receive Packet And Save %d Bytes Data. Need %d Bytes Next\n",rcvLen,mTcp_NeedLen);
    	        return CT_boolFALSE ;
            }

		    TagPacketHead mHead;
			char mybuf[64]="";
			memset(mybuf,0,64);
			memcpy(mybuf,rcvBuf,TCPHEADERSIZE);
			char *p=mybuf;
			p += sizeof(UINT);  //跳过TotalLength
			CTbyte2CTuint(p,&mHead.CommmandID);
			p += sizeof(UINT);  //跳过SequenceID
			p += sizeof(UINT);
			CTbyte2CTuint(p,&mHead.Sender);   
			if(mHead.CommmandID == 1)
			{
				if(mWorkMode==1)  //属于随机申请站点号
				{
					if(StationNo!=mHead.Sender)
					{
						StationNo = mHead.Sender;  //申请到的新站点号
						char get_buf[128]="",get_buf1[128]="";
						memset(get_buf,0,128);
						memset(get_buf1,0,128);
						::GetPrivateProfileString("LOCALCONFIG","Module","",get_buf,120,profile);
						if(strlen(get_buf)>0)
						{
							char *p = strstr(get_buf,",");
							if(p)
							{
								sprintf(get_buf1,"%d",mHead.Sender);
								strcat(get_buf1,p);
								::WritePrivateProfileString("LOCALCONFIG","Module",get_buf1,profile);
							}
							else
								::WritePrivateProfileString("LOCALCONFIG","Module","11,3000,1,127.0.0.1,Log\\network.log,",profile);
						}
						else
							::WritePrivateProfileString("LOCALCONFIG","Module","11,3000,1,127.0.0.1,Log\\network.log,",profile);
					}
				}
				bRegistered = true;

	            if(rcvLen - TCPHEADERSIZE>0){
		            HdlRcvPacket(rcvBuf + TCPHEADERSIZE , rcvLen - TCPHEADERSIZE );
			   	}
        		return CT_boolTRUE;
			}
			
        	if(rcvLen<constMINPACKETSIZE+TCPHEADERSIZE)
            {
   		        logfile->printf("HdlRcvPacket==>Packet Format Error. Packet Len(%d) Too Small(<%d).\n",rcvLen,(constMINPACKETSIZE+TCPHEADERSIZE));
            	return CT_boolFALSE ;
            }
			
			PACKETDATA  Data;
          	CTushort len=0;
            const int lenUS = sizeof(CTushort) ;
           	//取第一个位置的包的长度
            char str[3];
           	memset(str,0,3);
            memmove(str,rcvBuf+TCPHEADERSIZE,2);
           	CTushort_byte val(str);
            len = val.value;
           	if(len<constMINPACKETSIZE)
            {
   		        logfile->printf("HdlRcvPacket==>Packet Format Error. Packet Len(%d). Get %d Bytes.\n",len,rcvLen);
            	return CT_boolFALSE ;
           	}
            memset(m_buf,0,MAX_TCP_LEN);
           	if(len>=MAX_TCP_LEN)
    	        len=MAX_TCP_LEN-1;
            memmove(m_buf,rcvBuf+TCPHEADERSIZE+2,len);  //去掉TCP/IP的包头

            int ret = (int)Data.fromString(m_buf,constMAXPACKETLEN);
           	if(ret == -1)
            {
	            logfile->printf(_T("HdlRcvPacket==>解包错误！\n"));
           		return CT_boolFALSE ;
            }
           	theAppQueue->Put(GETMODULE(Data.receiver),Data);
            if(rcvLen - lenUS - len - TCPHEADERSIZE>0){
	            HdlRcvPacket(rcvBuf + lenUS  + len + TCPHEADERSIZE , rcvLen - lenUS - len - TCPHEADERSIZE );
           	}
        	return CT_boolTRUE;
        }
    }
}

int TTCPClient::OpenSocket(char *ip, short sPort, int iTimeout)
{
	int    iError=0;
	struct sockaddr_in addr;
	struct in_addr inaddr;
	fd_set wfds;
	struct timeval tv;
	int iSoError;
	socklen_t iSoLen;

	assert(iTimeout > 0);
	inaddr.S_un.S_addr = inet_addr(mIP);
	m_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_fd == INVALID_SOCKET) return E_TCP_ERROR;

	bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inaddr.s_addr;
	addr.sin_port = htons(sPort);
	setIOMode(1);
	if (connect(m_fd, (struct sockaddr*)&addr, sizeof(addr)) == 0) return 0;
	if (ERRNO != EINPROGRESS2) goto _fail;

	FD_ZERO(&wfds);
	FD_SET(m_fd, &wfds);
	tv.tv_usec = 0; tv.tv_sec = iTimeout;
	if ((iError = select((int)(m_fd + 1), NULL, &wfds, NULL, &tv)) == 0)    //yujun change
	{
		iError = E_TIMEOUT; 
		goto _fail;
	}
	// In spite that m_fd is writable, we couldn't
	// tell if m_fd is connected because an error 
	// maybe occured.
	iSoLen = sizeof(int);
	getsockopt(m_fd, SOL_SOCKET, SO_ERROR, (char*)&iSoError, &iSoLen);
	if (iSoError != 0) 
	{
		#ifndef _WIN32
			// Set errno so that SYSERR can report the correct error
			errno = iSoError;
		#endif
		goto _fail;
	}

	if(m_fd == INVALID_SOCKET)
	{
		iError = -1;
		goto _fail;
	}
	setIOMode(0);

	//发送登记包
	SendRegisterBlock();
	return YES;

_fail:
	closesocket(m_fd);
	m_fd = INVALID_SOCKET;
	return iError;
}

void TTCPClient::CloseSocket()
{
	if(INVALID_SOCKET!=m_fd && NULL!=m_fd){
		::closesocket(m_fd);
		m_fd = INVALID_SOCKET;
	}
}

int TTCPClient::sendBlock(char *buf, int iSize,BOOL bWait)
{
	if(m_fd==INVALID_SOCKET)
		return SOCKET_ERROR;
    int iHaveSent, iSendOnce;
    for (iHaveSent = 0, iSendOnce = 0;  iHaveSent < iSize; iHaveSent += iSendOnce)
    {
	    iSendOnce = ::send(m_fd, buf + iHaveSent, SEND_ONCE(iSize - iHaveSent), 0);
        if (iSendOnce == SOCKET_ERROR) return SOCKET_ERROR;
    }
    return E_SUCCESS ;
}

int TTCPClient::SendPacket(PACKETDATA & data)
{
    TagPacketHead mHead;
    CTuint StationNo=GETSTATION(data.receiver);

	mHead.CommmandID = 0; //!!!!!!!
	mHead.Status = 0;

	const int lenUS = sizeof(CTushort) ;
	TCHAR tBuf1[constMAXPACKETLEN+lenUS+1] = {0};
	TCHAR tBuf2[constMAXPACKETLEN+lenUS+1] = {0};
	int lenPacket = data.toString(tBuf1+lenUS,constMAXPACKETLEN);
	tBuf1[0] = ((char*)&lenPacket)[0] ;
	tBuf1[1] = ((char*)&lenPacket)[1] ;
    mHead.TotalLength = lenPacket + lenUS + TCPHEADERSIZE;

    char *p =tBuf2;
	CTuint2CTbyte(mHead.TotalLength,p);
	p += sizeof(UINT);
	CTuint2CTbyte(mHead.CommmandID,p);
	p += sizeof(UINT);
	CTuint2CTbyte(mHead.SequenceID,p);
	p += sizeof(UINT);
	CTuint2CTbyte(mHead.Sender,p);
	p += sizeof(UINT);
	CTuint2CTbyte(mHead.Receiver,p);
	p += sizeof(UINT);
	CTuint2CTbyte(mHead.Status,p);
	p += sizeof(UINT);
    memcpy(p,tBuf1,lenPacket+lenUS);
	//修改包头两个字节
	tBuf2[0]=TCPHEAD0;
	tBuf2[1]=TCPHEAD1;

	return  sendBlock(tBuf2,lenUS+lenPacket+TCPHEADERSIZE,TRUE) ;
}

int TTCPClient::SendRegisterBlock()
{
    TagPacketHead mHead;
	TCHAR tBuf2[TCPHEADERSIZE+1] = {0};
    mHead.TotalLength = TCPHEADERSIZE;
	mHead.Sender = StationNo;
	mHead.CommmandID = 1;
	mHead.SequenceID = 1;
	if(mWorkMode==1) 
	{
		if(bRegistered)
			mHead.Status = 2;  //已经申请过站点号，维持原先的站点号
		else
			mHead.Status = 1;  //!!!!!!随机申请新站点号
	}
	else
		mHead.Status = 0;

    char *p =tBuf2;
	CTuint2CTbyte(mHead.TotalLength,p);
	p += sizeof(UINT);
	CTuint2CTbyte(mHead.CommmandID,p);
	p += sizeof(UINT);
	CTuint2CTbyte(mHead.SequenceID,p);
	p += sizeof(UINT);
	CTuint2CTbyte(mHead.Sender,p);
	p += sizeof(UINT);
	CTuint2CTbyte(mHead.Receiver,p);
	p += sizeof(UINT);
	CTuint2CTbyte(mHead.Status,p);
	p += sizeof(UINT);
 	//修改包头两个字节
	tBuf2[0]=TCPHEAD0;
	tBuf2[1]=TCPHEAD1;

	return  sendBlock(tBuf2,TCPHEADERSIZE,TRUE) ;
}

bool TTCPClient::IsProxyStation(unsigned long station)
{
	if(station>=MAXSTATIONCOUNT || station<0)
		return false;
	else
	{
		if(mStationUsed[station]==1)
			return true;
		else
			return false;
	}
}