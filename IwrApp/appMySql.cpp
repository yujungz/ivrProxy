#include "IwrApp.h"


const struct stItemDisc std_call_record[] = {
     {"APPID", "234", 1}, { "fd2","val2",0 }
};
const int n_call_record = sizeof(std_call_record) / sizeof(std_call_record[0]);

CAppMySql::CAppMySql(CWorkLog*& pLog)
	: CWorkTrd(pLog)
{
 //   connect(this, &CAppHtc::doSend2User, this, &CAppHtc::onSend2User, Qt::QueuedConnection);
    m_bDbCnt = false;
    m_bNone = false;

    m_sDbMsg =
    m_sPrefix = "http_";
   // m_sPrefix = "iwr_";
    m_pQr = nullptr;
}

CAppMySql::~CAppMySql()
{

    YQ_FREE_OBJ(m_pQr);

    QSqlDatabase& db = m_db;
    if (db.isOpen()) {
        db.close();
        this->log(QString("mysql is closed !"));
    }

}

void CAppMySql::run() {
//    this->log(QString("CAppMySql::run !"));
    //QMYSQL
    CAppCfg*& pCfg = gpCfg;

    m_sPrefix = pCfg->getStr("mysql/prefix", m_sPrefix);

    QString sDrv = pCfg->getStr("mysql/drv");
    if (sDrv.isEmpty()) {
        m_bDbCnt = true;
        m_bNone = true;
        m_sDbMsg = "none";
    }
    else {
        QSqlDatabase& db = m_db;
        /*
        //{
        QStringList dls = db.drivers();
        int i, n = dls.length();
        QString str;
        for (i = 0; i < n; ++i) { str += dls.at(i) + "|"; }
        //}
        */
        db = QSqlDatabase::addDatabase(sDrv);
        db.setHostName(pCfg->getStr("mysql/host", "127.0.0.1"));
        db.setPort(pCfg->getInt("mysql/port", 3306));
        db.setUserName(pCfg->getStr("mysql/uid", "root"));
        db.setPassword(pCfg->getStr("mysql/pwd", "hssql123456"));
        db.setDatabaseName(pCfg->getStr("mysql/dbn", "hsai"));
        m_bDbCnt = db.open();
        if (m_bDbCnt) {
            m_sDbMsg = "ok";
            m_pQr = new QSqlQuery(db);
        }
        else {
            m_sDbMsg = db.lastError().text();
        }
    }

    this->setWaitSignal(1);
}

bool CAppMySql::handleMessage(const int nCmd, const int nPrm, const QString sPrm) {
    
//    this->log(QString::fromLocal8Bit("CAppMySql::handleMessage:nCmd(%1)nPrm(%2)sPrm(%3)").arg(nCmd).arg(nPrm).arg(sPrm));
    CAppCfg*& pCfg = gpCfg;
    switch (nCmd) {
    case SQL_QUERY:
        m_sVal = this->getRows(sPrm,(nPrm==0 ? false : true));
        this->setWaitSignal(nCmd);
        break;
    case SQL_EXEC:
        m_bExec = this->execSql(sPrm);
        this->setWaitSignal(nCmd);
        break;
    }

    return true;
}

QString CAppMySql::getRows(const QString& sSql, const bool assoc) {
    QString sRet = "";
    if (nullptr == m_pQr) {
        m_sErr = "server is not run";
        return sRet;
    }
    QSqlDatabase& db = m_db;
    QSqlQuery& qr = *m_pQr;
    bool bExec = qr.exec(sSql);
    if (bExec) {        
        CJsonKit jk;
        QSqlRecord rec = qr.record();
        int i,j,nFd = rec.count();
        for (i = 0; i < nFd; ++i) {
            jk.putVal(rec.fieldName(i),"/head/");
        }
        j = 0;
        while (qr.next()) {
            for (i = 0; i < nFd; ++i) {
                if (assoc) {
                    jk.putVal(qr.value(i).toString(), QString("/data/%1/%2").arg(j).arg(rec.fieldName(i)));
                }
                else {
                    jk.putVal(qr.value(i).toString(), QString("/data/%1/").arg(j));
                }
            }
            ++j;
        }
        qr.finish();
        sRet = jk.toString();
        m_nRec = j;
    }
    else {
        m_sErr = qr.lastError().text();
    }

    return sRet;
}

bool CAppMySql::execSql(const QString& sSql, const bool bClose) {
    if (nullptr == m_pQr) {
        m_sErr = "server is not run";
        return false;
    }

    QSqlQuery& qr = *m_pQr;
    bool bOk = qr.exec(sSql);
    if (bOk) {       
        m_nLastId = qr.lastInsertId().toInt();
        if (bClose) {
            qr.finish();
        }
    }else{
        m_sErr = qr.lastError().text();
    }

    return bOk;
}

int CAppMySql::mergeRec(QString& sVal, QString& sFds,const struct stItemDisc sid[],const int nItem, const int nFlag) {
    int i, n = nItem;

    sVal = "";
    sFds = "";
    for (i = 0; i < n; ++i) {
        if (i > 0) {
            sVal += ",";
            if (0 == nFlag) {
                sFds += ",";
            }
        }
        if (0 != nFlag) {
            sVal += sid[i].fdn+"=";
        }

        if (0 == sid[i].dataType) {
            sVal += "'";
        }
        sVal += sid[i].val;
        if (0 == sid[i].dataType) {
            sVal += "'";
        }
        if (0 == nFlag) {
            sFds += sid[i].fdn;
        }
    }

    return 0;
}

int CAppMySql::getVrfInfo(QString& sAppId, QString& sAppKey) {
    int nRet = 0;

    sAppId = "12345678";
    sAppKey = "abcdefg";

    CAppCfg*& pCfg = gpCfg;
    sAppId = pCfg->getVal("http/appId", sAppId);
    sAppKey = pCfg->getVal("http/appKey", sAppKey);

    QString 
        sVal
       , fds = "appid, appkey"
       , tbn = this->tbn("user")
       , cdt = " order by id desc  limit 1"
       , sSql = QString::fromLocal8Bit("select %1 from %2 %3").arg(fds).arg(tbn).arg(cdt)
       ;
    sVal = this->getRows(sSql,true);

   // int n = this->getCount();
    if (this->getCount() > 0) {
        CJsonKit jk(sVal);
        sAppId = jk.getStr("/data/0/appid", &sAppId);
        sAppKey = jk.getStr("/data/0/appkey", &sAppKey);
    }

    return nRet;
}