#include <QWebSocketServer>
#include <QWebSocket>
#include <QThread>
#include "wshProxy.h"

CAppWss::CAppWss(CWorkLog*& pLog, const int nPort, const QString& sIp, const QString& sName)
	: CWorkTrd(pLog), m_nPort(nPort), m_sIp(sIp), m_sName(sName)
{
    m_pWss = nullptr;
}

CAppWss::~CAppWss()
{
    this->freeWss();
}

void CAppWss::showLog(const QString& sLog) {
    emit this->doShowLog(sLog);
}

void CAppWss::showInfo(const QString& sMoudle, const QString& sMess, const QString& sIp, const int& nPort) {
    emit this->doShowInfo(sMoudle, sMess,  sIp, nPort);
}

bool CAppWss::createWss( const int nPort, const QString& sIp, const QString& sName) {
    QWebSocketServer*& pWss = (QWebSocketServer*&)m_pWss;
    if (nullptr != pWss) {
        return true;
    }
    bool bRet = false;

    QWebSocketServer::SslMode sslMode = QWebSocketServer::SslMode::NonSecureMode ;
    m_sName = sName.isEmpty() ? "WSS" : sName;
    pWss = new QWebSocketServer(sName, sslMode, nullptr);
    
    connect(pWss, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    connect(pWss, SIGNAL(closed()), this, SLOT(onClosed()));    
    connect(pWss, SIGNAL(serverError(QWebSocketProtocol::CloseCode)),this, SLOT(slot_serverError(QWebSocketProtocol::CloseCode)));
    
    if (sIp.isEmpty() || 0 == sIp.compare("*")) {
        bRet = pWss->listen(QHostAddress::Any, nPort);
    }
    else {
        bRet = pWss->listen(QHostAddress(sIp), nPort);
    }

    return bRet;
}

void CAppWss::freeWss() {
    QWebSocketServer*& pWss = (QWebSocketServer*&)m_pWss;
    if (nullptr == pWss) {
        return;
    }
    pWss->disconnect();
    if (pWss->isListening()) {
        pWss->close();
        this->log(QString("[%1::freeWss] websocket server is close ").arg(this->metaObject()->className()));
    }
    YQ_FREE_OBJ(pWss);
}

void CAppWss::sendto(QString ip, qint32 port, QString message) {
    QString key = QString("%1-%2").arg(ip).arg(port);
    if (m_hsClt.contains(key)){
        ((QWebSocket*)m_hsClt.value(key))->sendTextMessage(message);
    }
}

void CAppWss::onClosed(){
    QList<void*> lsClt = m_hsClt.values();
    int i, n = lsClt.size();
    for (i = 0; i < lsClt.size(); i++){
        ((QWebSocket*)lsClt.at(i))->close();
    }
    m_hsClt.clear();
}

void CAppWss::onNewConnection() {
    QWebSocketServer*& pWss = (QWebSocketServer*&)m_pWss;    

    QWebSocket* pWebSocket = pWss->nextPendingConnection();

   // pWss->
    
    connect(pWebSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(pWebSocket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(onError(QAbstractSocket::SocketError)));
    // 既会触发frame接收也会触发message接收
//    connect(pWebSocket, SIGNAL(textFrameReceived(QString,bool)),this, SLOT(slot_textFrameReceived(QString,bool)));
    connect(pWebSocket, SIGNAL(textMessageReceived(QString)),this, SLOT(onTextMessageReceived(QString)));

    m_hsClt.insert(QString("%1-%2").arg(pWebSocket->peerAddress().toString()).arg(pWebSocket->peerPort()),pWebSocket);
    QStringList slIp = pWebSocket->peerAddress().toString().split(":");
    int n = slIp.length();
    
    this->showInfo(QString("/%1/%2_%3").arg(m_sName).arg("WSC").arg(m_hsClt.size()), "web socket client is connected", slIp.at(n - 1), pWebSocket->peerPort());

    this->log(QString("[%1::onNewConnection] new client %2:%3 ").arg(this->metaObject()->className()).arg(pWebSocket->peerAddress().toString()).arg(pWebSocket->peerPort()));
}

void CAppWss::onDisconnected() {
    QWebSocket* pWebSocket = dynamic_cast<QWebSocket*>(sender());
    if (!pWebSocket){
        return;
    }
    
    m_hsClt.remove(QString("%1-%2").arg(pWebSocket->peerAddress().toString()).arg(pWebSocket->peerPort()));

    this->log(QString("[%1::onDisconnected] close client %2:%3 ").arg(this->metaObject()->className()).arg(pWebSocket->peerAddress().toString()).arg(pWebSocket->peerPort()));
}

void CAppWss::onError(QAbstractSocket::SocketError error){
    QWebSocket* pWebSocket = dynamic_cast<QWebSocket*>(sender());
    if (!pWebSocket){
        return;
    }

    this->log(QString("[%1::onError] error client %2:%3 ,%4").arg(this->metaObject()->className()).arg(pWebSocket->peerAddress().toString()).arg(pWebSocket->peerPort()).arg(pWebSocket->errorString()));
 //   emit signal_error(pWebSocket->peerAddress().toString(), pWebSocket->peerPort(), pWebSocket->errorString());
}

void CAppWss::onTextMessageReceived(const QString& message) {
    QWebSocket* pWebSocket = dynamic_cast<QWebSocket*>(sender());
    if (!pWebSocket){
        return;
    }

    pWebSocket->sendTextMessage("i am fine !");

    this->showLog(message);

    this->log(QString("[%1::onTextMessageReceived] recv client %2:%3 ,%4").arg(this->metaObject()->className()).arg(pWebSocket->peerAddress().toString()).arg(pWebSocket->peerPort()).arg(message));
 //   emit signal_textMessageReceived(pWebSocket->peerAddress().toString(), pWebSocket->peerPort(), message);
}


void CAppWss::run() {
    if (this->createWss(m_nPort, m_sIp, m_sName)) {
        this->showInfo(QString("/%1").arg(m_sName), "web socket server is started", (m_sIp.isEmpty() ? "*" : m_sIp), m_nPort);
    }
    else {
        this->showLog("web socket server is fail");
    }
}

void CAppWss::halt() {
    this->freeWss();
}