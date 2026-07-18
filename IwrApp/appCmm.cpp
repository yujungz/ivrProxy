#include "appCmm.h"
#include "IwrApp.h"

#ifndef  CMM_CFG
#define  CMM_CFG()  	 ((CAppCfg*&)m_pCfg)
#endif 

CAppCmm::CAppCmm(TCommManager* aCommManager, void* pCfg)
    : TCTModule(GOR_MAINTAINSERVER, (char*)"IWRAPP", aCommManager), m_pCfg(pCfg)
{
    mySender = MAKEMODULEGOR(this->GetStationNo(), GOR_MAINTAINSERVER);

    mDefaultcallflowtype  = CMM_CFG()->getInt("LOCALCONFIG/Defaultcallflowtype",1);
    ivr_station_no = CMM_CFG()->getInt("LOCALCONFIG/ivr_station_no", 11);
    /*
    char  tt[80];
    QString ss = QString(".\\record\\019927635819_20240426111416");
    getCallFile(tt, ss.toLocal8Bit().constData());
    */
}

CAppCmm::~CAppCmm()
{

}

bool CAppCmm::execSql(const char* sSql) {
    bool bRet = true;
    CAppMySql*& pApp = CMM_CFG()->getMy();

    int nCmd = SQL_EXEC;
    pApp->setWaitSignal(0);
    emit pApp->doMessage(nCmd, 0, sSql);
    if (pApp->waitSignal(3000, nCmd)) {
        bool bRet = pApp->getExecRet();
        if (bRet) {
           // pMain->doShowLog(QString::fromLocal8Bit("saveBill succ"));

            CMM_CFG()->log(QString::fromLocal8Bit("execSql succ !"));
        }
        else {
            bRet = false;
            CMM_CFG()->log(QString::fromLocal8Bit("execSql fail ! err=%1").arg(pApp->getErr()));
            CMM_CFG()->log(QString::fromLocal8Bit("sql=%1").arg(sSql));
        }
    }

    return bRet;
}

CTbool CAppCmm::HandleMessage(CTuint sender, CTtranInfo& tranInfo) {
    CMM_CFG()->log(QString("[CAppCmm::HandleMessage] messageID=0x%1,sender=0x%2,ivr_station_no=0x%3")
        .arg(tranInfo.messageID, 8, 16, QChar('0'))
        .arg(GETSTATION(sender), 8, 16, QChar('0'))
        .arg(ivr_station_no, 8, 16, QChar('0'))
    );

    IwrApp* pMain = (IwrApp*)(CMM_CFG()->getMain());

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
            CTerror er1 = kvset.GetString(GWKEY_RequestData, &pData) ,er2 = kvset.GetShort(GWKEY_Command, &cmdid);

            if (er1 != CT_errorOK && er2 != CT_errorOK){
                this->SendRespDirectRequestData(tranInfo.sender, 2, (char*)"");  //!!!
            }
            else {
                CTuint uTid = tranInfo.tranID;
                char* pBuf = nullptr;
                if (pData != nullptr && pData[0] != '\0') {
                    pBuf = new char[strlen(pData) + 1];
                    strcpy(pBuf,pData);

                    QString strData = QString::fromLocal8Bit(pData);
                    uTid = strData.split(",")[0].toLong();
                }
                this->handleRequestData(uTid, tranInfo.sender, cmdid, pBuf);
                this->SendRespDirectRequestData(tranInfo.sender, 0, (char*)"");  
            }
        }
        break;
    case RespPSP_TELSUN_MakePredictiveCall:
        this->HandleRespMakePredialCall(tranInfo.sender, tranInfo);
        break;
    case EvtPSP_TELSUN_PredialReady:
        this->HandleEvtMakePredialCall(tranInfo.sender, tranInfo);
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

bool CAppCmm::SendPredialCallToMG( char* caller,  char* called,   char* serviceid,   char* pridata, unsigned int predialid,    char* displaytel, CTuint sno) {
    //PrivateData=type$Caller$Called$... type=0先呼通住叫，=1先呼通被叫，=2先启动流程，再MAKECALL主叫，=3先启动流程，再MAKECALL被叫
    char	retstr[255] = ""; // , calltype[2] = "";
    int		len = 0;
    char* p = 0;
    CTtranInfo tranInfo(mySender, 0, CmdPSP_TELSUN_MakePredictiveCall);
    tranInfo.tranID = sno;
    tranInfo.receiver = MAKEMODULEGOR(this->ivr_station_no, GOR_SwitchControl);

    TKVSet& kvset = tranInfo.GetMessageData();
    tranInfo.status = 0; //=1表示先启动流程，后执行MAKECALL
    kvset.Clear();

    kvset.PutString(PSP_TELSUN_PrivateData, pridata);
    
    /*
    if (!pridata || strlen(pridata) < 1)
    {
        sprintf(retstr, "OCXCALLOUT^0$%s$%s$", caller, called);
        kvset.PutString(PSP_TELSUN_OrigCaller, (char*)"");
        kvset.PutString(PSP_TELSUN_Called, caller);
        kvset.PutString(PSP_TELSUN_PrivateData, retstr);
    }
    else
    {
        p = strstr(pridata, "$");
        if (!p)
        {
            tranInfo.status = 0;
            sprintf(retstr, "OCXCALLOUT^0$%s$%s$", caller, called);
            kvset.PutString(PSP_TELSUN_Called, caller);
            kvset.PutString(PSP_TELSUN_PrivateData, retstr);
        }
        else
        {
            len = p - pridata;
            if (len == 0)
            {
                tranInfo.status = 0;
                sprintf(retstr, "OCXCALLOUT^0$%s$%s$", caller, called);
                kvset.PutString(PSP_TELSUN_Called, caller);
                kvset.PutString(PSP_TELSUN_PrivateData, retstr);
            }
            else if (pridata[0] == '0')
            {
                tranInfo.status = 0;
                sprintf(retstr, "OCXCALLOUT^%s", pridata);
                kvset.PutString(PSP_TELSUN_Called, caller);
                kvset.PutString(PSP_TELSUN_PrivateData, retstr);
            }
            else if (pridata[0] == '1')
            {
                tranInfo.status = 0;
                sprintf(retstr, "OCXCALLOUT^%s", pridata);
                kvset.PutString(PSP_TELSUN_Called, called);
                kvset.PutString(PSP_TELSUN_PrivateData, retstr);
            }
            else if (pridata[0] == '2')
            {
                tranInfo.status = 1;
                sprintf(retstr, "OCXCALLOUT^%s", pridata);
                kvset.PutString(PSP_TELSUN_Called, caller);
                kvset.PutString(PSP_TELSUN_PrivateData, retstr);
            }
            else
            {
                tranInfo.status = 1;
                sprintf(retstr, "OCXCALLOUT^%s", pridata);
                kvset.PutString(PSP_TELSUN_Called, called);
                kvset.PutString(PSP_TELSUN_PrivateData, retstr);
            }
        }
    }
    */

    if (mDefaultcallflowtype == 1)
        tranInfo.status = 0;
    else if (mDefaultcallflowtype == 2)
        tranInfo.status = 1;
    
    //kvset.PutString(PSP_TELSUN_Caller, serviceid);
    kvset.PutString(PSP_TELSUN_Caller, called);
    kvset.PutString(PSP_TELSUN_Called, caller);
    kvset.PutString(PSP_TELSUN_OrigCaller, (char*)"");
    kvset.PutByte(PSP_TELSUN_Location, 0);
    kvset.PutString(PSP_TELSUN_OrigCalled, (char*)"");
    kvset.PutString(PSP_TELSUN_ServiceNumber, serviceid); //IVR根据该号码进行认证
    if (displaytel && strlen(displaytel) > 0)
        kvset.PutString(PSP_TELSUN_TelephoneNumber, displaytel);
    sprintf(retstr, "1$%d$%d", predialid, predialid);
    kvset.PutString(PSP_TELSUN_MessageIndex, retstr);
    if (this->SendMessage(tranInfo.receiver, tranInfo) != CT_errorOK)
    {
        CMM_CFG()->log(QString("[Comm] Send PredialCall To 0x%1 Failed. caller(%2) called(%3) serviceid(%4) privatedata(%5)")
            .arg(tranInfo.receiver, 8, 16, QChar('0')).arg(caller).arg(called).arg(serviceid).arg(pridata));
     //   logfile->printf("[Comm] Send PredialCall To 0x%x Failed. caller(%s) called(%s) serviceid(%s) privatedata(%s)\n", tranInfo.receiver, caller, called, serviceid, pridata);
        return false;
    }
    else
    {
        CMM_CFG()->log(QString("[Comm] Send PredialCall To 0x%1 Succed. caller(%2) called(%3) serviceid(%4) privatedata(%5)")
            .arg(tranInfo.receiver, 8, 16, QChar('0')).arg(caller).arg(called).arg(serviceid).arg(pridata));
     //   logfile->printf("[Comm] Send PredialCall To 0x%x Ok. caller(%s) called(%s) serviceid(%s) privatedata(%s)\n", tranInfo.receiver, caller, called, serviceid, pridata);
        return true;
    }
    

    return true;
}

int CAppCmm::getCallFile(char* sFile, const char* sCall) {
    int n = strlen(sCall),i,j = 0;

    char sBuf[200] = { 0 };
    for (i = n - 1; i >= 0; --i) {
        if (sCall[i] == '\\') {
            sBuf[j++] = '\0';
        } else {
            sBuf[j++] = sCall[i];
        }
    }

    n = strlen(sBuf);
    sFile[0] = '\0';
    for (i = 0; i < n; ++i) {
        sFile[i] = sBuf[n - i - 1];
    }
    sFile[i] = '\0';

    return n;
}

bool CAppCmm::handleRequestData(const CTuint tranID, CTuint sender, const CTshort cmdid, char* pData){
    QString prvData;
    if (pData == nullptr || pData[0] == '\0') {
        CMM_CFG()->log(QString("[CAppCmm::handleRequestData] no private data ! "));
        return false;
    }else{
        prvData = QString::fromLocal8Bit(pData);
        CMM_CFG()->log(QString("[CAppCmm::handleRequestData] tranID=%1,cmdid=%2, private data = %3 ! ").arg(tranID).arg(cmdid).arg(prvData));
        delete pData;
    }

    struct stTapDial* pTd = CMM_CFG()->findDial(tranID);
    if (nullptr == pTd) {
        CMM_CFG()->log(QString("[CAppCmm::handleRequestData] get dial record fail ! tranID=%1").arg(tranID));
        return false;
    }
    struct stTapDial& td = *pTd;

//    QDateTime callTime = QDateTime::currentDateTime()
    quint64 msceCur = QDateTime::currentMSecsSinceEpoch();
    char sCallTime[80];
    strcpy(sCallTime, QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss").toLocal8Bit().constData());

    bool bFree = true;
    QString sStatus = QString::fromLocal8Bit("挂机通知"),sVal;
    QString sCode = td.InCaller ;
    int msgType = 3;

    IwrApp* pMain = (IwrApp*)(CMM_CFG()->getMain());
    int callType = pTd->CallType;
    switch (callType) {
    case 0:
    case 1:
        if (1 == cmdid) {  //摘机
            bFree = false;

            if (1 == callType) {  //主叫摘机
                sStatus = QString::fromLocal8Bit("主叫摘机");
                sVal = QString("CALLERHOOKOFFTIME='%1'").arg(sCallTime);
                td.stpHookOffCaller = msceCur;

            }
            else {  //被叫摘机
                sStatus = QString::fromLocal8Bit("被叫摘机");
                sCode = td.InCalled;
                sVal = QString("CALLEDHOOKOFFTIME='%1'").arg(sCallTime);
                td.stpHookOffCalled = msceCur;
            }
            msgType = 2;
        }
        else if (2 == cmdid) {  //挂机

            sVal = QString("CALLERSTOPTIME='%1',CALLEDSTOPTIME='%1',CALLERHOLDTIME=%2,CALLEDHOLDTIME=%3").arg(sCallTime).arg((msceCur - pTd->stpHookOffCaller)/1000).arg((msceCur - pTd->stpHookOffCalled) / 1000);
        }
        else {  //呼叫失败
            sStatus = QString::fromLocal8Bit("呼叫失败");
        }
        break;
    case 2:
        if (2 == cmdid) {  //主叫挂机
            sVal = QString("CALLERSTOPTIME='%1',CALLERHOLDTIME=%2").arg(sCallTime).arg((msceCur - pTd->stpHookOffCaller) / 1000);
        }
        else {
            sCode = td.InCalled;
        }

        break;
    case 3:
        if (1 == cmdid) {  //主叫摘机
            bFree = false;

            sVal = QString("CALLERHOOKOFFTIME='%1'").arg(sCallTime);
            td.stpHookOffCaller = msceCur;
            msgType = 2;
        }
        else if (2 == cmdid) {  //主叫挂机
            sVal = QString("CALLERSTOPTIME='%1',CALLERHOLDTIME=%2").arg(sCallTime).arg((msceCur - pTd->stpHookOffCaller) / 1000);
        }
        else {  //呼叫失败
            sStatus = QString::fromLocal8Bit("呼叫失败");
        }
        break;

    }
    //{
    if (!sVal.isEmpty()) {
        CAppMySql*& pMy = CMM_CFG()->getMy();
        QString sRecName = "";
        if (bFree) {
            char sFile[200] = {0};
            this->getCallFile(sFile, prvData.toLocal8Bit().constData());            
            if (strlen(sFile) > 0) {
                sRecName = QString(",RECORDFILENAME='%1'").arg(sFile);
            }
            /*
            QStringList arData = prvData.split(",");
            if (arData.count() > 2 && !arData[2].isEmpty()) {
                sRecName = ",RECORDFILENAME='" + arData[2] + "'";
            }
            */
        }
        QString sSql = QString::fromLocal8Bit("update %1 set %2 %3  where  INCALLID='%4'").arg(pMy->tbn("call_record")).arg(sVal).arg(sRecName).arg(td.InCallID);

        char sql[1024];
        strcpy(sql, sSql.toLocal8Bit().constData());
        this->execSql(sql);
    }
    // }    

    if (QString(td.ReportUrl).isEmpty()) {
        CMM_CFG()->log(QString("[Comm::handleRequestData] ReportUrl is blank, can not doing report !"));
    }
    else {
        CJsonKit jk;
        jk.putVal(td.InCallID, "/incallId");
        jk.putNumber(msgType, "/msgType");
        QString sMsg = QString::fromLocal8Bit("%1 %2").arg(sCode).arg(sStatus);
        jk.putVal(sMsg, "/Msg");
        jk.putNumber(QDateTime::currentSecsSinceEpoch(), "/Time");

        QString strInput = jk.toString(), strResult;

        CMM_CFG()->SendReq(td.ReportUrl, strInput, strResult, false);
        CMM_CFG()->log(QString("[Comm::handleRequestData] url(%1)ret(%2)")
           .arg(strInput).arg(strResult));
    }

    if (bFree) {
        emit pMain->doFreeDial(tranID);
    }

    return true;
}

void CAppCmm::HandleRespMakePredialCall(CTuint sender, CTtranInfo& tranInfo) {

    QString sStatus = QString::fromLocal8Bit("未知");

    switch (tranInfo.status)
    {
    case 0: //成功
    //    mTcpHead.Status = 0;
        sStatus = QString::fromLocal8Bit("成功");
     //   sStatus = QString::fromLocal8Bit("振铃");
        break;
    case CSTA_Cause_NoAnswer:// 无人接
    //    mTcpHead.Status = 1;
        sStatus = QString::fromLocal8Bit("无人接");
        break;
    case CSTA_Cause_UserBusy:  //用户忙
    //    mTcpHead.Status = 2;
        sStatus = QString::fromLocal8Bit("用户忙");
        break;
    case CSTA_Cause_CallFail:  //失败
   //     mTcpHead.Status = 3;
        sStatus = QString::fromLocal8Bit("失败");
        break;
    case CSTA_Cause_TrunkBusy: // 线路忙
    //    mTcpHead.Status = 4;
        sStatus = QString::fromLocal8Bit("线路忙");
        break;
    case CSTA_OP_InvalidDestination: // 空号
     //   mTcpHead.Status = 5;
        sStatus = QString::fromLocal8Bit("空号");
        break;
    case CSTA_OP_InvalidCalledDevice: //申请资源失败
    //    mTcpHead.Status = 7;
        sStatus = QString::fromLocal8Bit("申请资源失败");
        break;
    default:
    //    mTcpHead.Status = 6;
        break;
    }

    QString InCaller = QString("%1").arg(tranInfo.tranID);

    CMM_CFG()->log(QString("[CAppCmm::HandleRespMakePredialCall] tranID(%1)status(%2)")
        .arg(InCaller).arg(tranInfo.status));
   // return;

    IwrApp* pMain = (IwrApp*)(CMM_CFG()->getMain());
   // const struct stTapDial& td = CMM_CFG()->getDialInfo(InCaller);
    const struct stTapDial* pTd = CMM_CFG()->findDial(tranInfo.tranID);
    if (nullptr == pTd) {
        CMM_CFG()->log(QString("[CAppCmm::HandleRespMakePredialCall] get dial record fail !"));
        return;
    }
    const struct stTapDial& td = *pTd;
    
    CJsonKit jk;
    jk.putVal(td.InCallID, "/incallId");
    //jk.putNumber(tranInfo.status,"/msgType");
    jk.putNumber(1, "/msgType");
    QString sMsg = QString::fromLocal8Bit("%1 %2").arg(td.InCaller).arg(sStatus);
    jk.putVal(sMsg, "/Msg");
    jk.putNumber(QDateTime::currentSecsSinceEpoch() , "/Time");

    QString strInput = jk.toString(), strResult;

    CMM_CFG()->SendReq(td.ReportUrl, strInput, strResult, false);
    CMM_CFG()->log(QString("[Comm::HandleRespMakePredialCall] url(%1)ret(%2)")
        .arg(strInput).arg(strResult));

    emit pMain->doFreeDial(td.tranID);  //无论成功失败都释放拨号记录
        /*
    if (0 != tranInfo.status) {
        emit pMain->doFreeDial(td.tranID);
    }
    */
    /*
    TcpNode* mHead = 0;
    TagPacketHeadD mTcpHead;
    if (mTcpRespLink)
    {
        mHead = mTcpRespLink->SearchNodeBySequeneID(tranInfo.tranID);
        if (!mHead)
        {
            logfile->printf("[Comm]Can't Find PredialID(%d) In WaitResp Link.\n", tranInfo.tranID);
            return;
        }
        else
        {
            mTcpHead.CommmandID = 402003;
            mTcpHead.Sender = tranInfo.sender;  //sessionid
        }
        switch (tranInfo.status)
        {
        case 0: //成功
            mTcpHead.Status = 0;
            break;
        case CSTA_Cause_NoAnswer:// 无人接
            mTcpHead.Status = 1;
            break;
        case CSTA_Cause_UserBusy:  //用户忙
            mTcpHead.Status = 2;
            break;
        case CSTA_Cause_CallFail:  //失败
            mTcpHead.Status = 3;
            break;
        case CSTA_Cause_TrunkBusy: // 线路忙
            mTcpHead.Status = 4;
            break;
        case CSTA_OP_InvalidDestination: // 空号
            mTcpHead.Status = 5;
            break;
        case CSTA_OP_InvalidCalledDevice: //申请资源失败
            mTcpHead.Status = 7;
            break;
        default:
            mTcpHead.Status = 6;
            break;
        }
        if (mTcpServer)
        {
            mTcpServer->SendBuffer(mHead->packetHead.Sender, mTcpHead, "", 0);
            logfile->printf("[Comm]Send RespPredialCall PredialID(%d) Ok For SocketID(0x%x). Flow Device(0x%x). Resp Status(0x%x)\n", tranInfo.tranID, mHead->packetHead.Sender, tranInfo.sender, tranInfo.status);
        }
        else
            logfile->printf("[Comm]Send RespPredialCall PredialID(%d) Failded For SocketID(0x%x). Flow Device(0x%x). Resp Status(0x%x)\n", tranInfo.tranID, mHead->packetHead.Sender, tranInfo.sender, tranInfo.status);
        mTcpRespLink->DeleteNodeBySequeneID(tranInfo.tranID);
    }
    */
}

#define MAXCALLITEMCOUNT 30

void CAppCmm::HandleEvtMakePredialCall(CTuint sender, CTtranInfo& tranInfo){
    int callindex = GET_DEVICE_INDEX(tranInfo.tranID);
    int sessionindex = 0;
    CTuint sessionid = tranInfo.sender;
    sessionindex = GET_DEVICE_INDEX(sessionid);
    if (callindex < 0 || callindex >= MAXCALLITEMCOUNT){
       // logfile->printf("[IVR_0x%x]Recv EvtPredialCall PredialID(0x%x) Ok.Can't find CallIndex(%d)\n", sessionid, tranInfo.tranID, callindex);
        CMM_CFG()->log(QString("[IVR_0x%1]Recv EvtPredialCall PredialID(0x%2) Ok.Can't find CallIndex(%3)")
            .arg(sessionid, 8, 16, QChar('0')).arg(tranInfo.tranID, 8, 16, QChar('0')).arg(callindex));
        return;
    }
    else{

        /*
        if (!mCallInfo.Item[callindex].Used)
        {
            logfile->printf("[IVR_0x%x]HandleEvtMakePredialCall. PredialID(0x%x) Ok. CallIndex(%d) No Used. Ignore It\n", tranInfo.sender, tranInfo.tranID, callindex);
            return;
        }
        mCallInfo.Item[callindex].UpdateCallInfo(sessionid, 1);//update sessionid
        mSessionMap[sessionindex] = callindex; //!!!!!
        logfile->printf("[IVR_0x%x]Recv EvtPredialCall PredialID(0x%x) Ok. CallIndex(%d) \n", sessionid, tranInfo.tranID, callindex);
        */
    }
}