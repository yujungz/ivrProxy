//Modified by Walker Gun2,1998-08-13
#ifndef __CTPACKET_H__
#define __CTPACKET_H__

#include <string.h>
#include <assert.h>

#include "ctgor.h"
#include "cttypes.h"
#include "ctkvset.h"
#include "ctutil.h"

//#define  constMAXBUFFER			0x2000 //4096

//Modified by Walker,1998-07-03
const int constMINPACKETSIZE = constSIZEOFINT_2 + constMINTRANINFOSIZE;
const int constMAXPACKETLEN  = constMINPACKETSIZE + MAXKVSETLEN;

class PACKETDATA
{
	public:
		CTuint      sender;
		CTuint      receiver;
		CTtranInfo  tranInfo;
	public:
		PACKETDATA() : sender(0),receiver(0) { }
		PACKETDATA(CTuint aSender,CTuint aReceiver,CTtranInfo &aTranInfo)
			: sender(aSender),receiver(aReceiver),tranInfo(aTranInfo) {	}
		PACKETDATA(CTuint aSender,CTuint aReceiver,CTuint aKey)
			: sender(aSender),
			receiver(aReceiver),
			tranInfo(aSender, aReceiver, aKey) { }
		PACKETDATA& operator = (const PACKETDATA& aPacket)
		{
			sender = aPacket.sender;
			receiver = aPacket.receiver;
			tranInfo = aPacket.tranInfo;
			return *this;
		}
		PACKETDATA(const PACKETDATA& aPacket) { *this = aPacket; }
		~PACKETDATA() { }

		void MakePacket(CTuint aSender, CTuint aReceiver,
			CTtranInfo &aTranInfo)
		{
			sender = aSender;
			receiver = aReceiver;
			tranInfo = aTranInfo;
		}
		void DePacket(CTuint &aSender, CTuint &aReceiver,
			CTtranInfo &aTranInfo)
		{
			aSender = sender;
			aReceiver = receiver;
			aTranInfo = tranInfo ;
		}

		CTerror MakePacket(CTuint aSender, CTuint aReceiver,
			char *buf, CTint len)
		{
			assert(len>=constMINPACKETSIZE);
			sender = aSender;
			receiver = aReceiver;
			return tranInfo.String2TranInfo(buf,len);
		}

		CTint DePacket(CTuint &aSender, CTuint &aReceiver,
			char *data, CTint maxlen)
		{
			aSender = sender;
			aReceiver = receiver;
			return tranInfo.TranInfo2String(data,maxlen);
		}

		CTint toString(char *str, int maxlen)
		{
			assert(maxlen>=constMINPACKETSIZE);
			char *p = str;
			CTuint2CTbyte(sender,p);
			p += sizeof(CTuint);
			CTuint2CTbyte(receiver,p);
			p += sizeof(CTuint);
			p += tranInfo.TranInfo2String(p,maxlen-constSIZEOFINT_2);
			return (p-str);
		}

		CTerror fromString(const char *str, int len)
		{
			assert(len>=constMINPACKETSIZE);
			char *p = (char *)str;
			CTbyte2CTuint( p, &sender);
			p += sizeof(CTuint);
			CTbyte2CTuint( p, &receiver);
			p += sizeof(CTuint);
			return tranInfo.String2TranInfo(p,len-constSIZEOFINT_2);
		}

		CTtranInfo& GetPacketData() { return tranInfo; }
		CTuint GetReceiver() { return receiver; }
		CTuint GetSender() { return sender; }
};

#endif
