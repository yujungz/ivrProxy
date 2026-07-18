#include "appCmm.h"
#include "wshProxy.h"

#ifndef  CMM_CFG
#define  CMM_CFG()  	 ((CAppCfg*&)m_pCfg)
#endif 

CAppCmm::CAppCmm(TCommManager* aCommManager, void* pCfg)
    : TCTModule(GOR_MAINTAINSERVER, (char*)"WSHSVR", aCommManager), m_pCfg(pCfg)
{
    mySender = MAKEMODULEGOR(this->GetStationNo(), GOR_MAINTAINSERVER);

    ivr_station_no = 11;
}

CAppCmm::~CAppCmm()
{

}

CTbool CAppCmm::HandleMessage(CTuint sender, CTtranInfo& tranInfo) {
    CMM_CFG()->log(QString("[CAppCmm::HandleMessage] messageID=0x%1,sender=0x%2,ivr_station_no=0x%3")
        .arg(tranInfo.messageID, 8, 16, QChar('0'))
        .arg(GETSTATION(sender), 8, 16, QChar('0'))
        .arg(ivr_station_no, 8, 16, QChar('0'))
    );

    wshProxy* pMain = (wshProxy*)(CMM_CFG()->getMain());

    switch (tranInfo.messageID) {
    case EvtPUBLIC_TELSUN_ModuleActivate:
        pMain->doShowLog(QString("common module activated ! cmm.sender(0x%1)packet{sender(0x%2)receiver(0x%3)messageID(0x%4)}")
            .arg(sender, 8, 16, QChar('0')).arg(tranInfo.sender, 8, 16, QChar('0'))
            .arg(tranInfo.receiver, 8, 16, QChar('0')).arg(tranInfo.messageID, 8, 16, QChar('0')));
        CMM_CFG()->log(QString("[CAppCmm::HandleMessage] Recv IVR Message - EvtPUBLIC_TELSUN_ModuleActivate(%1:%2:%3:%4)")
            .arg(sender).arg(tranInfo.sender).arg(tranInfo.receiver).arg(tranInfo.messageID));
        break;
    case EvtPUBLIC_TELSUN_ModuleDeactivate:
        CMM_CFG()->log(QString("[CAppCmm::HandleMessage] Recv IVR Message - EvtPUBLIC_TELSUN_ModuleDeactivate(%1:%2:%3:%4)")
            .arg(sender).arg(tranInfo.sender).arg(tranInfo.receiver).arg(tranInfo.messageID));
        //if (GETSTATION(sender) == ivr_station_no)
         //   HandleIVRStop(sender, tranInfo);
        break;
    case CmdPSP_TELSUN_IPRequestData:
        CMM_CFG()->log(QString("[CAppCmm::HandleMessage] Recv IVR Message - CmdPSP_TELSUN_IPRequestData(%1:%2:%3:%4)")
            .arg(sender).arg(tranInfo.sender).arg(tranInfo.receiver).arg(tranInfo.messageID));
        {
            CTshort cmdid = 0;
            char* pData = nullptr ;
            TKVSet& kvset = tranInfo.GetMessageData();
            CTerror er1 = kvset.GetString(GWKEY_RequestData, &pData), er2 = kvset.GetShort(GWKEY_Command, &cmdid);
            if (er1 != CT_errorOK && er2 != CT_errorOK){
                this->SendRespDirectRequestData(tranInfo.sender, 2, (char*)"");  //!!!
            }
            else {
                char* pBuf = nullptr;
                if (pData != nullptr && pData[0] != '\0') {
                    pBuf = new char[strlen(pData) + 1];
                    strcpy(pBuf,pData);
                }
                this->handleRequestData(tranInfo.sender, cmdid, pBuf);
            }
        }
        break;
    default: 
        CMM_CFG()->log(QString("[CAppCmm::HandleMessage] Recv IVR Message - default(%1:%2:%3:%4)")
            .arg(sender).arg(tranInfo.sender).arg(tranInfo.receiver).arg(tranInfo.messageID));
     //   PrintConsole(1, "[IVR]Recv IVR Message(0x%x:0x%x:0x%x:0x%x)", sender, tranInfo.sender, tranInfo.receiver, tranInfo.messageID);
    }

    return CT_boolTRUE;
}



bool CAppCmm::SendRespDirectRequestData(CTuint sessionid, int status,  char* fd){
    
    if (sessionid <= 0){
        CMM_CFG()->log(QString("[CAppCmm::SendRespDirectRequestData] [IVR_0x%1]Send RespDirectRequestData Failed.")
            .arg(sessionid, 8, 16, QChar('0')));
     //   logfile->printf("[IVR_0x%x]Send RespDirectRequestData Failed.\n", sessionid);
        return false;
    }
    
    CTtranInfo tranInfo(mySender, sessionid, RespPSP_TELSUN_IPRequestData);
    
    CTuint recv = MAKEMODULEGOR(GET_DEVICE_STATION(sessionid), GOR_SIA);
    
    tranInfo.status = status;
    TKVSet& kvset = tranInfo.GetMessageData();
    if (fd)
        kvset.PutString(GWKEY_AnswerData, fd);
    else
        kvset.PutString(GWKEY_AnswerData, (char*)"");

    if (SendMessage(recv, tranInfo) != CT_errorOK)
    {
        CMM_CFG()->log(QString("[CAppCmm::SendRespDirectRequestData] [IVR_0x%1]Send RespDirectRequestData(%2)  to 0x%3 Failed.")
            .arg(sessionid, 8, 16, QChar('0')).arg(status).arg(recv, 8, 16, QChar('0')));
    //    logfile->printf("[IVR_0x%x]Send RespDirectRequestData(%d)  to 0x%x Failed.\n", sessionid, status, recv);
        return false;
    }
    else
    {
        CMM_CFG()->log(QString("[CAppCmm::SendRespDirectRequestData] [IVR_0x%1]Send RespDirectRequestData(%2)  to 0x%3 Ok..")
            .arg(sessionid, 8, 16, QChar('0')).arg(status).arg(recv, 8, 16, QChar('0')));
    //    logfile->printf("[IVR_0x%x]Send RespDirectRequestData(%d)  to 0x%x Ok.\n", sessionid, status, recv);
        return true;
    }
    

    return true;
}

bool CAppCmm::handleRequestData(const CTuint sessionid, const CTshort cmdid, char* pData){
    CMM_CFG()->getHts()->doDirectRequestData(sessionid, cmdid,QString::fromLatin1("%1").arg(pData));

    if (pData != nullptr) {
        delete pData;
    }

    return true;
}