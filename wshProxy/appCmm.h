#pragma once

#include "ctkvset.h"
#include "PUBLIC.h"
#include "PSPMSG.h"
#include "Gwpacket.h"
#include "WINCTModule.h"

class CAppCmm  : public TCTModule
{
public:
	CAppCmm(TCommManager* aCommManager, void* pCfg);
	~CAppCmm();

    CTuint  ivr_station_no;
    bool SendRespDirectRequestData(CTuint sessionid, int status,  char* fd);
protected:
    CTbool HandleMessage(CTuint sender, CTtranInfo& tranInfo);

private:
    CTuint  mySender;
    void* m_pCfg;

    bool handleRequestData(const CTuint sessionid,const CTshort cmdid, char* pData);
};
