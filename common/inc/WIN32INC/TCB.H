//************************************************
// Module : TUDP Communication Class Head File.
// Time   : 1998/10/28
// System : Windiws NT With Visual C++
// AT WorkGroup (R) CopyRight (C) By : Mr. HYY.
// Update History:
//			Updated 2000-08-16 HYY
//************************************************
#ifndef __TUDPTRANSFERFRAME_H__
#define __TUDPTRANSFERFRAME_H__

#define MAXWINSIZE  0x0100
#define MAXSENDSIZE 0x0100
#define MAXRECVSIZE 0x0100
#define SENDWINSIZE 0x0080
#define RECVWINSIZE 0x0080
#define WINLOOKBACK 0x0080
#define UBUFFERSIZE 0x2000

#define stReady       0x01
#define stSynchronize 0x02
#define stWork        0x03
#define stReset       0x04
#define stResetPaulse 0x05
#define stPool        0x06
#define stPaulse      0x07

#define pmACK         0x80
#define pcDATA        0x10
#define pcCONTROL     0x20
#define pcTIMER       0x30

#define ptREJ         0x01
#define ptSYN         0x04
#define ptPOOL		  0x06
#define ptRST         0x07

#define MAX_TCBTIMER  0x05
#define TM_S          0x00
#define TM_W          0x01
#define TM_B          0x02
#define TM_R          0x03
#define TM_P          0x04

#define TM_STIMER     8*100
#define TM_WTIMER     6*100
#define TM_BTIMER     1*100
#define TM_RTIMER     8*100
#define TM_PTIMER     20*100
#define RE_SYN        0x08
#define RE_SEND       0x08
#define RE_RESET      0x08
#define RE_POOL       0x08

#define TIMERMASK     0x07

#define MINSEQUENCE 0x00000000L
#define MAXSEQUENCE 0xFFFFFFFFL

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <fstream.h>
#include "ctmsg.h"
#include "ctstatus.h"
#include "Udp.h"
#include "ClientQueue.h"
#include "tlog.h"

#ifndef SocketAddr
typedef struct sockaddr_in SocketAddr;
#endif
CTuint NEXT(CTuint SEQ);
CTuint PREV(CTuint SEQ);
CTuint NEXTN(CTuint SEQ,CTuint N);
CTuint PREVN(CTuint SEQ,CTuint N);
int BETWEEN(CTuint BASE,CTuint SIZE,CTuint SEQ);

class TUDPWindow
{
private:
	CTushort MaxWin;
	CTushort WinSize;
public:
	CTuint WinHead;
	CTuint WinSeq;
	TRANSFRAME Window[MAXWINSIZE];
	CTbyte     Occupy[MAXWINSIZE];
	TRANSFRAME* WinFrame(CTuint Seq);
	int WinOffset(CTuint Seq);
	void SaveFrame(TRANSFRAME* Frame);
	void DelFrame(CTuint Seq);
	void Reset();
	int WinCount();
	int WinRemain();
	TUDPWindow(CTuint winsize);
	~TUDPWindow();
};

class CTUDPTimer;
class TUDPServer;
class TTCB
{
protected:
	TUDPWindow* SendWindow;
	TUDPWindow* RecvWindow;

	PEER SourAddress;
	PEER DestAddress;
	CTint State;
	int Valid;

	TPrintLog Trace ;

	CTAppQueue* RecvQueue;	

	CTUDPTimer* UDPTimer;
	TUDPSendSocket* SendSocket;
	CTuint TCBID;
	CTuint TimerID[MAX_TCBTIMER+1];

	void ClearTimerID(CTushort type);
	int SendFrame(CTbyte type,CTuint seq,CTuint ack,
		CTushort window,PACKETDATA* Packet=NULL);
	int SendFrame(TRANSFRAME& Frame);
	int SavePacket(TRANSFRAME* Frame);

public:
	TTCB::TTCB(CTAppQueue *r,CTUDPTimer *t,TUDPSendSocket *s,
		CTuint id,const PEER &f,const PEER &l);

	void   HandleEvent(TRANSFRAME& event);
	int    SendPacket(const PACKETDATA& Packet);
	int    TCBValid()const{ return Valid == 1 ? 1: 0; }
	int    Resume();
	CTuint getHandle()const {return TCBID;}
	CTint  getState() const {return State;}
	
	CTint  isMember(SocketAddr address);
	CTint  isMember(CTuint handle);
	void   TCBInfo(char *buffer);

	void StartTrace();
	void StopTrace();
	CTbool isTrace(){ return (Trace.getTraceMode() == LOG_TO_NULL? CT_boolFALSE:CT_boolTRUE);}

	TTCB *prev,*next;
	virtual ~TTCB();
	friend class TUDPServer;

private:
	int MaxSynchronizeTimes;
	int MaxSendTimes;
	int MaxResetTimes;
	int MaxPoolTimes;
	void SetTimer(CTushort type,CTuint ticks);
	void DelTimer(CTushort type);
	int IsSetTimer(int type);

	void DoSetSTimer(){ SetTimer(TM_S,TM_STIMER); }
	void DoDelSTimer(){ DelTimer(TM_S); }

	void DoSetWTimer(){ SetTimer(TM_W,TM_WTIMER); }
	void DoDelWTimer(){ DelTimer(TM_W); }

	void DoSetBTimer(){ SetTimer(TM_B,TM_BTIMER); }
	void DoDelBTimer(){ DelTimer(TM_B); }

	void DoSetRTimer(){ SetTimer(TM_R,TM_RTIMER); }
	void DoDelRTimer(){ DelTimer(TM_R); }

	void DoSetPTimer(){ SetTimer(TM_P,TM_PTIMER); }
	void DoDelPTimer(){ DelTimer(TM_P); }

	void DoSYN();
	void DoACKSYN();
	void DoCheckSYN();
	void DoCheckPool();
	void DoCheckReset();

	void DoReceiveFrame(TRANSFRAME* Frame);
	void DoACK(TRANSFRAME* Frame);
	void DoREJ(TRANSFRAME* Frame);
	void DoSendRST();
	void DoRST();
	void DoPOOL();
	void DoACKPOOL();
	void DoReSend();
	void DoSendACK();
	void DoSendREJ();
	void DoReActive();
	void DoResetAll();
	void DoGameOver();
};

#endif
