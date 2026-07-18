#ifndef __ClientData_H__
#define __ClientData_H__

#include <string.h>
#include <assert.h>
#include <iostream.h>
#include <fstream.h>
#include <memory.h>

#include "ctgor.h"
#include "cttypes.h"
#include "ctkvset.h"
#include "Udp.h"


#ifndef WIN32
#error Win32 API needed!
#endif

#include "KernelObject.h"

inline void MakeGorToClientData( CTuint gor, CTbyte *info )
{
	if( gor < constMAXMODULENUM ) 
	{
		CTuint byteIndex = gor / 8;
		CTuint bitIndex  = gor % 8;
		CTbyte mask = 1;
		info[byteIndex] |= ( mask << bitIndex );
	}
}

inline void ClearGorToClientData( CTuint gor, CTbyte *info )
{
	if( gor < constMAXMODULENUM ) 
	{
		CTuint byteIndex = gor / 8;
		CTuint bitIndex  = gor % 8;
		CTbyte mask = 1;
		info[byteIndex] &= ~(mask << bitIndex); 
	}
}

inline int GetGorFromClientData( CTuint gor, CTbyte *info)
{
	if( gor < constMAXMODULENUM ) 
	{
		CTuint byteIndex = gor / 8;
		CTuint bitIndex  = gor % 8;
		CTbyte data = info[byteIndex];
		return (int)( ( data >> bitIndex ) & (CTbyte)0x1 );
	}
	return 0;
}


class STATIONDATA
{
public :
	STATIONDATA(CTuint s, PEER &a,  CTbyte *m) :
		m_station(s)
	{	
		m_sendsync = 0;
		m_address = a ;
		m_keeplive = CT_boolFALSE;
		if( m ) 
			memcpy( m_info, m, constSTATIONDATASIZE);
		else
			memset( m_info, 0, constSTATIONDATASIZE);
	}
	~STATIONDATA() {};

	STATIONDATA& operator = (const STATIONDATA& data)
	{		
		m_sendsync = data.m_sendsync;
		m_station = data.m_station;
		m_keeplive = data.m_keeplive;
		m_address = data.m_address ;
		memcpy( m_info, data.m_info, constSTATIONDATASIZE);
		return *this;
	}

	STATIONDATA(const STATIONDATA& data) { *this = data; }

	CTuint GetStation() { return m_station; }
    CTbyte *GetInfo( ) { return (CTbyte *)m_info; }	
	PEER   &GetAddress() { return m_address; }	

	void MakeGorToClientData( CTuint gor) { ::MakeGorToClientData(gor, m_info);}
	void ClearGorToClientData( CTuint gor) {::ClearGorToClientData(gor, m_info);}
	int  GetGorFromClientData( CTuint gor){ return(::GetGorFromClientData(gor, m_info));}
	
    CTbool SetKeepLive( CTbool flag ) { m_keeplive=flag; return CT_boolTRUE;}
    CTbool IsKeepLive( ) { return m_keeplive ; }
	void   IncSyncCount(){m_sendsync++;}
	int    GetSyncCount(){return m_sendsync;}
	bool   CheckModuleActive()
	{
		for(int i=0;i<constSTATIONDATASIZE;i++)
			if(m_info[i]>0)
				return true;
		return false;
	}

	friend ofstream& operator << ( ofstream& log, STATIONDATA& c );

private :	
	CTuint  m_station;
	PEER	m_address ;
	CTbyte  m_info[constSTATIONDATASIZE];
	CTbool  m_keeplive;

public:
	int     m_sendsync;

};



class CTClientData
{
private:
		CTuint			IndexOfStation;
		CTuint			IndexOfSegment;
		CTuint			IndexOfOffset;
		CTCriticalSection m_lock;
		STATIONDATA		*Data[constMAXSTATIONNUM];
		time_t			tm1[constMAXMODULENUM]; //before GET() time
		time_t			tm2[constMAXMODULENUM];	//after GET() time

public:
		CTClientData();
		~CTClientData();

		CTbool CheckModule( CTuint stationNo, CTuint moduleGOR );
		CTbool CheckStation( CTuint stationNo );
		CTbool CheckStationData( CTuint stationNo, CTbyte* ClientData );

		CTbool GetStationData( CTuint stationNo, CTbyte* ClientData );

		CTbool AddStation( CTuint stationNo, const char *ip, int port, CTbyte* data );		
		CTbool AddModule( CTuint stationNo, CTuint moduleGOR );
		CTbool AddModule( CTuint moduleNo );

		CTbool DelModule( CTuint moduleNo );
		CTbool DelStation( CTuint stationNo );

        CTbool SetKeepLive( CTuint stationNo, CTbool flag );
        CTbool IsKeepLive( CTuint stationNo);

		inline CTbool Search( CTuint stationNo, PEER& aPeer );
		inline int Search( CTuint stationNo, const char *ipaddr, int port);

		int		SetTM1( CTuint moduleGOR)
		{
			if( moduleGOR >= 0 && moduleGOR < constMAXMODULENUM )
				tm1[moduleGOR] = time(0);
			return 0;
		}
		time_t		GetTM1( CTuint moduleGOR)
		{
			if( moduleGOR >= 0 && moduleGOR < constMAXMODULENUM )
				return tm1[moduleGOR];
			return 0;
		}

		int		SetTM2( CTuint moduleGOR)
		{
			if( moduleGOR >= 0 && moduleGOR < constMAXMODULENUM )
				tm2[moduleGOR] = time(0);
			return 0;
		}
		time_t		GetTM2( CTuint moduleGOR)
		{
			if( moduleGOR >= 0 && moduleGOR < constMAXMODULENUM )
				return tm2[moduleGOR];
			return 0;
		}
		STATIONDATA **GetStationData() { return Data; }

		bool   CheckModuleActive(CTuint stationNo);
		void   IncSyncCount(CTuint stationNo);
		int    GetSyncCount(CTuint stationNo);

};

inline CTbool CTClientData::Search( CTuint stationNo, PEER& aPeer )
{
	CTbool ret=CT_boolFALSE;

	if ( stationNo<constMINSTATIONNUM || stationNo>constMAXSTATIONNUM )	
		return CT_boolFALSE;
	
	m_lock.Lock();
	if ( Data[stationNo-1] )
	{
		aPeer = Data[stationNo-1]->GetAddress();
		ret = CT_boolTRUE ;
	}
	m_lock.Unlock();

	return ret;	
}

inline int CTClientData::Search( CTuint stationNo, const char *ipaddr, int port)
{
    int ret=-1;

    if ( stationNo<constMINSTATIONNUM || stationNo>constMAXSTATIONNUM || !ipaddr )
        return CT_boolFALSE;

    m_lock.Lock();
	if ( Data[stationNo-1] )
	{
        PEER peer( ipaddr, port);
		PEER aPeer = Data[stationNo-1]->GetAddress();
		if( aPeer == peer ) 
			ret = 0 ;
		else 
			ret = 1 ;
	}
    m_lock.Unlock();

    return ret;
}

#endif
