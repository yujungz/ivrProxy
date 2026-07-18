#pragma once

#include <QtWidgets/QApplication>
#include <QSettings>
#include <QSystemSemaphore>
#include <QSharedMemory>

#include <cmmdef.h>
#include <WorkLog.h>

class CAppExt{
public:
	CAppExt(const int& argc0, const char**& argv0,const char* prm="",const char* key="",const char* val="", const char* ini="");
	~CAppExt();

    const bool isBlank(const char* s) { return NULL == s || '\0' == s[0]; }
    const int  str_len(const char* s);
    const int  str_cat(char* t, const char* s, const int idx = 0,const int len = 0);
    void       str_rvs(char* t, const char* s = NULL);
    const int  str_pos(const char* s,const char* t);

    void setAppName(const char* sApp) { m_sAppName = QString::fromLocal8Bit("%1").arg(sApp); }
    const QString& getAppName() { return  m_sAppName; }
    const QString& getLogDir() { return  m_sLogDir; }
    const QString getAppPath() { return  QString::fromLocal8Bit("%1").arg(m_sPathFull); }
    const QString getAppDir() { return  QString::fromLocal8Bit("%1").arg(m_sDirFull); }
    const QString getSplStr() { return  QString::fromLocal8Bit("%1").arg(m_sSpl);  }

    const char* getPrgName() { return m_sPrgName; }
    const char* getIniName() { strcpy(m_sIniFileName, m_sIni.toLocal8Bit().constData()); return m_sIniFileName; }
    const QString& getRunVal() { return m_sVal; }

    void splitModuleName(const char* prgName, char& sSpl, char*& sNameDrv, char*& sNameDir, char*& sNamePath
        , char*& sDirFull, char*& sPathFull, char*& sNameApp, char*& sNameExt);
    CWorkLog* newLog(const QString& sLog = "");
    const QString get_cpuId();
    bool testReload(const bool bPmt = true);
    bool log(const QString& sLog);

    //{ ini
    void reload(const bool& bRel = true,const QString& sIni = "");
    const QString getVal(const QString& key, const QString& val = "");
    const QString getStr(const QString& key, const QString& val = "") { return  this->getVal(key, val); }
    const int getInt(const QString& key, const int& val = 0) { return  this->getVal(key, QString("%1").arg(val)).toInt(); }
    const double getDouble(const QString& key, const double& val = 0) { return  this->getVal(key, QString("%1").arg(val)).toDouble(); }
    void  setVal(const QString& key, const QString& val) {
        QSettings*& pIni = m_pIni;
        pIni->setValue(key, val);
    }
    void  setInt(const QString& key, const int& val) {
        QSettings*& pIni = m_pIni;
        pIni->setValue(key, QString("%1").arg(val));
    }
    void  setDouble(const QString& key, const int& val) {
        QSettings*& pIni = m_pIni;
        pIni->setValue(key, QString("%1").arg(val));
    }
    //}
    //{
    int  countLimit(const int n,const int nMin, const int nMax);
    int  bufInit(uchar**& ucBuf, const int nBuf = 1);
    void bufFree(uchar**& ucBuf, const int nBuf = 1,const bool bAll=true);
    uchar* bufNew(uchar**& ucBuf, quint64& nSizeCur, const quint64 nSizeNew, const int nBuf = 1, const int iBuf = 0);
    //}

    int    argc;
    char** argv;
private:
    int  m_argcOld;
    char* m_sPrgName,m_sSpl;
    char* m_sNameExt,*m_sNameApp, *m_sNameDir, *m_sNamePath, *m_sNameDrv;
    char* m_sDirFull, * m_sPathFull, *m_sPathPrt;
    char m_sIniFileName[300];
        
    QString    m_sIni,m_sVal,m_sAppName,m_sLogDir;
    QSettings *m_pIni;
    CWorkLog  *m_pLog;
    QMap  <QString, CWorkLog*> m_logs;

    QSystemSemaphore* m_pSema;
    QSharedMemory* m_pMem;
     
    void free();
    const int  newArgv(const char* prm,const char* val);
    void freeArgv();
	
    void savePrm (char*& t, char*&  s,int& j);
    void savePath(char*& t, const char*& s, const int& j, const char& spl);

    void getLogName(char* t, char* d = NULL, const char* s = NULL);
    int  createPath(const char* sLogName);

    void initIni(QString& sVal, const QString& sKey = "", const QString& sIni = "");
    void freeIni();
 
    void freeLog();
    const bool makeDir(const QString& sDir);
    void getcpuid(const unsigned int CPUInfo[4], const unsigned int& InfoType);
   
};