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
    bool SendRespDirectRequestData(CTuint sessionid,  int status,  char* fd);
    bool SendPredialCallToMG(  char* caller,   char* called,   char* serviceid,   char* pridata, unsigned int predialid,    char* displaytel, CTuint sno);
    bool execSql(const char* sSql);
protected:
    CTbool HandleMessage(CTuint sender, CTtranInfo& tranInfo);

private:
    CTuint  mySender;
    void* m_pCfg;
    int mDefaultcallflowtype;
    int getCallFile(char* sFile,const char* sCall);

    bool handleRequestData(const CTuint tranID, CTuint sender, const CTshort cmdid, char* pData);
    void HandleRespMakePredialCall(CTuint sender, CTtranInfo& tranInfo);
    void HandleEvtMakePredialCall(CTuint sender, CTtranInfo& tranInfo);
};
