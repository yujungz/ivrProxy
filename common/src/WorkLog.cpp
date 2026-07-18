#include <QDateTime>
#include <QFileInfo>
#include <QThread>
#include <WorkLog.h>

CWorkLog::CWorkLog(const QString& sName, QObject* parent) : QObject(parent)
	, m_bValid(false), m_bOpen(false), m_bFlush(false), m_bPause(false)
	, m_nLine(0), m_nLineCur(0), m_nFlush(0)
{
	connect(&m_tmFlush, SIGNAL(timeout()), this, SLOT(ot_do_flush()));
	m_tmFlush.setInterval(1000);

	m_nWaitFlush = 60 * 1000;
	if (!sName.isEmpty()) {
		this->open(sName);
	}

	m_bShowTid = true;
	m_nSegFlush = 1;  
}

CWorkLog::~CWorkLog() {
	m_bOpen  = false;
	m_bValid = false;

	if (m_tmFlush.isActive()) {
		m_tmFlush.stop();
	}
	if (m_tLog.isOpen()) {
		m_tLog.close();
	}
}

const bool CWorkLog::open(const QString& sName) {
	if (sName.isEmpty()) {
		m_sErr = QString::fromLocal8Bit("file name is empty !");
		return false;
	}
	if (m_bOpen) {
		m_sErr = QString::fromLocal8Bit("file is opened !");
		return m_bOpen;
	}

	QFileInfo fi(sName);
	if (fi.exists()) {
		if (fi.size() > 0) {
			m_dtCur = fi.lastModified().date();
			//m_dtCur = m_dtCur.addDays(-1);  //test
			this->backLog(m_dtCur, sName);
		}		
	}
	else {
		m_dtCur = QDate::currentDate();
	}

	m_tLog.setFileName(sName);
	m_bValid = this->openFile();

	return this->isValid();
}

bool CWorkLog::openFile() {
	QFile& log = m_tLog;
	bool bRet = log.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
	if (bRet) {
		m_bOpen = true;
		m_sErr = QString::fromLocal8Bit("ok !");
		if (m_nSegFlush > 1) {
			m_tmFlush.start();
		}
	}
	else {
		m_bOpen = false ;
		m_sErr = QString::fromLocal8Bit("open log file fail !");
	}

	return bRet;
}

const bool CWorkLog::log(const QString& sLog, const QString& sTm, const bool bEnter) {
	if(!this->isValid()){
		m_sErr = QString::fromLocal8Bit("log is valid !");
		return false;
	}
	
	QString sMess;
	if (!sTm.isEmpty()) {
		sMess += QDateTime::currentDateTime().toString(sTm).toLocal8Bit();
	}
	if (m_bShowTid) {
		sMess += QString("[%1] ").arg((quint64)QThread::currentThreadId(), 6, 16, QLatin1Char('0'));
	}
	sMess += sLog;
	if (bEnter) {
		sMess += "\n";
	}
	{
		std::lock_guard<std::mutex> autoLock(m_vsMutex);
		m_tLog.write(QString::fromLocal8Bit("%1").arg(sMess).toLocal8Bit());
	}
	this->flushLog(false);

	return true;
}

bool const CWorkLog::logb(const QByteArray& sLog, const int len, const QString  head) {
	QString sByte = head+" ";

	int i, n = sLog.length();
	if (len > 0) {
		if (len < n) {
			n = len;
		}
	}
	if (len != 0 && n > 0) {
		sByte += QString("(len:%1) ").arg(n,4,10, QLatin1Char('0'));
	}
	for (i = 0; i < n; ++i) {
		if (i > 0) {
			sByte += " ";
		}
		sByte += QString::fromLocal8Bit("%1").arg(sLog.at(i) & 0xff, 2, 16, QChar('0'));
	}

	return this->log(sByte);
}

void CWorkLog::flushLog(const bool bAuto) {
	if (this->backLog(m_dtCur, m_tLog.fileName())) {
		return;
	}

	qint64 nCur = QDateTime::currentMSecsSinceEpoch();
	bool bFlush = false;
	if (bAuto) {
		if (m_nLineCur < m_nLine && nCur - m_nFlush>m_nWaitFlush) {
			m_nFlush = nCur;
			bFlush = true;			
		}
	}
	else {
		m_nFlush = nCur;
		++m_nLine;
		bFlush = m_nSegFlush>1 ? (0 == (m_nLine % m_nSegFlush)) : true ;
	}

	if (bFlush) {		
		m_nLineCur = m_nLine;

		std::lock_guard<std::mutex> autoLock(m_vsMutex);
		m_tLog.flush();
	}
}

bool CWorkLog::backLog(QDate& dtLast,const QString& sLogName) {
	QDate dtCur = QDate::currentDate();
	if (dtLast >= dtCur) {
		return false;
	}
	if (m_bOpen) {
		if (m_tmFlush.isActive()) {
			m_tmFlush.stop();
		}
		if (m_tLog.isOpen()) {
			m_tLog.close();
		}
	}
	
	QString sFmt = "yyyyMMdd" ;
	QString sNameNew = QString::fromLocal8Bit("%1_%2").arg(sLogName).arg(dtLast.toString(sFmt));
	if (!m_bOpen || m_nLine > 0) {
		if (QFileInfo::exists(sNameNew)) {
			QFile::remove(sNameNew);
		}
		QFile::rename(sLogName, sNameNew);
	}

	dtLast = dtCur;
	m_nLineCur =
	m_nLine = 0;

	if (m_bOpen) {
		m_bValid = this->openFile();
	}

	return true;
}

void CWorkLog::ot_do_flush() {
	if (!this->isValid() || this->isPause() ) {
		return ;
	}

	this->flushLog(true);
}