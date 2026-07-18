#ifndef __TCPCLIENT_H__
#define __TCPCLIENT_H__

#define PROXYPORT_DEFAULT    8085
#define MAX_TCP_LEN          2048
#define MAXSTATIONCOUNT      1024
#define TCPHEADERSIZE		   24
const   unsigned char		  TCPHEAD0=(unsigned char)0xff;
const   unsigned char		  TCPHEAD1=(unsigned char)0xee;
typedef struct CPacketHead
{
	unsigned long		TotalLength;
	unsigned long		CommmandID;
	unsigned long		SequenceID;
	unsigned long		Sender;
	unsigned long		Receiver;
	unsigned long		Status;
	CPacketHead()
	{
		TotalLength=0;CommmandID=0;
		SequenceID=0;Sender=0;
		Receiver=0;Status=0;
	}
}TagPacketHead;
//=========================================================================================
class TTCPClient
{
private:
	char        mIP[32];
	int         mPort,mID;
	char		m_buf[MAX_TCP_LEN];
	int         mStationUsed[MAXSTATIONCOUNT];  //为了实现快速判断，需要代理的站点标志数组
	int         m_iWaitRet ;
	BOOL        m_bWaitRet ;
	BOOL        m_RunFlag , m_StopFlag;
	SOCKET      m_fd;
	TPrintLog*	logfile;
	CTAppQueue  *theAppQueue;
	CTThread    *theRecThread ; //pool theNetWQ, put packet to recvQueue!
	unsigned long StationNo;
	char		profile[128];

	void   setIOMode(int iNonblocking);
	int    OpenSocket(char *ip, short Port, int iTimeout);
    void   CloseSocket();
	CTbool HdlRcvPacket(LPCTSTR rcvBuf,int rcvLen);
public:
	TTCPClient(char *profile,CTAppQueue *queue);
	~TTCPClient(void);

    int    sendBlock(char *buf, int iSize,BOOL bWait=FALSE);

	CTbool Run();//must called the function to active the thread!!
	VOID   Stop();
	void   DoPacket();
	int    SendPacket(PACKETDATA & data);

	int  SendRegisterBlock();
	bool bRegistered;
	int  mWorkMode;  //=0--direct pass,1--dynatic all stationNo;
	bool IsProxyStation(unsigned long station);

public: //一个包包含多个子包
    int  mTcp_CurrentLen;
    int  mTcp_NeedLen;
    char mTcp_Bufffer[MAX_TCP_LEN];
};

#ifdef UNICODE
	#define  _tstoi    _wtoi
#else
	#define  _tstoi    atoi
#endif

#endif