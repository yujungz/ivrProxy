#pragma once

#include <QSqlDatabase>
#include <QSqlError> 
#include <QSqlQuery>
#include <QSqlRecord>
#include <WorkTrd.h>

#define  SQL_QUERY      1
#define  SQL_EXEC       2

struct stItemDisc {
    QString fdn;
    QString val;
    int     dataType;
};

class CAppMySql  : public CWorkTrd
{
	Q_OBJECT

public:
    CAppMySql(CWorkLog*& pLog);
	~CAppMySql();

    bool getDbCnt(QString& sMsg) { sMsg = m_sDbMsg; return  m_bDbCnt; }
    void setPrefix(const QString sPre) { m_sPrefix = sPre; }
    const QString tbn(const QString& tbnBase) { return  m_sPrefix + tbnBase ; }
    QString getRows(const QString& sSql,const bool assoc = false);
    bool    execSql(const QString& sSql,const bool bClose = true);
    QString getErr() { return m_sErr; }    
    QString getVal(int& nRec) { nRec = m_nRec; return m_sVal; }
    int getCount() { return m_nRec; }
    const int getLastId() const { return m_nLastId; }
    bool getExecRet() { return  m_bExec; }
    int mergeRec(QString& sVal, QString& sFds, const struct stItemDisc sid[], const int nItem,const int nFlag = 0);
    int getVrfInfo(QString& sAppId, QString& sAppKey);
protected:
    virtual void run();
    virtual bool handleMessage(const int nCmd, const int nPrm, const QString sPrm);

private:
    QSqlDatabase m_db;
    QSqlQuery *m_pQr ;

    bool m_bDbCnt,m_bExec,m_bNone;
    QString m_sDbMsg, m_sPrefix , m_sErr, m_sVal;
    int m_nLastId,m_nRec;

public slots:

signals:

};


extern const struct stItemDisc std_call_record[];
extern const int n_call_record;