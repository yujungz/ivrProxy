//************************************************
// Module : TUDP Communication Class Head File.
// Time   : 1998/10/28
// System : Windiws NT With Visual C++
// AT WorkGroup (R) CopyRight (C) By : Mr. HYY.
// Update History:
//			Updated 2000-08-16 HYY
//************************************************
//#define _TRACE_

#ifdef _TRACE_
#include "fstream.h"
ofstream TRACE("TRACE.LOG");
#endif

#include <stdio.h>

#include <fstream.h>
#include <assert.h>
#include <stdarg.h>


#include "TCB.h"
#include "ctstatus.h"
#include "Timer.h"

extern char* getmyipaddr(void);

TUDPWindow::TUDPWindow(CTuint winsize)
{
	WinHead = WinSeq = MINSEQUENCE;
	MaxWin = MAXWINSIZE;
	WinSize = (unsigned short)winsize;
	for(CTushort i=0;i<MaxWin;i++)
		Occupy[i] = 0;
}

TRANSFRAME* TUDPWindow::WinFrame(CTuint Seq)
{
	CTushort Index =(CTushort)Seq%MaxWin;
	if(Occupy[Index]==0)
		return NULL;
	return &Window[Index];
}

int TUDPWindow::WinOffset(CTuint Seq)
{
	return BETWEEN(WinHead,WinSize,Seq);
}

void TUDPWindow::SaveFrame(TRANSFRAME* Frame)
{
	if(!Frame)
		return;

	CTushort Index = ((CTushort)Frame->Seq)%MaxWin;
	Window[Index] = *Frame;
	Occupy[Index] = 1;
}

void TUDPWindow::DelFrame(CTuint Seq)
{
	CTushort Index = (CTushort)Seq%MaxWin;
	Occupy[Index] = 0;
}

int TUDPWindow::WinCount()
{
	int Temp = BETWEEN(WinHead,(CTuint)(WinSize+1),WinSeq);
	return Temp;
}

int TUDPWindow::WinRemain()
{
	int Temp = (int)(WinSize-BETWEEN(WinHead,(CTuint)(WinSize+1),WinSeq));
	return Temp;
}

void TUDPWindow::Reset()
{
	for(CTushort i=0;i<MaxWin;i++)
		Occupy[i] = 0;
	WinHead = WinSeq = MINSEQUENCE;
}

TUDPWindow::~TUDPWindow()
{
}

void TTCB::SetTimer(CTushort type,CTuint ticks)
{
	if(!UDPTimer || type >= MAX_TCBTIMER)
		return ;
	CTuint i;
	if(TimerID[type])
		UDPTimer->timerClear(TimerID[type]);
	i = UDPTimer->timerSet(TCBID,type,ticks);
	TimerID[type] = i;
}

void TTCB::DelTimer(CTushort type)
{
	if(!UDPTimer || type >= MAX_TCBTIMER)
		return ;

	if(IsSetTimer(type))
		UDPTimer->timerClear(TimerID[type]);
	TimerID[type] = 0;
}

void TTCB::ClearTimerID(CTushort type)
{
	if(!UDPTimer || type >= MAX_TCBTIMER)
		return ;
	TimerID[type] = 0;
}

int TTCB::IsSetTimer(int type)
{
	if(!UDPTimer || type >= MAX_TCBTIMER)
		return 0;

	return ( TimerID[type]? 1 : 0 );
}

TTCB::TTCB(CTAppQueue *r,CTUDPTimer *t,TUDPSendSocket *s,CTuint id,const PEER &f,const PEER &l)
	: DestAddress(f),SourAddress(l),
	  TCBID(id),UDPTimer(t),
	  SendSocket(s),RecvQueue(r),
	  prev(0),next(0)
{
	assert(r && t && s);
	SourAddress.addr.sin_family = AF_INET;
	DestAddress.addr.sin_family = AF_INET;

	State = stReady;
	Valid = 1;
	MaxSynchronizeTimes = RE_SYN;
	MaxSendTimes = RE_SEND;
	MaxResetTimes = RE_RESET;
	MaxPoolTimes = RE_POOL;

	SendWindow = new TUDPWindow(SENDWINSIZE);
	assert(SendWindow);
	RecvWindow = new TUDPWindow(RECVWINSIZE);
	assert(RecvWindow);

	for(int i = 0;i<MAX_TCBTIMER+1;i++)
		TimerID[i] = 0;

	char fn[80], path[80]="";
	sprintf( fn, "tcb%02d.log" ,TCBID ) ;
	Trace.Open( fn, path, LOG_TO_NULL);
}

TTCB::~TTCB()
{
	if(SendWindow)
		delete SendWindow;
	if(RecvWindow)
		delete RecvWindow;
}

void TTCB::HandleEvent(TRANSFRAME& event)
{
	if((event.Flags&pcTIMER)==pcTIMER)
		ClearTimerID(event.Flags&TIMERMASK);

	switch(State){
	case stReady:
	case stSynchronize:
		switch(event.Flags){
		case pcCONTROL|ptSYN:
			DoACKSYN();
		case pmACK|pcCONTROL|ptSYN:
			DoResetAll();
			State = stWork;
			break;
		case pcTIMER|TM_S:
			DoSYN();
			DoCheckSYN();
			break;
		default :
			DoSYN();
			if(State==stReady){
				DoSetSTimer();
				State = stSynchronize;
			}
			break;
		}
		break;
	case stWork:
	case stReset:
		switch(event.Flags){
		case pcDATA:
			DoReceiveFrame(&event);
			break;
		case pmACK:
			DoACK(&event);
			break;
		case pmACK|pcDATA:
			DoReceiveFrame(&event);
			DoACK(&event);			
			break;
		case pcCONTROL|ptREJ:
			DoREJ(&event);
			break;
		case pcTIMER|TM_W:
			DoReSend();
			break;
		case pcTIMER|TM_B:
			DoSendACK();
			break;
		case pcCONTROL|ptRST:
			DoRST();
			break;
		case pmACK|pcCONTROL|ptRST:
			DoDelRTimer();
			DoReActive();
			State = stWork;
			break;
		case pcTIMER|TM_R:
			DoSendRST();
			DoCheckReset();
			break;
		case pcCONTROL|ptSYN:
			DoACKSYN();
			DoResetAll();
			State = stWork;
			break;
		case pcCONTROL|ptPOOL:
			DoACKPOOL();
			break;
		}
		break;
	case stResetPaulse:
		if(event.Flags==(pmACK|pcCONTROL|ptRST)){
			DoReActive();
			State = stWork;
			break;
		}else if(event.Flags==(pcCONTROL|ptRST)){
			DoRST();
			DoSetRTimer();
			State = stReset;
			break;
		}
	case stPool:
	case stPaulse:
		switch(event.Flags){
		case pcCONTROL|ptPOOL:
			DoACKPOOL();
		case pmACK|pcCONTROL|ptPOOL:
			if(State==stPool)
				DoDelPTimer();
			if(State==stResetPaulse){
				DoSetRTimer();
				State = stReset;
			}else{
				DoReActive();
				State = stWork;
			}
			break;
		case pcCONTROL|ptSYN:
			DoACKSYN();
		case pmACK|pcCONTROL|ptSYN:
			DoResetAll();
			State = stWork;
			break;
		case pcTIMER|TM_P:
			DoPOOL();
			DoCheckPool();
			break;
		default:
			if((event.Flags&0x70)!=pcTIMER)
			{
				if(State==stResetPaulse){
					DoSetRTimer();
					State = stReset;
				}else{
					DoReActive();
					State = stWork;
				}
			}
			break;
		}
		break;
	}
}

int TTCB::SendFrame(CTbyte type,CTuint seq,CTuint ack,CTushort window,
	PACKETDATA* Packet)
{
	TRANSFRAME Frame(SourAddress.addr,DestAddress.addr,
		type,seq,ack,window,Packet);

	return SendSocket->SendFrame(Frame);
}

int TTCB::SendFrame(TRANSFRAME& Frame)
{
	return SendSocket->SendFrame(Frame);
}

int TTCB::SavePacket(TRANSFRAME* Frame)
{
	int Result = -1;

	if(Frame)
	{
		if( RecvQueue->TryPut(GETMODULE(Frame->Data.receiver),Frame->Data) != E_QUEUEFULL )			
			Result = 0;
	}

	return Result;
}

CTint TTCB::isMember(CTuint Handle)
{
	return TCBID==Handle ? 1 : 0;
}

CTint TTCB::isMember(SocketAddr address)
{
    if((DestAddress.addr.sin_port==address.sin_port)&&
       (DestAddress.addr.sin_addr.s_addr==address.sin_addr.s_addr))
        return 1;
	return 0 ;
}

int TTCB::Resume()
{
	DoSetSTimer();
	State = stSynchronize;

	#ifdef _TRACE_	
	TRACE <<"TCB("<< TCBID << ") ST(" << dec << State
	<<") Send SYN To: "
	<< inet_ntoa(DestAddress.addr.sin_addr) << " " << dec
	<< ntohs(DestAddress.addr.sin_port) << endl;	
	#endif
	
	Trace.printf("TCB(%d) ST(%d) Send SYN To: %s %d\n",TCBID,
		State,
		inet_ntoa(DestAddress.addr.sin_addr),
		ntohs(DestAddress.addr.sin_port));
	

	return SendFrame(pcCONTROL|ptSYN,0,0,0,0)>=0 ? 0 : -1;
}
void TTCB::DoSYN()
{
	#ifdef _TRACE_	
	TRACE <<"TCB("<< TCBID << ") ST(" << dec << State
	<<") Send SYN To: "
	<< inet_ntoa(DestAddress.addr.sin_addr) << " " << dec
	<< ntohs(DestAddress.addr.sin_port) << endl;	
	#endif
	
	Trace.printf("TCB(%d) ST(%d) Send SYN To: %s %d\n",TCBID,
		State,
		inet_ntoa(DestAddress.addr.sin_addr),
		ntohs(DestAddress.addr.sin_port) );
	

	SendFrame(pcCONTROL|ptSYN,0,0,0,0);
}

void TTCB::DoACKSYN()
{
	#ifdef _TRACE_	
	TRACE <<"TCB("<< TCBID << ") ST(" << dec << State
	<<") Recv SYN From: "
	<< inet_ntoa(DestAddress.addr.sin_addr) << " " << dec
	<< ntohs(DestAddress.addr.sin_port) << endl;	
	#endif
	
	Trace.printf("TCB(%d) ST(%d) Recv SYN From: %s %d\n",TCBID,
		State,
		inet_ntoa(DestAddress.addr.sin_addr),
		ntohs(DestAddress.addr.sin_port) );
	

	SendFrame(pmACK|pcCONTROL|ptSYN,0,0,0,0);
}

void TTCB::DoCheckSYN()
{
	if(--MaxSynchronizeTimes <= 0) {
		#ifdef _TRACE_		
		TRACE <<"TCB("<< TCBID << ") ST(" << dec << State
		<<") Synchronize Failure !!! " << endl;
		#endif
		
		Trace.printf("TCB(%d) State(%d) Synchronize Failure !!!",
			TCBID,State);
		
		MaxSynchronizeTimes = RE_SYN;
		State = stPaulse;
		DoGameOver();
		return;
	}	
	else	
	{
		#ifdef _TRACE_		
		TRACE <<"TCB("<< TCBID << ") ST(" << dec << State
		<<") TM_S" << endl;		
		#endif
		
		Trace.printf("TCB(%d) ST(%d) TM_S\n",TCBID,State);
	}
	
	DoSetSTimer();
}

void TTCB::DoCheckPool()
{
	if(--MaxPoolTimes <= 0) {
		#ifdef _TRACE_		
		TRACE <<"TCB("<< TCBID << ") ST(" << dec << State
		<<") Pool Failure!!! " << endl;				
		#endif
		Trace.printf("TCBID(%d) ST(%d) Pool Failure!!!\n", TCBID, State);
		
		MaxPoolTimes = RE_POOL;
		State = stPaulse;
		DoGameOver();
		return;
	}
	
	else
	{
		#ifdef _TRACE_		
		TRACE <<"TCB("<< TCBID << ") ST(" << dec << State
		<<") TM_P" << endl;
		#endif		
		
		Trace.printf("TCB(%d) ST(%d) TM_P\n",TCBID,State);
	}
	
	DoSetPTimer();
}

void TTCB::DoCheckReset()
{
	if(--MaxResetTimes <= 0) {
		#ifdef _TRACE_		
		TRACE <<"TCB("<< TCBID << ") ST(" << dec << State
		<<") Reset Failure!!! " << endl;
		#endif	
		
		Trace.printf("TCBID(%d) ST(%d) Reset Failure!!!\n");
		
		MaxPoolTimes = RE_RESET;
		if(IsSetTimer(TM_W))
			DoDelWTimer();
		if(IsSetTimer(TM_B))
			DoDelBTimer();
		State = stResetPaulse;
		return;
	}
	
	else
	{
		#ifdef _TRACE_	
		TRACE <<"TCB("<< TCBID << ") ST(" << dec << State
		<<") TM_R" << endl;
		#endif	
		
		Trace.printf("TCB(%d) ST(%d) TM_R\n",TCBID,State);
	}
	
	DoSetRTimer();
}

int TTCB::SendPacket(const PACKETDATA &packet)
{
	int ret=E_FAIL;

	if(State!=stWork)
	{
		switch(State){
		case stPaulse:
			if(!IsSetTimer(TM_P))
				DoSetPTimer();
			State = stPool;
			ret = E_TCBPAUSE ;
			break;

		case stResetPaulse:
			if(!IsSetTimer(TM_R))
				DoSetRTimer();
			State = stReset;
			ret = E_TCBPAUSE;
			break;

		case stReset:
			break;
		}

		return ret;
	}

	if(SendWindow->WinRemain()<=0)
		return E_QUEUEFULL;

	TRANSFRAME Frame(SourAddress.addr,DestAddress.addr,
		0,0,0,0,(PACKETDATA*)&packet);
	
	Frame.Flags = pcDATA;
	Frame.Seq = SendWindow->WinSeq;
	Frame.Ack = 0;
	Frame.Window = 0;
	if(IsSetTimer(TM_B)){
		DoDelBTimer();
		Frame.Flags |= pmACK;
		Frame.Ack = RecvWindow->WinHead;

		#ifdef _TRACE_		
		TRACE <<"	TCB("<< TCBID << ") ST("
		<< dec << State
		<<") Send DATA_ACK("
		<<Frame.Ack<<") PDU("<<Frame.Seq
		<<")"<< endl;
		#endif
		
		Trace.printf("	TCB(%d) ST(%d) Send DATA_ACK(%d) PDU(%d)\n",
			TCBID,State,Frame.Ack,Frame.Seq );		
	}

	#ifdef _TRACE_	
	TRACE <<"TCB("<< TCBID << ") ST(" << dec << State
	<<") Send PDU("<< Frame.Seq << ") SWND("
	<< SendWindow->WinHead
	<<") SSEQ("<<SendWindow->WinSeq <<")"<< endl;	
	#endif

	Trace.printf("TCB(%d) ST(%d) Send PDU(%d) SWND(%d) SSEQ(%d)\n",
		TCBID,State,Frame.Seq,SendWindow->WinHead,SendWindow->WinSeq);
	

	SendWindow->SaveFrame(&Frame);
	SendWindow->WinSeq = NEXT(SendWindow->WinSeq);
	
	SendFrame(Frame);

	if(!IsSetTimer(TM_W))
		DoSetWTimer();

	return E_SUCCESS;
}

void TTCB::DoReceiveFrame(TRANSFRAME* Frame)
{
	#ifdef _TRACE_	
	TRACE <<"	TCB("<< TCBID << ") ST(" << dec << State
	<<") Recv PDU("<< Frame->Seq<<") RWND("<<RecvWindow->WinHead
	<<") RSEQ("<<RecvWindow->WinSeq<<")"<< endl;
	#endif
	
	Trace.printf("	TCB(%d) ST(%d) Recv PDU(%d) RWND(%d) RSEQ(%d)\n",
		TCBID,State,Frame->Seq,RecvWindow->WinHead,RecvWindow->WinSeq);
	

	if(Frame->Seq==RecvWindow->WinHead)	{
		if(RecvWindow->WinHead==RecvWindow->WinSeq)	{
			SavePacket(Frame);
			RecvWindow->WinHead = RecvWindow->WinSeq =
				NEXT(RecvWindow->WinHead);
			if(!IsSetTimer(TM_B))
				DoSetBTimer();
			return;
		} else	{
			SavePacket(Frame);
			for(RecvWindow->WinHead = NEXT(RecvWindow->WinHead);
				RecvWindow->WinFrame(RecvWindow->WinHead) &&
				RecvWindow->WinOffset(RecvWindow->WinSeq)>=0;
				RecvWindow->WinHead = NEXT(RecvWindow->WinHead)) {
				SavePacket(RecvWindow->WinFrame(RecvWindow->WinHead));
				RecvWindow->DelFrame(RecvWindow->WinHead);
			}
			if(RecvWindow->WinOffset(RecvWindow->WinSeq)>=0)
				DoSendREJ();
			else {
				if(IsSetTimer(TM_B))
					DoDelBTimer();
				DoSendACK();
				RecvWindow->WinSeq = RecvWindow->WinHead;
			}
			return;
		}
	} else {
		int WinSeqOffset = RecvWindow->WinOffset(Frame->Seq);
		if(WinSeqOffset>=0)	{
			RecvWindow->SaveFrame(Frame);
			if(RecvWindow->WinHead==RecvWindow->WinSeq)
				DoSendREJ();
			else if(!IsSetTimer(TM_B))
				DoSetBTimer();
			if(WinSeqOffset>RecvWindow->WinOffset(RecvWindow->WinSeq))
				RecvWindow->WinSeq = Frame->Seq;
		}
		else if(RecvWindow->WinHead==RecvWindow->WinSeq)
		{
			#ifdef _TRACE_			
			TRACE <<"	TCB("<< TCBID << ") ST(" << dec << State
			<<") RE-Send ACK("<<RecvWindow->WinHead<<")"<< endl;
			#endif
			
			Trace.printf("	TCB(%d) State(%d) RE-Send ACK(%d)\n",
				TCBID,State,RecvWindow->WinHead);
			
			DoSendACK();
		}
		else
		{
			#ifdef _TRACE_			
			TRACE <<"	TCB("<< TCBID << ") ST(" << dec << State
			<<") RE-Send REJ("<<RecvWindow->WinHead<<")"<< endl;			
			#endif
			
			Trace.printf("	TCB(%d) ST(%d) RE-Send REJ(%d)\n",
				TCBID,State,RecvWindow->WinHead);
			
			DoSendREJ();
		}
		return;
	}
	if(!IsSetTimer(TM_B))
		DoSetBTimer();
}

void TTCB::DoACK(TRANSFRAME* Frame)
{
	if(State!=stWork)
		return;
	#ifdef _TRACE_	
	TRACE <<"TCB("<< TCBID << ") ST(" << dec << State
	<<") Recv ACK(" <<Frame->Ack << ") SWND("<<SendWindow->WinHead
	<<") SSEQ("<<SendWindow->WinSeq<<")"<< endl;
	#endif
	
	Trace.printf("TCB(%d) ST(%d) Recv ACK(%d) SWND(%d) SSEQ(%d)\n",
		TCBID,State,Frame->Ack,SendWindow->WinHead,SendWindow->WinSeq);
	
	int AckCount = BETWEEN(SendWindow->WinHead,SENDWINSIZE+1,Frame->Ack);
	if(AckCount<0) {
		if(BETWEEN(PREVN(SendWindow->WinHead,WINLOOKBACK),
			WINLOOKBACK,Frame->Ack)<0)	{

			#ifdef _TRACE_			
			TRACE <<"TCB("<< TCBID << ") ST(" << dec << State
			<<") Recv ACK(" <<Frame->Ack << ") OUT OF WINDOW! SWND("
			<<SendWindow->WinHead
			<<") SSEQ("<<SendWindow->WinSeq<< ")"<<endl;			
			#endif
			
			Trace.printf("TCB(%d) ST(%d) Recv ACK(%d) OUT OF WINDOW! SWND(%d) SSEQ(%d)\n",
				TCBID,State,Frame->Ack,SendWindow->WinHead,SendWindow->WinSeq);
			

			if(IsSetTimer(TM_W))
				DoDelWTimer();

			SendWindow->Reset();
			DoSendRST();
			if(!IsSetTimer(TM_R))
				DoSetRTimer();
			State = stReset;
		}
		return;
	}

	if(IsSetTimer(TM_W))
		DoDelWTimer();

	for(int i=0;i<AckCount;i++) {
		SendWindow->DelFrame(SendWindow->WinHead);
		SendWindow->WinHead = NEXT(SendWindow->WinHead);
	}

	if(SendWindow->WinFrame(SendWindow->WinHead))
		DoSetWTimer();

	MaxSendTimes = RE_SEND;
}

void TTCB::DoREJ(TRANSFRAME* Frame)
{
	if(State!=stWork)
		return;
	#ifdef _TRACE_	
	TRACE <<"TCB("<< TCBID << ") ST(" << dec << State
	<<") Recv REJ(" <<Frame->Seq << ") SWND("<<SendWindow->WinHead
	<<") SSEQ("<<SendWindow->WinSeq<<")"<< endl;
	#endif
	
	Trace.printf("TCB(%d) ST(%d) Recv REJ(%d) SWND(%d) SSEQ(%d)\n",
		TCBID,State,Frame->Seq,SendWindow->WinHead,SendWindow->WinSeq);
	
	int AckCount = BETWEEN(SendWindow->WinHead,SENDWINSIZE,Frame->Seq);
	if(AckCount<0) {
		#ifdef _TRACE_		
		TRACE <<"TCB("<< TCBID << ") ST(" << dec << State
		<<") Recv REJ(" <<Frame->Seq << ") OUT OF WINDOW! SWND("
		<<SendWindow->WinHead
		<<") SSEQ("<<SendWindow->WinSeq<<")"<< endl;
		#endif
		
		Trace.printf("TCB(%d) ST(%d) Recv REJ(%d) OUT OF WINDOW! SWND(%d) SSEQ(%d)\n",
			TCBID,State,Frame->Seq,SendWindow->WinHead,SendWindow->WinSeq);
		

		if(IsSetTimer(TM_W))
			DoDelWTimer();

		SendWindow->Reset();
		DoSendRST();
		if(!IsSetTimer(TM_R))
			DoSetRTimer();
		State = stReset;
		return;
	}

	if(IsSetTimer(TM_W))
		DoDelWTimer();

	for(int i=0;i<AckCount;i++) {
		SendWindow->DelFrame(SendWindow->WinHead);
		SendWindow->WinHead = NEXT(SendWindow->WinHead);
	}

	if(SendWindow->WinFrame(Frame->Seq))
	{
		#ifdef _TRACE_		
		TRACE <<"TCB("<< TCBID << ") ST(" << dec << State
		<<") Send LOST PDU("<< Frame->Seq << ") SWND("<< SendWindow->WinHead
		<<") SSEQ("<<SendWindow->WinSeq<<")"<< endl;
		#endif
		
		Trace.printf("TCB(%d) ST(%d) Send LOST PDU(%d) SWND(%d) SSEQ(%d)\n",
			TCBID,State,Frame->Seq,SendWindow->WinHead,SendWindow->WinSeq);
		
		TRANSFRAME* Temp = SendWindow->WinFrame(Frame->Seq);
		if(Temp->Occupy)
			SendFrame(pcDATA,Frame->Seq,0,0,&Temp->Data);
	}

	if(SendWindow->WinFrame(SendWindow->WinHead))
		DoSetWTimer();

	MaxSendTimes = RE_SEND;
}

void TTCB::DoReSend()
{
	if(State!=stWork)
		return;
	if(--MaxSendTimes <= 0) {
		MaxSendTimes = RE_SEND;

		DoSetPTimer();
		State = stPool;

		#ifdef _TRACE_		
		TRACE <<"TCB("<< TCBID << ") ST(" << dec << State
		<<") ReSend Failure!!!"<<endl;
		#endif
		
		Trace.printf("TCB(%d) ST(%d) ReSend Failure!!!\n",
			TCBID,State);
		
		return;
	}
	#ifdef _TRACE_	
	TRACE <<"TCB("<< TCBID << ") ST(" << dec << State
	<<") TM_W"<< endl;
	#endif
	
	Trace.printf("TCB(%d) ST(%d) TM_W\n",TCBID,State);
	

	TRANSFRAME* Temp = SendWindow->WinFrame(SendWindow->WinHead);
	if(IsSetTimer(TM_B)) {
		DoDelBTimer();

		#ifdef _TRACE_		
		TRACE <<"	TCB("<< TCBID << ") ST(" << dec << State
		<<") ReSend DATA_ACK("
		<<RecvWindow->WinHead<<") PDU("<<SendWindow->WinHead<<")"<< endl;
		
		TRACE <<"TCB("<< TCBID << ") ST(" << dec << State
		<<") ReSend PDU("<< SendWindow->WinHead << ") SWND("
		<< SendWindow->WinHead
		<<") SSEQ("<<SendWindow->WinSeq<<")"<< endl;		
		#endif
		
		Trace.printf("	TCB(%d) ST(%d) ReSend DATA_ACK(%d) PDU(%d)\n",
			TCBID,State,RecvWindow->WinHead,SendWindow->WinHead );

		
		Trace.printf("TCB(%d) ST(%d) ReSend PDU(%d) SWND(%d) SSEQ(%d)\n",
			TCBID,State,SendWindow->WinHead,SendWindow->WinHead,SendWindow->WinSeq);

		

		if(Temp){
			if(Temp->Occupy)
				SendFrame(pmACK|pcDATA,SendWindow->WinHead,
					RecvWindow->WinHead,0,&Temp->Data);
		}
	} else {
		#ifdef _TRACE_		
		TRACE <<"TCB("<< TCBID << ") ST(" << dec << State
		<<") ReSend PDU("<< SendWindow->WinHead << ") SWND("
		<< SendWindow->WinHead
		<<") SSEQ("<<SendWindow->WinSeq<<")"<< endl;
		#endif
		
		Trace.printf("TCB(%d) ST(%d) ReSend PDU(%d) SWND(%d) SSEQ(%d)\n",
			TCBID,State,SendWindow->WinHead,SendWindow->WinHead);
		
		if(Temp){
			if(Temp->Occupy)
				SendFrame(pcDATA,SendWindow->WinHead,0,0,&Temp->Data);
		}
	}
	if(SendWindow->WinFrame(SendWindow->WinHead))
		DoSetWTimer();
}

void TTCB::DoSendRST()
{
	#ifdef _TRACE_	
	TRACE <<"TCB("<< TCBID << ") ST(" << dec << State
	<<") Send RST"<< endl;
	#endif
	
	Trace.printf("TCB(%d) ST(%d) Send RST\n",
		TCBID,State);
	
	SendFrame(pcCONTROL|ptRST,0,0,0,0);
}

void TTCB::DoRST()
{
	#ifdef _TRACE_	
	TRACE <<"	TCB("<< TCBID << ") ST(" << dec << State
	<<") Recv RST Reset RecvWindow!!!"<< endl;
	#endif
	
	Trace.printf("	TCB(%d) ST(%d) Recv RST Reset RecvWindow!!!\n",
		TCBID,State);
	

	if(IsSetTimer(TM_B))
		DoDelBTimer();

	RecvWindow->Reset();
	SendFrame(pmACK|pcCONTROL|ptRST,0,0,0,0);
}

void TTCB::DoPOOL()
{
	#ifdef _TRACE_	
	TRACE <<"TCB("<< TCBID << ") ST(" << dec << State
	<<") Send POOL"<< endl;
	#endif
	
	Trace.printf("TCB(%d) State(%d) Send POOL\n",TCBID,State);
	
	SendFrame(pcCONTROL|ptPOOL,0,0,0,0);
}

void TTCB::DoACKPOOL()
{
	#ifdef _TRACE_	
	TRACE <<"TCB("<< TCBID << ") ST(" << dec << State
	<<") Send Ack POOL"<< endl;
	#endif
	
	Trace.printf("TCB(%d) ST(%d) Send Ack POOL\n",
		TCBID,State);
	
	SendFrame(pmACK|pcCONTROL|ptPOOL,0,0,0,0);
}

void TTCB::DoSendACK()
{
	#ifdef _TRACE_	
	TRACE <<"	TCB("<< TCBID << ") ST(" << dec << State
	<<") TM_B"<< endl;
	TRACE <<"	TCB("<< TCBID << ") ST(" << dec << State
	<<") Send ACK("<<RecvWindow->WinHead<<")"<< endl;
	#endif
	
	Trace.printf("	TCB(%d) ST(%d) TM_B\n",TCBID,State);
	Trace.printf("	TCB(%d) ST(%d) Send ACK(%d)\n",
		TCBID,State,RecvWindow->WinHead);
	

	SendFrame(pmACK,0,RecvWindow->WinHead,0,0);
}

void TTCB::DoSendREJ()
{
	#ifdef _TRACE_	
	TRACE <<"	TCB("<< TCBID << ") ST(" << dec << State
	<<") Send REJ("<<RecvWindow->WinHead<<")"<< endl;
	#endif
	
	Trace.printf("	TCB(%d) ST(%d) Send REJ(%d)\n",
		TCBID,State,RecvWindow->WinHead);

	
	SendFrame(pcCONTROL|ptREJ,RecvWindow->WinHead,0,0,0);

	if(IsSetTimer(TM_B))
		DoDelBTimer();
}

void TTCB::DoReActive()
{
	#ifdef _TRACE_	
	TRACE <<"TCB("<< TCBID << ") ST(" << dec << State
	<<") WEEK UP TO ACTIVE"<< endl;
	#endif
	
	Trace.printf("TCB(%d) ST(%d) WEEK UP TO ACTIVE\n",
		TCBID,State);
	
	if(SendWindow->WinFrame(SendWindow->WinHead))
		if(!IsSetTimer(TM_W))
			DoSetWTimer();
	if(!IsSetTimer(TM_B))
		DoSetBTimer();
	Valid = 1;
}

void TTCB::DoResetAll()
{
	#ifdef _TRACE_	
	TRACE <<"TCB("<< TCBID << ") ST(" << dec << State
	<<") RESET ALL!!!"<< endl;
	#endif
	
	Trace.printf("TCB(%d) ST(%d) RESET ALL!!!\n",TCBID,State);
	
	SendWindow->Reset();
	RecvWindow->Reset();
	Valid = 1;
	MaxSynchronizeTimes = RE_SYN;
	MaxSendTimes = RE_SEND;
	MaxResetTimes = RE_RESET;
	MaxPoolTimes = RE_POOL;
	if(IsSetTimer(TM_S)) DoDelSTimer();
	if(IsSetTimer(TM_W)) DoDelWTimer();
	if(IsSetTimer(TM_B)) DoDelBTimer();
	if(IsSetTimer(TM_R)) DoDelRTimer();
	if(IsSetTimer(TM_P)) DoDelPTimer();
}

void TTCB::TCBInfo(char *buffer)
{
	const char *StateMsgArray[8]={"UNK", "Ready","Synchronize","Work",
		"Reset","ResetPause","Pool","Pause"};
	char StateMsg[10];

	if( State >= stReady && State <= stPaulse )
		strcpy( StateMsg, StateMsgArray[State] );
	else 
		strcpy( StateMsg, "UNK" );

	sprintf(buffer, "TCB(%d:%s,%d) ST(%s) SendWinHead(%d) SSEQ(%d) RecvHead(%d) RecvSeq(%d)\n",
			TCBID, 	inet_ntoa(DestAddress.addr.sin_addr) ,
			ntohs(DestAddress.addr.sin_port) ,
			StateMsg,SendWindow->WinHead,SendWindow->WinSeq,
			RecvWindow->WinHead,RecvWindow->WinSeq);
}


void TTCB::DoGameOver()
{
	#ifdef _TRACE_	
	TRACE <<"TCB("<< TCBID << ") ST(" << dec << State
	<<") GAME OVER!!!"<< endl;
	#endif
	
	Trace.printf("TCB(%d) GAME OVER!\n",TCBID,State);
	
	Valid = 0;
}

void TTCB::StartTrace()
{
	Trace.setTraceMode(LOG_TO_FILE);	
}

void TTCB::StopTrace()
{
	Trace.setTraceMode(LOG_TO_NULL);	
}


CTuint NEXT(CTuint SEQ)
{
	if(SEQ==MAXSEQUENCE)
		return MINSEQUENCE;
	return SEQ+1;
}

CTuint PREV(CTuint SEQ)
{
	if(SEQ==MINSEQUENCE)
		return MAXSEQUENCE;
	return SEQ-1;
}

CTuint NEXTN(CTuint SEQ,CTuint N)
{
	if(N==0)
		return SEQ;
	if(MAXSEQUENCE-SEQ<N)
		return MINSEQUENCE+(N-(MAXSEQUENCE-SEQ))-1;
	return SEQ+N;
}

CTuint PREVN(CTuint SEQ,CTuint N)
{
	if(N==0)
		return SEQ;
	if(SEQ-MINSEQUENCE<N)
		return MAXSEQUENCE-(N-(SEQ-MINSEQUENCE))+1;
	return SEQ-N;
}

int BETWEEN(CTuint BASE,CTuint SIZE,CTuint SEQ)
{
	if(NEXTN(BASE,SIZE-1)>=BASE)
	{
		if(SEQ>=BASE&&SEQ<=NEXTN(BASE,SIZE-1))
			return SEQ-BASE;
		else
			return -1;
	}
	else if(SEQ>=BASE)
		return SEQ-BASE;
	else
	{
		if(SEQ<=NEXTN(BASE,SIZE-1))
			return MAXSEQUENCE-BASE+SEQ-MINSEQUENCE+1;
		else
			return -1;
	}
}
