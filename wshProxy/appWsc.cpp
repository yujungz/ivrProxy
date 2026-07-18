#include <QWebSocket>
#include "wshProxy.h"

CAppWsc::CAppWsc(CWorkLog*& pLog, const QString& sIp, const int nPort) : CWorkTrd(pLog), m_sIp(sIp), m_nPort(nPort) {
    m_pWsc = nullptr;
}

CAppWsc::~CAppWsc(){
    YQ_FREE_OBJ((QWebSocket*&)m_pWsc);
	
}

void CAppWsc::showLog(const QString& sLog) {
    emit this->doShowLog(sLog);
}

void CAppWsc::showInfo(const QString& sMoudle, const QString& sMess, const QString& sIp, const int& nPort) {
    emit this->doShowInfo(sMoudle, sMess, sIp, nPort);
}

void CAppWsc::run() {
    this->createWsc(m_sIp, m_nPort);
 
}

void CAppWsc::halt() {
    this->freeWsc();
}

void CAppWsc::createWsc(const QString& sIp, const int nPort) {
    QWebSocket*& pWsc = (QWebSocket*&)m_pWsc;
    if (nullptr != pWsc) {
        return;
    }
    pWsc = new QWebSocket();
    this->setConnect((void*&)pWsc);
    pWsc->open(QUrl(QString("ws://%1:%2").arg(sIp).arg(nPort)));
}

void CAppWsc::freeWsc() {
    QWebSocket*& pWsc = (QWebSocket*&)m_pWsc;
    if (nullptr == pWsc) {
        return;
    }
    pWsc->disconnect();    
    if (pWsc->isValid()) {
        pWsc->close();
        this->log(QString("[%1::freeWss] websocket client is close ").arg(this->metaObject()->className()));
    }
    YQ_FREE_OBJ(pWsc);
}

void CAppWsc::setConnect(void*& p) {
    QWebSocket*& pWsc = (QWebSocket*&)p;
    connect(pWsc, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(pWsc, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(pWsc, SIGNAL(textMessageReceived(QString)), this, SLOT(onTextMessageReceived(QString)));
}

void CAppWsc::onConnected() {
    this->log(QString("[%1::onConnected] connect to %1:%2 ").arg(this->metaObject()->className()).arg(m_sIp).arg(m_nPort));

    QWebSocket* pWebSocket = dynamic_cast<QWebSocket*>(sender());
    this->showInfo(QString("/%1").arg("WSC"), QString("web socket client connect to %1:%2").arg(pWebSocket->peerAddress().toString()).arg(pWebSocket->peerPort())
        , pWebSocket->localAddress().toString(), pWebSocket->localPort());

    pWebSocket->sendTextMessage("hello,world!");  //test
}

void CAppWsc::onDisconnected() {
    this->log(QString("[%1::onConnected] deconnect to %1:%2 ").arg(this->metaObject()->className()).arg(m_sIp).arg(m_nPort));
}

void CAppWsc::onTextMessageReceived(QString sMess) {
    this->log(QString("[%1::onTextMessageReceived] recv : %2 ").arg(this->metaObject()->className()).arg(sMess));
}