#pragma once

#include <QThread>
#include "WorkLog.h"

//#define WT_SIGNAL_RUN     1
#define WT_SIGNAL_STOP    2

class CWorkTrd  : public QObject
{
	Q_OBJECT

public:
	CWorkTrd(CWorkLog*& pLog, QObject* app = nullptr, QObject* parent = nullptr);
	~CWorkTrd();

	bool start(QObject* app = nullptr,const bool bRun = true);
	void stop();
	QThread*& getTrd() { return m_pTrdCur; }
	CWorkLog*& getLog() { return m_pLog; }
	void log(const QString& sLog);
	void logb(const QByteArray& sLog,const QString& sFlag="", const int len = -1);
	void setWaitSeg(const qint64 nSeg) { m_nWaitSeg = nSeg; }
	const qint64 getWaitSeg() { return m_nWaitSeg;	}
	static bool chkRunStatus(bool& bRun,const bool bReverse = false);
    const int getWaitSignal() { std::lock_guard<std::mutex> autoLock(m_vsMutex); int n = m_waitSignal;  return n; }
//    void    setWaitSignal(const int n = 0) { std::lock_guard<std::mutex> autoLock(m_vsMutex);  m_waitSignal = n; }
//	const int getWaitSignal() { int n = m_waitSignal;  return n; }
	void    setWaitSignal(const int n = 0) { m_waitSignal = n; }
	bool    waitSignal(const quint64 nSeg = 1000, const int nSignal = 0);
    const qint64& getWaitFree() { return m_nWaitFree; }
private:
    std::mutex  m_vsMutex;
    int     m_waitSignal;
	bool    m_bRun, m_bRan,m_bStop, m_bDoStart;
	QString m_sName;
	QObject* m_pApp;
	QThread* m_pTrdCur;
	CWorkLog* m_pLog;
	qint64 m_nStart,m_nWaitSeg,m_nWaitFree;
	qint64 m_nWaitBase;
    Qt::HANDLE  m_nCurTid;

	void newThread(const bool bDoRun = false);
	void freeThread();

protected:
	bool waitEvents(qint64& nCur, const qint64& nWainSeg=0,const bool bSetCur=true,const bool bSetEvt = true);
	bool isRun() { return m_bRun && nullptr!=m_pTrdCur && !m_pTrdCur->isInterruptionRequested(); }

	virtual void run();
	virtual void halt();
	virtual bool handleMessage(const int nCmd, const int nPrm, const QString sPrm);
public slots:
	void onMessage(const int nCmd, const int nPrm, const QString sPrm);
	void onStart();
	void onStop();
signals:
	void doMessage(const int nCmd, const int nPrm=0, const QString sPrm="");
	void doStart();
	void doStop();
};
