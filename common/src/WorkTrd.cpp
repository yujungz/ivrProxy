#include "WorkTrd.h"
#include <QCoreApplication>
#include <QDateTime>

CWorkTrd::CWorkTrd(CWorkLog*& pLog, QObject* app, QObject *parent)
	: QObject(parent), m_pApp(app),  m_pTrdCur(nullptr), m_pLog(pLog)
{
    m_nCurTid = nullptr;
	m_bDoStart = false;
    m_bRan = false;
	m_bRun = false;
    m_bStop = false;
	m_nWaitBase = 1;
	m_nWaitSeg  = m_nWaitBase;
	m_nWaitFree = 6 * 1000;
	m_nStart = QDateTime::currentMSecsSinceEpoch();
    m_waitSignal = 0;

	if (m_pApp != nullptr) {
		this->newThread();
	}
}

CWorkTrd::~CWorkTrd(){
	this->freeThread();
}

void CWorkTrd::log(const QString& sLog) {
	CWorkLog*& pLog = m_pLog;
	if (nullptr == pLog) {
		return;
	}
	pLog->log(QString("%1").arg(sLog));
}

void CWorkTrd::logb(const QByteArray& sLog , const QString& sFlag, const int len) {
	CWorkLog*& pLog = m_pLog;
	if (nullptr == pLog) {
		return;
	}
	QString str = sFlag.isEmpty() ? QString("") : QString("[%1]").arg(sFlag);
	pLog->logb(sLog,len, QString("%1").arg(str));
}

bool CWorkTrd::chkRunStatus(bool& bRun, const bool bReverse) {
	if (bReverse) {
		if (!bRun) {
			return true;
		}
		bRun = false;
	}
	else {
		if (bRun) {
			return true;
		}
		bRun = true;
	}

	return false;
}

bool CWorkTrd::start( QObject* app, const bool bRun) {
	if (m_pApp != nullptr) {
		return true ;
	}
	if (app != nullptr) {
		m_pApp = app;
	}
	this->newThread(bRun);

	return true;
}

void  CWorkTrd::stop() {
	if (nullptr == m_nCurTid) {
		return;
	}
    if (QThread::currentThreadId() == m_nCurTid) {
        this->onStop();
    }
    else {
		/*
		if (!this->waitSignal(m_nWaitFree, WT_SIGNAL_RUN)) {
			this->log(QString("[%1::stop] run work time out ! m_nCurTid=%2").arg(this->metaObject()->className()).arg((quint64)m_nCurTid, 6, 16, QLatin1Char('0')));
		}
		*/
        this->log(QString("[%1::stop] wait stop ...").arg(this->metaObject()->className()));
		this->setWaitSignal();
        emit this->doStop();
		
        this->log(QString("[%1::stop] stop work begin ...").arg(this->metaObject()->className()));
        if (!this->waitSignal(m_nWaitFree, WT_SIGNAL_STOP)) {
            this->log(QString("[%1::stop] stop work time out ! m_nCurTid=%2").arg(this->metaObject()->className()).arg((quint64)m_nCurTid, 6, 16, QLatin1Char('0')));
        }
        this->log(QString("[%1::stop] stop work end !").arg(this->metaObject()->className()));
		
    }
}

void CWorkTrd::newThread(const bool bDoRun) {
	if (m_pTrdCur != nullptr) {
		if (bDoRun && m_bRun) {
			emit this->doStart();
		}
		return;
	}
	//
	this->log(QString("[%1::newThread] (MSec:%2) work thread is starting ... ").arg(this->metaObject()->className()).arg(QDateTime::currentMSecsSinceEpoch()));

	connect(this, &CWorkTrd::doMessage, this, &CWorkTrd::onMessage);
	connect(this, &CWorkTrd::doStart  , this, &CWorkTrd::onStart  );
	connect(this, &CWorkTrd::doStop   , this, &CWorkTrd::onStop   );

	this->moveToThread(new QThread(m_pApp));
	m_pTrdCur = this->thread();
	m_pTrdCur->start();
	if (bDoRun) {
		emit this->doStart();
	}
}

void CWorkTrd::freeThread() {
	if (getTrd() == nullptr) {
		return;
	}

	this->log(QString("[%1::freeThread] (MSec:%2) work thread is free !").arg(this->metaObject()->className()).arg(QDateTime::currentMSecsSinceEpoch()));

	if (m_bRun) {
		m_bRun = false;
		getTrd()->requestInterruption();
	}

	if (this->parent() == nullptr || getTrd()->currentThreadId() != this->parent()->thread()->currentThreadId()) {
		bool bFree = false;
		if (getTrd()->isRunning()) {
			getTrd()->quit();
			//this->log(QString("[%1::freeThread] (MSec:%2) trace 4 !").arg(this->metaObject()->className()).arg(QDateTime::currentMSecsSinceEpoch()));
			if (getTrd()->wait(m_nWaitFree)) {
				bFree = true;
				getTrd()->deleteLater();
			}
			else {
				this->log(QString("[%1::freeThread] (MSec:%2) work thread free is time out !").arg(this->metaObject()->className()).arg(QDateTime::currentMSecsSinceEpoch()));
				getTrd()->terminate();
			}
		}
		if (!bFree) {
			delete getTrd();
		}
	}
}

void CWorkTrd::run() {
	this->log(QString("[%1::run] (MSec:%2) work thread is begin running...").arg(this->metaObject()->className()).arg(QDateTime::currentMSecsSinceEpoch()));
	/*
	quint64 iCount = 0;
	quint64 nCur = QDateTime::currentMSecsSinceEpoch(), nTimeSize = 1000L;
	this->log(QString("[CWorkBase](%1::%2)  work test begin ... timesize=%3")
			.arg((quint64)QThread::currentThreadId()).arg(nCur).arg(nTimeSize));
	while (!getTrd()->isInterruptionRequested()) {
		QCoreApplication::processEvents();
		QThread::msleep(1);
		
		if (++iCount > nTimeSize) {
			break;
		}

	}

	quint64 nEnd = QDateTime::currentMSecsSinceEpoch();
	this->log(QString("[CWorkBase](%1::%2)  work test end ! sub=%3")
		.arg((quint64)QThread::currentThreadId()).arg(nEnd).arg(nEnd - nCur));
	*/
	this->log(QString("[%1::run] (MSec:%2) work thread is end running !").arg(this->metaObject()->className()).arg(QDateTime::currentMSecsSinceEpoch()));
}

void CWorkTrd::halt() {
	this->log(QString("[%1::halt] (MSec:%2) work thread is halt !").arg(this->metaObject()->className()).arg(QDateTime::currentMSecsSinceEpoch()));
}

void CWorkTrd::onMessage(const int nCmd, const int nPrm, const QString sPrm) {
	this->handleMessage(nCmd, nPrm, sPrm);
}

bool CWorkTrd::handleMessage(const int nCmd, const int nPrm, const QString sPrm) {
	this->log(QString("[%1:handleMessage] (MSec:%2)  handle message : cmd=%3,nPrm=%4,sPrm=%5 !").arg(this->metaObject()->className())
		.arg(QDateTime::currentMSecsSinceEpoch()).arg(nCmd).arg(nPrm).arg(sPrm));

	return true;
}

void CWorkTrd::onStart() {
    if (this->chkRunStatus(m_bDoStart)) {
        return;
    }
    this->log(QString("[%1::onStart]  work is started ! ").arg(this->metaObject()->className()));

	m_bRun = true;
    m_bRan = false; 
    m_bStop = false;
	m_nStart = QDateTime::currentMSecsSinceEpoch();
    m_nCurTid = getTrd()->currentThreadId();

	this->run();
	this->log(QString("[%1::onStart] run work finished  ! m_bStop = %2").arg(this->metaObject()->className()).arg(m_bStop));

    m_bRan = true;
    if (m_bStop) {
        m_bStop = false;
        this->onStop();

    }
//	this->setWaitSignal(WT_SIGNAL_RUN);
}

void CWorkTrd::onStop() {
    this->log(QString("[%1::onStop]  wait work stop ...  m_bRan = %2 ").arg(this->metaObject()->className()).arg(m_bRan));
    if (m_bRan) {
        this->halt();
        this->disconnect();    
    }
    else {
        m_bStop = true;
        m_bRun = false;
        m_bDoStart = false;
        m_nCurTid = nullptr;
    }
    this->setWaitSignal(WT_SIGNAL_STOP);
    
    /*
    if (!this->waitSignal(m_nWaitFree, WT_SIGNAL_STOP)) {
        this->log(QString("[%1::onStop] run work time out ! m_nCurTid=%2").arg(this->metaObject()->className()).arg((quint64)m_nCurTid, 6, 16, QLatin1Char('0')));
    }
    */

	/*
	//this->waitSignal(1000);
	if (!this->waitSignal(m_nWaitFree, WT_SIGNAL_RUN)) {
		this->log(QString("[%1::onStop] run work time out ! m_nCurTid=%2").arg(this->metaObject()->className()).arg((quint64)m_nCurTid, 6, 16, QLatin1Char('0')));
	}
	*/


}

bool CWorkTrd::waitSignal(const quint64 nSeg, const int nSignal) {
    bool bRet = 0 == nSignal;
    if (nSeg < 1) {
        return false ;
    }
	/*
    if (!bRet) {
        this->setWaitSignal();
    }
	*/
    quint64 nCur = QDateTime::currentMSecsSinceEpoch();
    while (QDateTime::currentMSecsSinceEpoch() - nCur < nSeg) {
        QCoreApplication::processEvents();
        QThread::msleep(1);
        if (0 == nSignal || this->getWaitSignal() != nSignal) {
            continue;
        }
        bRet = true;
        break;
    }

    return bRet;
}

/*
* nCur: 当前戳
* nWainSeg：等待间隔，默认使用父类
* bSetCur：是否修改当前戳，默认是
* bSetEvt：是否接受事件处理，默认是
*/
bool CWorkTrd::waitEvents(qint64& nCur, const qint64& nWainSeg, const bool bSetCur, const bool bSetEvt) {
	if (bSetEvt) {
		QCoreApplication::processEvents();
		QThread::msleep(m_nWaitBase);
	}
	
	const qint64& nSeg = 0 >= nWainSeg ? m_nWaitSeg : nWainSeg ;
	const qint64& nTmp = QDateTime::currentMSecsSinceEpoch();
	if (nTmp - nCur < nSeg) {
		return false;
	}
	if (bSetCur) {
		nCur = nTmp;
	}

	return true;
}