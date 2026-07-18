#include "appExt.h"
#include <windows.h>
#include <QThread>
#include <QDir>
#include <QMessageBox>

HMODULE GetSelfModuleHandle() {
    MEMORY_BASIC_INFORMATION mbi;
    if (::VirtualQuery(GetSelfModuleHandle, &mbi, sizeof(mbi)) == 0)
        return nullptr;

    return (HMODULE)mbi.AllocationBase;
}

CAppExt::CAppExt(const int& argc0, const char**& argv0,const char* prm,const char* key,const char* val, const char* ini)
	: argc(argc0), argv((char**& )argv0), m_argcOld(argc0), m_sSpl('\\')//, m_sPrgName((char*&)argv0[0])
    , m_sNameExt(NULL), m_sNameApp(NULL), m_sNameDir(NULL), m_sNamePath(NULL), m_sNameDrv(NULL)
    , m_sDirFull(NULL), m_sPathFull(NULL)
{
    m_pSema = nullptr;
    m_pMem = nullptr;

    m_sPathPrt = new char[200];
    m_sPathPrt[0] = '\0';
    str_cat(m_sPathPrt, "..\\..");     

    m_sPrgName = new char[MAX_PATH + 1];
    if (argv[0] == NULL || argv[0][0] == '\0') {
        ::GetModuleFileNameA(::GetSelfModuleHandle(), m_sPrgName, MAX_PATH);
        argv[0] = m_sPrgName;
    }
    else {
        strcpy(m_sPrgName, argv[0]);
    }
    this->splitModuleName(m_sPrgName, m_sSpl, m_sNameDrv, m_sNameDir, m_sNamePath
        , m_sDirFull, m_sPathFull, m_sNameApp, m_sNameExt);

    QString sVal = val;
    this->initIni(sVal,key,ini);    
	this->newArgv(prm, sVal.toLatin1().data());

    if (1 == this->getInt("sys/log",1)) {
        m_pLog = this->newLog();
    }
    
}


CAppExt::~CAppExt(){
    
    this->freeArgv();
    this->freeLog();
    this->freeIni();
    this->free();
}

void CAppExt::free() {
    delete[] m_sNameExt, m_sNameApp, m_sNameDir, m_sNamePath, m_sNameDrv;
    delete[] m_sDirFull, m_sPathFull, m_sPrgName, m_sPathPrt;

    YQ_FREE_OBJ(m_pMem);
    YQ_FREE_OBJ(m_pSema);
}

void CAppExt::splitModuleName(const char* prgName, char& sSpl, char*& sNameDrv, char*& sNameDir, char*& sNamePath
    , char*& sDirFull, char*& sPathFull, char*& sNameApp, char*& sNameExt) {
    int i = this->str_len(prgName), j = 0, jp = 0;
    char* p = new char[(quint64)i + 1], * pp = new char[(quint64)i + 1], spl;
    bool bPoint = false
        , bApp = false
        , bDir = false
        , bDrv = false
        , bChar
        ;
    while (--i >= 0) {
        bChar = false;
        if (!bPoint && '.' == prgName[i]) {
            bPoint = true;
            this->savePrm(sNameExt, p, j);
        }
        else if (!bApp && ('/' == prgName[i] || '\\' == prgName[i])) {
            bApp = true;
            this->savePrm(sNameApp, p, j);
            jp = 0;
            spl = prgName[i];
            sSpl = spl;
        }
        else if (bApp && !bDir && (':' == prgName[i] || 0 == i)) {
            bDir = true;
            p[j] = '\0';
            this->savePath(sNamePath, (const char*&)p, j, spl);
            this->savePrm(sNameDir, p, j);
            if (':' == prgName[i]) {
                pp[jp++] = prgName[i];
            }
        }
        else if (bDir && !bDrv) {
            bDrv = true;
            if (0 == i) {
                p[j++] = prgName[i];
                pp[jp++] = prgName[i];
            }
            this->savePrm(sNameDrv, p, j);

            pp[jp] = '\0';
            this->savePath(sPathFull, (const char*&)pp, jp, spl);
            this->savePrm(sDirFull, pp, jp);
        }
        else {
            if (!bPoint || (bPoint && !bApp) || (bApp && !bDir) || (bDir && !bDrv)) {
                bChar = true;
            }
        }

        if (bChar) {
            p[j++] = prgName[i];
            pp[jp++] = prgName[i];
        }
    }

    delete[] p, pp;
}

//{tools
const int CAppExt::str_len(const char* s) {
    int n = 0;
    if (s == NULL) {
        return n;
    }

    while ('\0' != s[n]) ++n;

    return n;
}

//idx<0 ,等于strcpy
const int  CAppExt::str_cat(char* t, const char* s, const int idx, const int len) {
    int i0 = idx<0 ? 0 : idx, i = i0, j = 0  ;

    if (idx < 0) {
        *t = '\0';
    }
    else {
        j = this->str_len(t);
    }
    while (NULL!=s && s[i]) {
        t[j++] = s[i++];
        if (len > 0 && i - i0 >= len) {
            break;
        }

    }
    t[j] = '\0';

    return j;
}

void CAppExt::str_rvs(char* t, const char* s) {

    int n = this->str_len(s), i;
    if (s == NULL || s == t) {
        char c;
        for (i = 0; i < n / 2; ++i) {
            c = t[i];
            t[i] = t[n - 1 - i];
            t[n - 1 - i] = c;
        }
    }
    else {
        for (i = 0; i < n; ++i) {
            t[i] = s[n - 1 - i];
        }
    }
}

const int  CAppExt::str_pos(const char* s, const char* t) {
    int ret = -1, i = 0, j;
    bool bl;
    if (s == NULL || *s == '\0') {
        return ret;
    }

    while ('\0' != s[i]) {
        j = 0;
        bl = true;
        while ('\0' != t[j] && '\0' != s[i + j] && s[i + j] != t[j++]) {
            bl = false;
            break;
        }
        if (bl) {
            return i;
        }
        ++i;
    }

    return ret;
}
//}

void  CAppExt::savePrm(char*& t,  char*& s, int& j) {
    s[j] = '\0';
    if (j > 0) {
        t = new char[(quint64)j + 1];
        this->str_rvs(t, s);
        t[j] = '\0';
        j = 0;
    }
}

void CAppExt::savePath(char*& t, const char*& s, const int& j, const char& spl) {
    if (j < 1) {
        return;
    }
    t = new char[(quint64)j + 2];
    this->str_rvs(t, s);
    t[j] = spl ;
    t[j + 1] = '\0';
}

const int  CAppExt::newArgv(const char* prm, const char* val) {
    /*
    if ('\0' == this->argv[0][0]) {
        str_cat(this->argv[0], m_sPrgName);
    }
    */
    if (NULL == prm || NULL == val || '\0' == *prm || '\0' == *val) {
        return this->argc;
    }

    const int   nMax = 1024;

    char** argv0 = this->argv;

    bool bExist = true;
    int i, j, k;

    for (i = 0; i < argc; ++i) {
        bExist = true;  j = 0;
        while (argv[i][j] != '\0' && prm[j] != '\0' && j < nMax) {  if (argv[i][j] != prm[j]) { bExist = false; break; } ++j; }
        if (bExist) { break; }
    }

    if (!bExist) {
        this->argv = new char* [++this->argc];
        for (i = 0; i < this->argc; ++i) {
            j = 0;  k = 0;
            if (this->argc - 1 == i) {                
                while (prm[j++] != '\0');          --j;
                while (val[k++] != '\0') { ++j; }  --k;
            }
            else {
                while (argv0[i][j] != '\0' && j < nMax) { ++j; }
            }

            this->argv[i] = new char[(quint64)j + 1];
            this->argv[i][j] = '\0';
            while (--j >= 0) {
                this->argv[i][j] = this->argc - 1 == i ? (--k < 0 ? prm[j] : val[k]) : argv0[i][j];
            }
        }
    }

    return this->argc;
}

void CAppExt::freeArgv() {
    int& n = argc;
    if (n <= m_argcOld) { return; }

    for (int i = 0; i < n; ++i) { delete[] argv[i]; }
    delete[] argv;
}

void CAppExt::getLogName(char* t, char* d,const char* s) {
    t[0] = '\0';

    int idx = 0,lenSrc = str_len(s);
    char spl[2] = { ':' ,'\0' };
    if (s == NULL || str_pos(s, spl) < 0 || spl[0] == s[0]) {
        this->str_cat(t, m_sNameDrv);
        this->str_cat(t, spl);
    }
    else {
        char* sDrv = new char[(quint64)lenSrc+1];
        while (idx< lenSrc && s[idx++] != spl[0]) sDrv[idx-1] = s[idx-1];
        sDrv[idx-1] = spl[0];  sDrv[idx] = '\0';
        this->str_cat(t, sDrv);
        delete[] sDrv;
    }
    spl[0] = m_sSpl;
    if (this->isBlank(s) || str_pos(s, spl) < 0) {
        this->str_cat(t, m_sNamePath);
        this->str_cat(t, m_sPathPrt);
        this->str_cat(t, spl);
        this->str_cat(t, "logs");
        this->str_cat(t, spl);
    }
    else {
        if (NULL != s && spl[0] == s[idx]) {
            int len = this->str_len(s + idx);
            this->str_cat(t, s+idx);
            idx += len;
        }
        else {
            this->str_cat(t, m_sNamePath);
        }
    }
    if (NULL != d) {
        d[0] = '\0';
        this->str_cat(d, t);
        int lenTmp = str_len(d);
        if (lenTmp > 0) {
            if (m_sSpl == d[lenTmp-1]) {
                d[lenTmp - 1] = '\0';
            }
        }
    }
    if (this->isBlank(s) || (NULL != s && spl[0] == s[lenSrc - 1])) {
        this->str_cat(t, m_sNameApp);
        this->str_cat(t, ".log");
    }
    else {
        spl[0] = '.';
        if (NULL != s && '\0'!=s[idx]) {
            if (spl[0] == s[idx] && spl[0] != s[idx+1]) {
                this->str_cat(t, m_sNameApp);
            }
            if (spl[0] == s[lenSrc - 1]) {
                if (lenSrc > 1) {
                    this->str_cat(t, s + idx, 0, lenSrc - 1);
                }                
            }
            else {
                this->str_cat(t, s + idx);
                if (this->str_pos(s + idx, spl) < 0) {
                    this->str_cat(t, ".log");
                }
            }
        }
    }
}

int CAppExt::createPath(const char* sLogName) {
    int nRet = 0;
    const char*& p = sLogName;
    if (NULL == p || '\0' == p[0]) {
        return nRet;
    }
    char spl[] = {':','\0'}
        ,*sDrv = NULL, * sPath = NULL,*sName = NULL;
    int i = this->str_pos(p, spl)
        ,nDrv = 0 , nPath = 0,nFile = 0
        ,nLog = this->str_len(sLogName), n = 0
        ;
    bool bDrv = i>0 , bPath;
    
    nDrv = (bDrv ? (i+1): this->str_len(m_sNameDrv)) ;
    sDrv = new char[(quint64)nDrv + 1];
    this->str_cat(sDrv, (i > 0 ? p : m_sNameDrv), -1, nDrv);
    if (i < 0) {
        this->str_cat(sDrv, spl);
        ++nDrv;
    }
    i = bDrv ? nDrv : 0 ;
    i += m_sSpl == p[i] ? 1 : 0;
    bPath = m_sSpl == p[nDrv];
    nPath = (bPath ? 1 : this->str_len(m_sNamePath));
    if (nPath>0) {
        sPath = new char[(quint64)nPath + 1];
        this->str_cat(sPath, (bPath ? (p + nDrv) : m_sNamePath), -1, nPath);
    }

    nFile = this->str_len(sLogName+i) ;
    n = nDrv + nPath;
    sName = new char[(quint64)n+ nFile+1];
    this->str_cat(sName, sDrv, -1);
    this->str_cat(sName, sPath);

    p = sLogName + i;
    i = 0; 
    do {
        if ('\0' == *p) {
            continue;
        }
        if (m_sSpl == *p && i>0) {
            sName[n] = '\0';
            i = 0;
            this->makeDir(sName);
        }
        sName[n++] = *p;

        ++i;
    } while ('\0'!=*(++p));

    if (NULL != sDrv) {   delete[] sDrv;  }
    if (NULL != sPath) { delete[] sPath; }
    if (NULL != sName) { delete[] sName; }

    return nRet;
}

void CAppExt::initIni(QString& sVal, const QString& sKey, const QString& sIni) {
    m_sIni = sIni;
    if (m_sIni.isEmpty()) {
        m_sIni = QString::fromLocal8Bit("%1%2%3%4.ini").arg(m_sPathFull).arg(m_sPathPrt).arg(m_sSpl).arg(m_sNameApp);
    }
    this->reload(false);
    if (!sKey.isEmpty()) {
        sVal = this->getStr(sKey, sVal);
    }
    m_sVal = sVal;
}

void CAppExt::freeIni() {
    QSettings*& pIni = m_pIni;
    if (nullptr != pIni) {
        delete pIni;
        pIni = nullptr;
    }
}

void CAppExt::reload(const bool& bRel, const QString& sIni) {
    if (bRel) {
        this->freeIni();
    }

    QSettings*& pIni = m_pIni;
    if (!sIni.isEmpty()) {
        m_sIni = sIni;
    }
    pIni = new QSettings(m_sIni, QSettings::IniFormat);
    pIni->setIniCodec("UTF-8");
}

const QString CAppExt::getVal(const QString& key, const QString& val) {
    QSettings*& pIni = m_pIni;
    QString sRet = pIni->value(key).toString();
    if (sRet.isEmpty()) {
        sRet = val;
    }

    return sRet;
}

CWorkLog* CAppExt::newLog(const QString& sLog) {
    QMapIterator <QString, CWorkLog*> it(m_logs);
    while (it.hasNext()) {
        it.next();
        if (it.key().toUpper() == sLog.toUpper()) {
            return it.value();
        }
    }
    char log[1024],dir[1024];
    this->getLogName(log,dir, sLog.toLocal8Bit().data());
    this->createPath(log);
    CWorkLog* pLog = new CWorkLog(log);
    m_logs.insert(sLog, pLog);

    m_sLogDir = dir;

    return pLog;
}

void CAppExt::freeLog() {
    CWorkLog* pLog;
    QMapIterator <QString, CWorkLog*> it(m_logs);
    while (it.hasNext()) {
        it.next();
        pLog = it.value();
        if (nullptr != pLog) {            
            delete pLog;
        }
    }
    m_logs.clear();
}

const bool CAppExt::makeDir(const QString& sDir) {
    bool bRet = true;
    QDir dirCur;
    dirCur.setPath(sDir);
    if (!dirCur.exists()) {
        bRet = dirCur.mkdir(sDir);
    }
    return bRet;
}

void CAppExt::getcpuid(const unsigned int CPUInfo[4], const unsigned int& InfoType) {
#if defined(__GNUC__)
    __cpuid(InfoType, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
#elif defined(_MSC_VER)
#if _MSC_VER >= 1400 
    __cpuid((int*)(void*)CPUInfo, (int)(InfoType));
#else
    getcpuidex(CPUInfo, InfoType, 0);
#endif
#endif
}

const QString CAppExt::get_cpuId() {
    QString cpu_id = "";
    unsigned int dwBuf[4] = { 0 };
    unsigned long long ret = 0;
    getcpuid(dwBuf, 1);
    ret = dwBuf[3];
    ret = ret << 32;

    QString str0 = QString::number(dwBuf[3], 16).toUpper();
    QString str0_1 = str0.rightJustified(8, '0');
    QString str1 = QString::number(dwBuf[0], 16).toUpper();
    QString str1_1 = str1.rightJustified(8, '0');
    cpu_id = str0_1 + str1_1;
    return cpu_id;
}
//
int CAppExt::countLimit(const int n, const int nMin, const int nMax) {
    int nRet = n;
    if      (nRet < nMin) nRet = nMin;
    else if (nRet > nMax) nRet = nMax;

    return nRet;
}

int CAppExt::bufInit(uchar**& ucBuf, const int nBuf) {
    int i, n = countLimit(nBuf, 1, YC_BUFF_MAX);
    ucBuf = new uchar* [n] ;
    for (i = 0; i < n; ++i) {
        ucBuf[i] = nullptr;
    }

    return n;
}

void CAppExt::bufFree(uchar**& ucBuf, const int nBuf, const bool bAll) {
    if (nullptr == ucBuf) {
        return;
    }

    int i, n = nBuf;
    for (i = 0; i < n; ++i) {
        YQ_FREE_VAR(ucBuf[i]);
    }
    if (bAll) {
        YQ_FREE_VAR(ucBuf);
    }
}

uchar* CAppExt::bufNew(uchar**& ucBuf, quint64& nSizeCur, const quint64 nSizeNew, const int nBuf , const int iBuf) {
    if (nSizeNew < 1 || nBuf < 1 || iBuf < 0 || nullptr == ucBuf) {
        return nullptr;
    }

    int n = nBuf, idx = iBuf % n;
    bool bNew = nSizeNew > nSizeCur ;
    if (bNew) {
        nSizeCur = nSizeNew;
        this->bufFree(ucBuf, nBuf,false);
    }
    uchar*& pBuf = ucBuf[idx];
    if (nullptr == pBuf) {
        pBuf = new uchar[nSizeCur + 1];
    }    
    
    return pBuf;
}

bool CAppExt::testReload(const bool bPmt) {
    if (m_pSema == nullptr) {
        m_pSema = new QSystemSemaphore(m_sNameApp, 1, QSystemSemaphore::Open);
    }
    if (m_pMem == nullptr) {
        m_pMem = new  QSharedMemory(QString("%1Object").arg(m_sNameApp)); //全局对象名
    }
    m_pSema->acquire();

    bool bCreate = m_pMem->create(1);
    m_pSema->release();
    if (!bCreate && bPmt) {
        QMessageBox::warning(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("系统不能重复运行！"));
    }

    return bCreate;
}

bool CAppExt::log(const QString& sLog) {
    if (nullptr == m_pLog) {
        return false;
    }

    return m_pLog->log(QString::fromLocal8Bit("[%1] %2").arg((quint64)QThread::currentThreadId(), 6, 16, QLatin1Char('0')).arg(sLog));
}