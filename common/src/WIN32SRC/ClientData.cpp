
#include <io.h>
#include "ClientData.h"
#ifdef WIN32
#include "Shlwapi.h"
#endif

CTClientData::CTClientData()
{
	int i;
	for( i=0; i<constMAXSTATIONNUM; i++ )
	{
		Data[i] = 0;
	}

	for( i=0; i<constMAXMODULENUM; i++ )
	{
		tm1[i] = time(0);
		tm2[i] = time(0);
	}
	
}

CTClientData::~CTClientData()
{
	for( CTuint i=0; i<constMAXSTATIONNUM; i++ )
	{
		delete  Data[i];
		Data[i] = 0;
	}	
}

CTbool CTClientData::AddStation( CTuint stationNo, const char *ip, int port, CTbyte* data )
{
	if(!stationNo || stationNo>constMAXSTATIONNUM || !ip)
		return CT_boolFALSE;

	CTbool retval = CT_boolFALSE;
	int searval = Search(stationNo,ip,port);
	switch(searval)
	{case 0:  //ÒÑ¾­´æÔÚ
		break;
	 case 1:
		{
			m_lock.Lock();
			if(Data[stationNo-1])
			{
				retval = Data[stationNo-1]->IsKeepLive();
				delete Data[stationNo-1];
				Data[stationNo-1] = 0;
			}
			PEER peer(ip,port);
			Data[stationNo-1] = new STATIONDATA(stationNo,peer,data);	
			if(Data[stationNo-1])
				Data[stationNo-1]->SetKeepLive(CT_boolFALSE);
			m_lock.Unlock();
		}
		break;
	 default:
		{
			m_lock.Lock();
			if(Data[stationNo-1])
			{
				delete Data[stationNo-1];
				Data[stationNo-1] = 0;
			}
			PEER peer(ip,port);
			Data[stationNo-1] = new STATIONDATA(stationNo,peer,data);	
			if(Data[stationNo-1])
				Data[stationNo-1]->SetKeepLive(CT_boolFALSE);
			m_lock.Unlock();
		}
		break;
	}
	return CT_boolTRUE;
}

CTbool CTClientData::AddModule( CTuint moduleNo )
{
	CTbool ret = CT_boolFALSE;
	CTuint station = GETSTATION( moduleNo );
	CTuint gor     = GETMODULE( moduleNo );
	
	if( !station || station> constMAXSTATIONNUM )
		return CT_boolFALSE;

	m_lock.Lock();
	if ( Data[station-1] ) 
	{
		Data[station-1]->MakeGorToClientData( gor);
		ret = CT_boolTRUE ;
	}
	
	m_lock.Unlock();
	return ret;
}

CTbool CTClientData::AddModule( CTuint stationNo, CTuint moduleGOR )
{
	CTbool ret=CT_boolFALSE;

	if( !stationNo || stationNo > constMAXSTATIONNUM )
		return CT_boolFALSE;

	m_lock.Lock();
	if ( Data[stationNo-1] ) 
	{		
		Data[stationNo-1]->MakeGorToClientData( moduleGOR);
		ret = CT_boolTRUE ;
	}
	m_lock.Unlock();

	return ret;
}


CTbool CTClientData::CheckModule( CTuint stationNo, CTuint moduleGOR )
{	
	CTbool retval = CT_boolFALSE;
	
	if( !stationNo || stationNo > constMAXSTATIONNUM )
		return retval;

	m_lock.Lock();
	if ( Data[stationNo-1] )	
		retval = Data[stationNo-1]->GetGorFromClientData( moduleGOR );

	m_lock.Unlock();
	return retval;
}


CTbool CTClientData::CheckStation( CTuint stationNo )
{	
	CTbool retval = CT_boolFALSE;
	
	if( !stationNo || stationNo > constMAXSTATIONNUM )
		return retval;

	m_lock.Lock();
	if ( Data[stationNo-1] )
		retval = CT_boolTRUE ;

	m_lock.Unlock();
	return retval;
	
}



CTbool CTClientData::CheckStationData( CTuint stationNo, CTbyte* ClientData )
{
	CTbool retval = CT_boolFALSE;
	if( !stationNo || stationNo > constMAXSTATIONNUM )
		return retval;

	m_lock.Lock();
	if ( Data[stationNo-1] )
		retval = memcmp( ClientData+1, (Data[stationNo-1]->GetInfo())+1, constSTATIONDATASIZE-1 ) 
					? CT_boolFALSE:CT_boolTRUE;

	m_lock.Unlock();
	return retval ;	
}

CTbool CTClientData::GetStationData( CTuint stationNo, CTbyte* ClientData )
{
	CTbool retval = CT_boolFALSE;
	if( !stationNo || stationNo > constMAXSTATIONNUM )
		return retval;

	m_lock.Lock();
	if ( Data[stationNo-1] )
	{
		memcpy( ClientData, Data[stationNo-1]->GetInfo(), constSTATIONDATASIZE );
		retval = CT_boolTRUE ;
	}

	m_lock.Unlock();
	return retval;
	
}

CTbool CTClientData::DelModule( CTuint moduleNo )
{
	CTuint stationNo = GETSTATION( moduleNo );
	CTuint gor     = GETMODULE( moduleNo );
	CTbool retval = CT_boolFALSE;
	if( !stationNo || stationNo > constMAXSTATIONNUM )
		return retval;
	
	m_lock.Lock();
	if ( Data[stationNo-1] )
	{
		Data[stationNo-1]->ClearGorToClientData( gor);
		retval = CT_boolTRUE ;
	}
	
	m_lock.Unlock();
	return retval;	
}

CTbool CTClientData::DelStation( CTuint stationNo )
{
	CTbool retval = CT_boolFALSE;
	if( !stationNo || stationNo > constMAXSTATIONNUM )
		return retval;

	m_lock.Lock();
	if ( Data[stationNo-1] )
	{
		delete Data[stationNo-1];
		Data[stationNo-1] = 0;
		retval = CT_boolTRUE ;
	}
	m_lock.Unlock();

	return retval;	
}

CTbool CTClientData::IsKeepLive( CTuint stationNo )
{
    CTbool retval = CT_boolFALSE;

    if( !stationNo || stationNo > constMAXSTATIONNUM )
        return retval;

    m_lock.Lock();
    if ( Data[stationNo-1] )
        retval = Data[stationNo-1]->IsKeepLive();

    m_lock.Unlock();
    return retval;
}

CTbool CTClientData::SetKeepLive( CTuint stationNo, CTbool flag )
{
    CTbool retval = CT_boolFALSE;

    if( !stationNo || stationNo > constMAXSTATIONNUM )
        return retval;

    m_lock.Lock();
    if ( Data[stationNo-1] )
        retval = Data[stationNo-1]->SetKeepLive(flag);

    m_lock.Unlock();
    return retval;
}

bool CTClientData::CheckModuleActive(CTuint stationNo)
{
	if(!stationNo || stationNo>constMAXSTATIONNUM)
		return false;
	bool retval = false;
	
	m_lock.Lock();
	if(Data[stationNo-1])
		retval = Data[stationNo-1]->CheckModuleActive();
	m_lock.Unlock();
	return retval;
}

void CTClientData::IncSyncCount(CTuint stationNo)
{
	if(!stationNo || stationNo>constMAXSTATIONNUM)
		return;
	m_lock.Lock();
	if(Data[stationNo-1])
		Data[stationNo-1]->IncSyncCount();
	m_lock.Unlock();
}

int  CTClientData::GetSyncCount(CTuint stationNo)
{
	int retval=0;
	if(!stationNo || stationNo>constMAXSTATIONNUM)
		return retval;
	m_lock.Lock();
	if(Data[stationNo-1])
		retval = Data[stationNo-1]->GetSyncCount();
	m_lock.Unlock();
	return retval;
}
