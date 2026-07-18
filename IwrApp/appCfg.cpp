#include <QLayout>
#include <QDir>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
/*
#include <QSqlDatabase>
#include <QSqlError> 
#include <QSqlQuery>
#include <QSqlRecord>
*/
#include "IwrApp.h"

void g_onHttpAccepted(const QPointer< JQHttpServer::Session >& session,void* pApp) {
    if (nullptr == pApp) {
//        this->log(QString::fromLocal8Bit("未连接应用模块"));
        return;
    }
   

    CAppCfg*& pCfg = (CAppCfg*&)pApp;
    pCfg->onHttpAccepted(session);

}

CAppCfg::CAppCfg(const int& argc0, const char**& argv0) : CAppExt(argc0, argv0), pcommainManager(NULL) {

    m_nLog = 0;
  //  m_uSNo = 0;
    m_nChannel = 1024;

    m_arDial = new struct stTapDial[m_nChannel];
    int i, n = m_nChannel;
    for (i = 0; i < n; ++i) {
        m_qsSession.push(i);
        ::memset(&m_arDial[i], 0, sizeof(struct stTapDial));
    }

    m_pMain = nullptr;
    m_pTb = nullptr;
    m_pAppCmm = nullptr;
    m_pAppHtc = nullptr;
    m_pAppHts = nullptr;
    m_pAppMy = nullptr;

    m_sPathOrig = "";
    QString sPathApp = this->getAppPath(),sPathCur = QDir::currentPath().replace("/", "\\") + "\\";
    if (sPathApp != sPathCur) {
        m_sPathOrig = sPathCur;
        QDir::setCurrent(sPathApp);
    }

    QString sNetIni = QString("%1..\\..\\%2").arg(this->getAppPath()).arg("network.ini");
    this->reload(true, sNetIni);

    m_sKeySep = this->getStr("http/key_separate","/tapDial/");
    m_nChannel = this->getInt("http/channel_count", m_nChannel);

    m_uSNo = this->getInt("http/SerialNo");

    m_nRptTime = this->getInt("http/report_time_default", 3600);
    m_sServiceId = this->getStr("http/service_id", "77777777");
    //=======================================================
//    QString sTest = md5("appId=12345678&callershow=82610000&incalled=18038724658&incaller=18988651520&incallid=c73846d8bfedc6fa44b6240cbccb7bd3&reporturl=http://192.168.18.41:28100/call.php&timestamp=1714099826000&appKey=abcdefg").toUpper();

    /*
    char sData[80] = "1,23,我的测试";
    QString strData = QString::fromLocal8Bit(sData);
    QStringList arData = strData.split(",");
    QString s1 = arData[0];

    */
    /*
    int nn = n_call_record;
    
    //{ test
    
    struct stItemDisc  sid[2] = {
    {"fd1", "234", 1}, { "fd2","val2",0 }
    };
    
    const struct stItemDisc*  psid = std_call_record;

    QString sVal, sFds;
    m_pAppMy->mergeRec(sVal, sFds, psid, nn,1);

    // }
    */
}

CAppCfg::~CAppCfg() {
    this->stop();
    if (!m_sPathOrig.isEmpty()) {
        QDir::setCurrent(m_sPathOrig);
    }
    delete m_arDial;
    m_qsSession.clear();
}

const bool CAppCfg::ifQuitApp() {
    return QMessageBox::information(nullptr, QString::fromLocal8Bit("提示")
        , QString::fromLocal8Bit("是否确认退出？"),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes;
}

void  CAppCfg::initTextBrowser(void*& pTb, const int n) {
    m_pTb = pTb;
    m_nLog = n;

    QTextBrowser*& p = (QTextBrowser*&)m_pTb;
    p->document()->setMaximumBlockCount(n);
    p->setLineWrapMode(QTextBrowser::NoWrap);
}

const quint64  CAppCfg::getSNo() {

    quint64 uno = m_uSNo;
    
    m_uSNo = ++uno;

    char sDef[80];
    sprintf(sDef,"%d", (int)m_uSNo);
    ::WritePrivateProfileStringA("http", "SerialNo", sDef, this->getIniName());

   // this->setInt("http/SerialNo", m_uSNo);

    return uno;
}

int CAppCfg::newSessionIdx() {
    if (m_qsSession.isEmpty()) {
        return -1;
    }

    return m_qsSession.pop();
}
/*
bool CAppCfg::freeSessionIdx1(const int idx) {
    if (idx < 0 || idx >= m_nChannel) {
        return false;
    }

    m_qsSession.push(idx);

    return true;
}
*/
struct stTapDial* CAppCfg::getDialInfoPtr() {
    int idx = this->newSessionIdx();
    if (idx < 0) {
        return nullptr;
    }

    IwrApp*& pMain = (IwrApp*&)m_pMain;
    pMain->doShowLog(QString::fromLocal8Bit("[getDialInfoPtr] get dial info! idx = %1").arg(idx));

    struct stTapDial& td = m_arDial[idx];
    ::memset(&td, 0 ,sizeof(struct stTapDial));
    td.idx = idx;

    return  &td;
}

struct stTapDial* CAppCfg::findDialInfoPtr(const int tranId) {

    return nullptr;
}
/*
const int CAppCfg::delDialInfo(const QString& InCaller) {
    const struct stTapDial& tap = this->getDialInfo(InCaller);

    m_qsSession.push(tap.idx);

    IwrApp*& pMain = (IwrApp*&)m_pMain;
    pMain->doShowLog(QString::fromLocal8Bit("[delDialInfo] delete dial info! idx = %1, InCaller=%2").arg(tap.idx).arg(InCaller));

    return m_mapDial.remove(InCaller);
}
*/
bool CAppCfg::showLog(const QString& sLog, const bool show) {

    QTextBrowser*& pTb = (QTextBrowser*&)m_pTb;
    if (show && m_pTb) {
        pTb->append(QTime::currentTime().toString("[HH:mm:ss] ") + sLog);
    }
    
    return CAppExt::log("[SHOW] " + sLog);
}


int  CAppCfg::SendReq(QString strUrl, QString strInput, QString& strResult, const bool bPost) {
    QNetworkRequest oNetRequest;
    oNetRequest.setUrl(QUrl(strUrl));
    oNetRequest.setRawHeader("Content-Type", (bPost ? "application/x-www-form-urlencoded" : "application/json"));
    if (!bPost) {
        oNetRequest.setRawHeader("Accept", "application/json");
    }

    QNetworkAccessManager oNetAccessManager;
    oNetAccessManager.setNetworkAccessible(QNetworkAccessManager::Accessible);

    QByteArray  postData;
    if (strInput != "") {
        postData.append(strInput);
    }
    QNetworkReply* oNetReply = oNetAccessManager.post(oNetRequest, postData);

    QEventLoop loop;
    IwrApp*& pMain = (IwrApp*&)m_pMain;
    pMain->connect(oNetReply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    int httpsCode = oNetReply->attribute(QNetworkRequest::Http2DirectAttribute).toInt();
    QNetworkReply::NetworkError e = oNetReply->error();
    if (e == QNetworkReply::NoError) {
        strResult = oNetReply->readAll();
    }
    else {
        strResult = oNetReply->errorString();
        if (httpsCode == 0) {
            httpsCode = e;
        }
    }
    //this->log(QString::fromLocal8Bit("rsp8url：ret=%1,err=%2 ").arg(strResult).arg(strErrInfo));

    return httpsCode;
}

bool CAppCfg::run() {
    QString sNetIni = QString("%1..\\..\\%2").arg(this->getAppPath()).arg("network.ini");
 //   this->reload(true, sNetIni);

    if (!this->startMySql(sNetIni)) {
        return false;
    }

    if (!this->startCmm(sNetIni)) {
        return false;
    }

    if (!this->startHts()) {
        return false;
    }
    this->startHtc();

    /*
    //test
    QString sAppId,  sAppKey;
    m_pAppMy->getVrfInfo(sAppId, sAppKey);
    this->log(QString("[TRACE] appid=%1, appkey=%2").arg(sAppId).arg(sAppKey));
    */
    return true;
}

void CAppCfg::stop() {
    if (nullptr != m_pAppHtc) {
        m_pAppHtc->stop();
    }

    this->freeCmm();
    if (m_pAppHts) {
        emit m_pAppHts->readyToClose();
        QThread::sleep(2);
    }
    YQ_FREE_OBJ(m_pAppHts);

    YQ_FREE_OBJ(m_pAppHtc);
    YQ_FREE_OBJ(m_pAppMy);

  //  this->freeTapDial();
}

bool CAppCfg::startHtc() {
    CAppHtc*& pApp = m_pAppHtc;
    if (nullptr != pApp) {
        return true;
    }

    CWorkLog* pLog = this->newLog("htc");
    pApp = new CAppHtc(pLog);
    pApp->start();

    IwrApp*& pMain = (IwrApp*&)m_pMain;
    pMain->doShowLog("http gateway started !");
    // pMain->doShowLog("http gateway started -2!");

    return true;
}

bool CAppCfg::startHts() {
    if (nullptr != m_pAppHts) {
        return true;
    }
    int nTrd = this->getInt("http/thread_count", 20) , nPort = this->getInt("http/server_port", 8050) ;
    
    m_pAppHts = new JQHttpServer::TcpServerManage(nTrd, this);
    m_pAppHts->setHttpAcceptedCallback(std::bind(g_onHttpAccepted, std::placeholders::_1, this));
    const auto listenSucceed = m_pAppHts->listen(QHostAddress::Any, nPort);
    this->log(QString::fromLocal8Bit("listenSucceed=%1").arg(listenSucceed));

    IwrApp*& pMain = (IwrApp*&)m_pMain;
    if (0 == listenSucceed) {
        QMessageBox::warning(nullptr, "prompt", "http服务端监听失败");
        pMain->doShowLog(QString::fromLocal8Bit("http服务端监听失败！"));
    }
    else {
        pMain->doShowLog(QString::fromLocal8Bit("http服务端监听成功！"));
    }

    return 0 != listenSucceed;
}

bool CAppCfg::startMySql(const QString& sNetIni) {

    CAppMySql*& pApp = m_pAppMy;
    if (nullptr != pApp) {
        return true;
    }

    CWorkLog* pLog = this->newLog("mysql");
    pApp = new CAppMySql(pLog);
    pApp->start();

    bool bRet = false;
    IwrApp*& pMain = (IwrApp*&)m_pMain;
    pApp->setWaitSignal(0);
    if (pApp->waitSignal(3000, 1)) {
        QString sMsg;
        if (pApp->getDbCnt(sMsg)) {
            pMain->doShowLog(QString::fromLocal8Bit("mysql server started ! (%1)").arg(sMsg));
            bRet = true;
        }
        else {
            pMain->doShowLog(QString::fromLocal8Bit("mysql server start fail ! (%1)").arg(sMsg));
        }
    }
    else {
        pMain->doShowLog("mysql server start timeout  !");
    }
    /*
    //{
    if (bRet) {
        int nCmd = 2;
        pApp->setWaitSignal(0);
        emit pApp->doMessage(nCmd, 0, QString::fromLocal8Bit(" insert into ha_test1(fd1,fd2) values('fd16','val16测试6')"));
        if (pApp->waitSignal(3000, nCmd)) {
            bool bRet = pApp->getExecRet();
            if (bRet) {
                pMain->doShowLog(QString::fromLocal8Bit("操作成功"));
            }
            else {
                pMain->doShowLog(pApp->getErr());

            }
        }
    }
    //}
    */
    /*
    //{
    if (bRet) {
        int nCmd = 1;
        pApp->setWaitSignal(0);
        emit pApp->doMessage(nCmd,0,QString::fromLocal8Bit("select * from  ha_test1"));
        if (pApp->waitSignal(3000, nCmd)) {
            QString sRet = pApp->getVal();
            if (sRet.isEmpty()) {
                pMain->doShowLog(pApp->getErr());
            }
            else {
                pMain->doShowLog(sRet);
            }
        }
    }
    //}
    */

    return bRet;
}

bool  CAppCfg::startCmm(const QString& sNetIni) {
    pcommainManager = new TCommManager(sNetIni.toLocal8Bit().data());
    if (!pcommainManager || !pcommainManager->Run()) {
        this->log("run communication manager fail !");
        //	PrintConsole(0, "Start Communication Module Failed! Please Exit Now");
        //	ExitFromError();
        //	AfxMessageBox("Warning:Start Communication Failed!", MB_OK, 0);
        return false;  //立即退出程序
    }
    this->log("run communication manager succ !");
    
    CAppCmm*& pCmm = (CAppCmm*&)m_pAppCmm;
    pCmm = new CAppCmm(pcommainManager,this);
    if (nullptr == pCmm || !pCmm->Run()) {
        this->log("run communication moudle fail !");
        return false;  //立即退出程序
    }
    this->log("run communication moudle succ !");
    
    //this->reload(true, sNetIni);

    IwrApp*& pMain = (IwrApp*&)m_pMain;
    
    pMain->doShowLog("common module started !");

    return true;
}

void CAppCfg::freeCmm() {
    if (NULL == pcommainManager) {
        return;
    }

    pcommainManager->Stop();
    ::Sleep(1000);
    delete pcommainManager;
    pcommainManager = NULL;

    YQ_FREE_OBJ(m_pAppCmm);
    IwrApp*& pMain = (IwrApp*&)m_pMain;
    pMain->doShowLog("common module stoped !");
}

/*
void CAppCfg::freeTapDial(const int nMax) {
    int i = 0,n = nMax>0 ? m_mapDial.count() - nMax : nMax;
    struct stTapDial* ptd;
    QMap<QString, struct stTapDial*>::iterator itor;
    for (itor = m_mapDial.begin(); itor != m_mapDial.end(); ++itor){
        if (n > 0 && ++i > n) {
            break;
        }
        ptd = (struct stTapDial*)itor.value();
        delete ptd;
    }
    m_mapDial.clear();
}
*/

QString CAppCfg::getUrlRespDef(const int Status, const QString msg) {
    CJsonKit jk;
    jk.putNumber(Status, "/Status");
    jk.putVal(msg,"/msg");

    return jk.toString();
}

void CAppCfg::onHttpAccepted(const QPointer< JQHttpServer::Session >& session) {

    QString msg = QString::fromLocal8Bit("成功");
    QString sUrl = session->requestUrl(), sData = session->requestBody(),sKeyFun;
    this->log(QString::fromLocal8Bit("url(%1)data(%2)").arg(sUrl).arg(sData));

    QStringList arUrl = sUrl.split(m_sKeySep);
    int n = arUrl.length();
    if (n < 2) {
        session->replyText(this->getUrlRespDef(1050, QString::fromLocal8Bit("接口资源错误")));
        return;
    }

    QString InCallID;
    CJsonKit jk(sData);

    IwrApp*& pMain = (IwrApp*&)m_pMain;
    pMain->doShowLog(QString::fromLocal8Bit("[HTTP-REQ] url(%1)data(%2)").arg(sUrl).arg(jk.toString().replace("\n", "").replace("    ", "")));

    sKeyFun = arUrl.at(n-1);
    if ("callRequest"== sKeyFun) {
        if (jk.isErr()) {
            session->replyText(this->getUrlRespDef(1005, QString::fromLocal8Bit("请求消息格式错误")));
        }
        else {
            this->hdlCallRequest(session, jk);
        }
    }
    else if ("pullrecordingfile" == sKeyFun) {
        session->replyText(this->getUrlRespDef(200, QString::fromLocal8Bit("成功")));
    }
    else if ("pullbill" == sKeyFun) {
        this->hdlPullBill(session, jk);
    }
    else if ("reportUrl" == sKeyFun) {
        this->hdlReportUrl(session, jk);
    }
    else {
        session->replyText(this->getUrlRespDef(1050, QString::fromLocal8Bit("接口未启用")));
        return;
    }

}

QString CAppCfg::md5(const QString& sSign) {
    QByteArray btArray = QCryptographicHash::hash(sSign.toLocal8Bit(), QCryptographicHash::Md5);
    QString sMd5;
    sMd5.append(btArray.toHex());

    return sMd5;
}

bool CAppCfg::chkSign(const QPointer< JQHttpServer::Session >& session, CJsonKit& jk, int& nCode, QString& sMsg, QString& sAppKey) {
    if (0 == this->getInt("http/chk_sign", 1)) {
        return true;
    }


    QStringList slSign;
    slSign << "incallid" << "incaller" << "incalled" << "serviceid" << "callershow" << "servicetype" << "calltype" << "inparam" << "reporturl" << "appId" << "timestamp";
    slSign.sort();

    QString sSign = "",sKey, sKey1, sVal;
    for (int i = 0; i < slSign.count(); ++i) {
        sKey = slSign.at(i);
        sKey1 = "/" + sKey;

        if ("timestamp" == sKey) {
            sVal = QString("%1").arg(jk.getLong(sKey1));
        }
        else {
            sVal = jk.getStr(sKey1);
            if (sVal.trimmed().isEmpty()) {
                continue;
            }
        }
        if (i > 0) {
            sSign += "&";
        }
        sSign += sKey + "=" + sVal;
    }
    sSign += "&appKey=" + sAppKey;
    this->log(QString("[TRACE] sign=%1").arg(sSign));
    sSign = md5(sSign).toUpper() ;
    this->log(QString("[TRACE] md5-self=%1,md5-other=%2").arg(sSign).arg(jk.getStr("sign")));
    if (sSign != jk.getStr("sign")) {
         nCode = -4;
         sMsg = QString::fromLocal8Bit("签名错误");
    }

    return 200 == nCode ;
}

bool CAppCfg::chkCommon(const QPointer< JQHttpServer::Session >& session, CJsonKit& jk, int& nCode,QString& sMsg, QString& sAppKey) {

  //  QString sAppId = this->getVal("http/appId").trimmed() ;

    QString sAppId;
    m_pAppMy->getVrfInfo(sAppId, sAppKey);
    //this->log(QString("[TRACE] appid=%1, appkey=%2").arg(sAppId).arg(sAppKey));

    if (200 == nCode && jk.getStr("/appId") != sAppId) {
        nCode = -1;
        sMsg = QString::fromLocal8Bit("appId不存在或不正确");
    }

    qint64 uStamp = jk.getLong("/timestamp");
    if (200 == nCode && jk.getLong("/timestamp")<10000) {
        nCode = 1004;
        sMsg = QString::fromLocal8Bit("timestamp不存在");
    }
    qint64 lCur = QDateTime::currentMSecsSinceEpoch();
    if (200 == nCode && lCur - uStamp > 60000*5) {
        nCode = -3;
        sMsg = QString::fromLocal8Bit("请求超时");
    }

    return 200 == nCode ;
}

void  CAppCfg::saveDial(struct stTapDial* td) {
    m_vcDial.append(td);
}

struct stTapDial* CAppCfg::findDial(const int tranId,const bool bDel) {
    int i = 0, n ;
    struct stTapDial* pTd = nullptr,*p;

    n = m_vcDial.size();
    for (i = 0; i < n; ++i) {
        p = m_vcDial.at(i);
        if (tranId == p->tranID) {
            pTd = p;
            if (bDel) {
                m_vcDial.remove(i);
            }
            break;
        }
    }

    return pTd;
}

bool CAppCfg::freeSesionIdx(const int idx) {
    if (idx < 0 || idx >= m_nChannel) {
        return false;
    }

    m_qsSession.push(idx);

    return true;
}

int CAppCfg::freeDial(const int tranId) {
    int nRet = -1;

    struct stTapDial* p = this->findDial(tranId, true);
    if (nullptr != p) {
        nRet = (int)p->tranID;
        this->freeSesionIdx(p->idx);

        IwrApp*& pMain = (IwrApp*&)m_pMain;
        pMain->doShowLog(QString::fromLocal8Bit("[CAppCfg] free dial info! idx = %1").arg(p->idx));
    }

    return nRet;
}

struct stTapDial*  CAppCfg::chkCall(const QPointer< JQHttpServer::Session >& session, CJsonKit& jk) {
    struct stTapDial* pRet =  nullptr;
    int nCode = 200;
    QString sMsg = QString::fromLocal8Bit("成功");;

    if (200 == nCode && jk.getStr("/incallid").isEmpty()) {
        nCode = 1006;
        sMsg = QString::fromLocal8Bit("请求消息参数有误(not incallid)");
    }
    if (200 == nCode && jk.getStr("/incaller").isEmpty()) {
        nCode = 1006;
        sMsg = QString::fromLocal8Bit("请求消息参数有误(not incaller)");
    }
    if (200 == nCode && jk.getStr("/incalled").isEmpty()) {
        nCode = 1006;
        sMsg = QString::fromLocal8Bit("请求消息参数有误(not incalled)");
    }

    QString sAppKey;
    this->chkCommon(session,jk, nCode, sMsg, sAppKey);
    if (200 == nCode) {
        this->chkSign(session, jk, nCode, sMsg, sAppKey);
    }

    if (200 == nCode) {
        pRet = this->getDialInfoPtr();
        if (nullptr == pRet) {
            nCode = 1007;
            sMsg = QString::fromLocal8Bit("申请会话资源失败");
        }
        else {
            this->saveDial(pRet);
        }
    }

    session->replyText(this->getUrlRespDef(nCode, sMsg));

    return pRet ;
}

bool CAppCfg::hdlCallRequest(const QPointer< JQHttpServer::Session >& session, CJsonKit& jk) {
    struct stTapDial* pdi = chkCall(session,jk);
    if (nullptr == pdi) {
        return false;
    }
    QString InCallID = jk.getStr("/incallid");
    QString InCaller = jk.getStr("/incaller");

    quint64 sno = this->getSNo();

    //struct stTapDial* ptd = new struct stTapDial;
    //::memset((void*)ptd, 0, sizeof(struct stTapDial));

    //struct stTapDial& td = *ptd ;
    struct stTapDial& td = *pdi;
    strcpy(td.InCallID, InCallID.toLocal8Bit().constData());
    strcpy(td.InCaller, InCaller.toLocal8Bit().constData());
    strcpy(td.InCalled, jk.getStr("/incalled").toLocal8Bit().constData());
    strcpy(td.InCallerShow, jk.getStr("/callershow").toLocal8Bit().constData());
    td.ServerType = 1;
    td.ServerType = jk.getInt("/servertype", &td.ServerType);
    td.CallType = jk.getInt("/calltype");
    //strcpy(td.CallType , jk.getStr("/CallType").toLocal8Bit().constData());
    strcpy(td.InParam, jk.getStr("/inparam").toLocal8Bit().constData());
    strcpy(td.ReportUrl, jk.getStr("/reporturl").toLocal8Bit().constData());
    strcpy(td.sign_name, jk.getStr("/sign").toLocal8Bit().constData());
    td.timestamp = QDateTime::currentMSecsSinceEpoch();

    strcpy(td.appId, jk.getStr("/appId").toLocal8Bit().constData());
    strcpy(td.ServiceId, jk.getStr("/serviceid", &m_sServiceId).toLocal8Bit().constData());
    //strcpy(td.ServiceId, m_sServiceId.toLocal8Bit().constData());

    td.tranID = sno;
    td.stpHookOffCalled = 0;
    td.stpHookOffCaller = 0;
    //   this->freeTapDial(m_nChannel);

    //QString sTranID = QString("%1").arg(sno);
   // m_mapDial.insert(sTranID, ptd);


    this->saveBill(pdi);

    IwrApp*& pMain = (IwrApp*&)m_pMain;
    emit pMain->doPredialCall(pdi);


 //   this->freeDial(sno);

    return true;
}


bool CAppCfg::hdlPullBill(const QPointer< JQHttpServer::Session >& session, CJsonKit& jk) {
    bool bRet = false;

    this->log(QString::fromLocal8Bit("[CAppCfg::hdlPullBill] %1").arg(jk.toString()));

    IwrApp*& pMain = (IwrApp*&)m_pMain;
    CAppMySql*& pApp = m_pAppMy;
    QString tbn = pApp->tbn("call_record")
        , fds = QString(
            "INCALLID as incallid,INCALLER as incaller,INCALLED as incalled,CALLERSHOW as incallershow,SERVICETYPE as Servertype"
            ",CALLTYPE as Calltype,INCALLED as Outcaller,INCALLED as Outcallershow,INCALLED as Outcalled"
            ",CONVERT(UNIX_TIMESTAMP(CALLTIME),signed) as Instarttime,CONVERT(UNIX_TIMESTAMP(CALLERHOOKOFFTIME),signed) as outhookoftime"
            ",CONVERT(UNIX_TIMESTAMP(CALLTIME),signed) as outstartTime"",CONVERT(UNIX_TIMESTAMP(CALLERSTOPTIME),signed) as outstopTime"
            ",CALLERHOLDTIME as Holdtime,RATEYPE as rateType,FEE as Fee,RECORDFILENAME as Recordfilename"
        )
        , cdt = this->getCdtRpt(jk)
        , sql
        ;

    sql = QString::fromLocal8Bit("select %1 from %2 where %3").arg(fds).arg(tbn).arg(cdt);

    CJsonKit jkRec;
    int nCmd = 1, nRec = 0;
    pApp->setWaitSignal(0);
    emit pApp->doMessage(nCmd, 1, sql);
    if (pApp->waitSignal(8000, nCmd)) {
        QString sRet = pApp->getVal(nRec);
        if (sRet.isEmpty() || nRec < 1) {
            pMain->doShowLog(pApp->getErr());

            jkRec.putVal("1001", "/status");
            jkRec.putVal(QString::fromLocal8Bit("话单不存在"), "/msg");
        }
        else {
            bRet = true;
            jkRec.putVal("0000", "/status");
            jkRec.putVal(QString::fromLocal8Bit("成功"), "/msg");

            CJsonKit jkData(sRet);
            jkRec.putObj(jkData.getVal("/data"), "/data");
        }
    }
    else {
        jkRec.putVal("1001", "/status");
        jkRec.putVal(QString::fromLocal8Bit("话单不存在"), "/msg");
    }

    session->replyText(jkRec.toString());

    return bRet;
}

void CAppCfg::saveBill(struct stTapDial* ptd) {
    IwrApp*& pMain = (IwrApp*&)m_pMain;
    CAppMySql*& pApp = m_pAppMy;

    /*
    QString 
        tbn = pApp->tbn("bill")
        ,fds = "status,msg,incallid,incaller,incalled,incallershow,Servertype,Calltype"
            ",Outcaller,Outcallershow,Outcalled,Instarttime,outhookoftime,outstartTime,outstopTime,Holdtime"
            ",rateType,Fee,Recordfilename"
        ,vls = QString::fromLocal8Bit("'%1','%2','%3','%4','%5','%6',%7,%8,'%9','%10','%11','%12','%13','%14','%15',%16,%17,%18,'%19'")
            .arg("1099").arg(QString::fromLocal8Bit("请求成功"))
            .arg(ptd->InCallID).arg(ptd->InCaller).arg(ptd->InCalled).arg(ptd->InCallerShow)
            .arg(ptd->ServerType).arg(ptd->CallType).arg(" ").arg(" ").arg(" ").arg(" ").arg(" ").arg(" ").arg(" ")
            .arg(0).arg(0).arg(0.0).arg(" ")


        ,sSql = QString::fromLocal8Bit("insert into %1(%2) values(%3)").arg(tbn).arg(fds).arg(vls)
        ;
    */
    QDateTime callTime = QDateTime::fromMSecsSinceEpoch(ptd->timestamp);
    char sCallTime[80];
    strcpy(sCallTime, callTime.toString("yyyy-MM-dd HH:mm:ss").toLocal8Bit().constData());

    QString
        tbn = pApp->tbn("call_record") , fds , vls , sSql 
        ;
    const struct stItemDisc std_record[] = {
         {"APPID", ptd->appId, 0}, { "INCALLID",ptd->InCallID,0 }, { "INCALLER",ptd->InCaller,0 }
         , { "INCALLED",ptd->InCalled,0 }, { "CALLERSHOW",ptd->InCallerShow,0 }, { "SERVICEID",ptd->ServiceId,0 }
        , { "SERVICETYPE",QString("%1").arg(ptd->ServerType),1 }, { "CALLTYPE",QString("%1").arg(ptd->CallType),1 }, { "CALLTIME",sCallTime,0}
        ,{ "CALLERHOOKOFFTIME","",0 }, { "CALLEDHOOKOFFTIME","",0 }, { "CALLERSTOPTIME","",0 }
        , { "CALLEDSTOPTIME","",0 }, { "CALLERHOLDTIME","0",1 }, { "CALLEDHOLDTIME","0",1 }
        , { "RATEYPE","0",1 }, { "FEE","0",1 }, { "RECORDFILENAME","",0 }, { "INPARAM",ptd->InParam,0 }
    };
    const int n_record = sizeof(std_record) / sizeof(std_record[0]);
    pApp->mergeRec(vls, fds, std_record, n_record, 0);
    sSql = QString::fromLocal8Bit("insert into %1(%2) values(%3)").arg(tbn).arg(fds).arg(vls);

    int nCmd = SQL_EXEC;
    pApp->setWaitSignal(0);
    emit pApp->doMessage(nCmd, 0, sSql);
    if (pApp->waitSignal(3000, nCmd)) {
        bool bRet = pApp->getExecRet();
        if (bRet) {
            pMain->doShowLog(QString::fromLocal8Bit("saveBill succ"));
        }
        else {
            pMain->doShowLog(QString::fromLocal8Bit("saveBill fail (%1)").arg(pApp->getErr()));
        }
    }

}

QString CAppCfg::getCdtRpt(CJsonKit& jk) {
    QString sRet = "";
    QString InCallID = jk.getStr("/InCallID");
    if (InCallID.isEmpty()) {
        qint64 nCur = QDateTime::currentSecsSinceEpoch();
        QString endTime = jk.getStr("/endTime");
        if (endTime.isEmpty()) {
            endTime = QString("%1").arg(nCur) ;
        }

        QString startTime = jk.getStr("/startTime");
        if (startTime.isEmpty()) {
            startTime = QString("%1").arg(nCur - m_nRptTime);
        }
        sRet = QString("CONVERT(UNIX_TIMESTAMP(CALLTIME),signed)>=%1 and CONVERT(UNIX_TIMESTAMP(CALLTIME),signed)<=%2").arg(startTime).arg(endTime);
    }
    else {
        sRet = QString("INCALLID='%1'").arg(InCallID);
    }

    return sRet;
}

bool CAppCfg::hdlReportUrl(const QPointer< JQHttpServer::Session >& session, CJsonKit& jk) {
    

    this->log(QString::fromLocal8Bit("[CAppCfg::hdlReportUrl] %1").arg(jk.toString()));

    session->replyText(jk.toString());

    return true;
}