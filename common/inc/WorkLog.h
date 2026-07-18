#pragma once

#include <QFile>
#include <mutex>
#include <QTimer>
#include <QDate>

class CWorkLog : public QObject {
	Q_OBJECT

public:
	CWorkLog(const QString& sName="", QObject* parent = nullptr);
	~CWorkLog();

	const bool open(const QString& sName);
	const bool log(const QString& sLog,const QString& sTm="HH:mm:ss ",const bool bEnter = true);
	const bool logb(const QByteArray& sLog,const int len=0, const QString  head = "");
	void setShowTid(const bool bShow) { m_bShowTid = bShow; }

	const bool isValid() const{
		return m_bValid;
	}
private:
	std::mutex  m_vsMutex;

	int  m_nSegFlush;
	QString m_sErr;
	bool    m_bValid,m_bOpen,m_bFlush,m_bPause,m_bShowTid;
	qint64 m_nLine, m_nLineCur, m_nFlush;
	qint64  m_nWaitFlush;
	QFile   m_tLog;
	QTimer  m_tmFlush;
	QDate   m_dtCur;

	const bool isPause() { std::lock_guard<std::mutex> autoLock(m_vsMutex); return m_bPause; }
	void setPause(const bool bPause = true) { std::lock_guard<std::mutex> autoLock(m_vsMutex); m_bPause = bPause;	}
	bool backLog(QDate& dtLast, const QString& sLogName);
	void flushLog(const bool bAuto = false);
	bool openFile();
public slots:
	void ot_do_flush();
};
