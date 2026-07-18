#pragma once

#include <QSplitter>
#include <QStack>
#include <QVector>
#include <appExt.h>

#include <JQHttpServer>
#include <JsonKit.h>
#include <bufPool.h>

#include "tapDial.h"
#include "CommManager.h"
#include "appCmm.h"
#include "appHtc.h"
#include "appMySql.h"

//IwrApp
class CAppCfg : public CAppExt{
public:
	CAppCfg(const int& argc0, const char**& argv0);
	~CAppCfg();

    void setMain(void* p) { m_pMain = p; }
    void* getMain() { return m_pMain; }
    bool showLog(const QString& sLog,const bool show = false);
	bool run();
	void stop();
    const bool ifQuitApp();
    void initTextBrowser(void*& pTb, const int n = 100);
    int  SendReq(QString strUrl, QString strInput, QString& strResult, const bool bPost = true);

    const quint64  getSNo();
    CAppHtc*& getHtc() { return m_pAppHtc; }
    CAppCmm*& getCmm() { return m_pAppCmm; }
    CAppMySql*& getMy() { return m_pAppMy; }
    //const struct stTapDial& getDialInfo(const QString& InCaller) { struct stTapDial tap = m_mapDial.value(InCaller);  return tap ; }
    //const struct stTapDial& getDialInfo(const QString& InCaller) { const struct stTapDial& tap = m_mapDial.value(InCaller);  return tap; }
    const struct stTapDial& getDialInfo(const QString& InCaller) { const struct stTapDial& tap = *m_mapDial.value(InCaller);  return tap; }
    //const int delDialInfo(const QString& InCaller);
    struct stTapDial* findDial(const int tranId, const bool bDel = false);
    int freeDial(const int tranId);

    QString getUrlRespDef(const int Status,const QString msg);
   // QString getPullBill();
    QString getCdtRpt(CJsonKit& jk);
    QString md5(const QString& sSign);

    void onHttpAccepted(const QPointer< JQHttpServer::Session >& session);
private:
    void* m_pMain;
    void* m_pTb;

    QString m_sPathOrig,m_sKeySep;

	TCommManager* pcommainManager;
    CAppCmm* m_pAppCmm;
    CAppHtc* m_pAppHtc;
    CAppMySql* m_pAppMy;
    JQHttpServer::TcpServerManage* m_pAppHts;
    QMap<QString, struct stTapDial*> m_mapDial;
    
    struct stTapDial* m_pDialInfoNull;
    struct stTapDial* m_arDial;
    QStack<int> m_qsSession ;
    QVector<struct stTapDial*> m_vcDial;

    int m_nRptTime;
    int  m_nChannel;
    int   m_nLog;
    quint64  m_uSNo;
    QString m_sServiceId;

    int newSessionIdx();
    bool freeSesionIdx(const int idx);

    struct stTapDial* getDialInfoPtr();
    struct stTapDial* findDialInfoPtr(const int tranId);

    bool startMySql(const QString& sNetIni);
    bool startCmm(const QString& sNetIni);
    bool startHtc();
    bool startHts();
    

    void freeCmm();
 //   void freeTapDial(const int nMax=0);

    void saveBill(struct stTapDial* ptd);

    bool chkSign(const QPointer< JQHttpServer::Session >& session, CJsonKit& jk, int& nCode, QString& sMsg, QString& sAppKey);
    bool chkCommon(const QPointer< JQHttpServer::Session >& session, CJsonKit& jk, int& nCode, QString& sMsg, QString& sAppKey);
    struct stTapDial*  chkCall(const QPointer< JQHttpServer::Session >& session, CJsonKit& jk);
    void saveDial(struct stTapDial* td);


    bool hdlCallRequest(const QPointer< JQHttpServer::Session >& session, CJsonKit& jk);
    bool hdlPullBill(const QPointer< JQHttpServer::Session >& session, CJsonKit& jk);
    bool hdlReportUrl(const QPointer< JQHttpServer::Session >& session, CJsonKit& jk);

};