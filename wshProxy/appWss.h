#pragma once

#include <QAbstractSocket>

#include <WorkTrd.h>

class CAppWss  : public CWorkTrd
{
	Q_OBJECT

public:
	CAppWss(CWorkLog*& pLog, const int nPort, const QString& sIp = "", const QString& sName = "");
	~CAppWss();

private:
    void* m_pWss;
    int m_nPort;
    QString m_sIp, m_sName;

    QHash<QString, void*> m_hsClt;

    void showLog(const QString& sLog);
    void showInfo(const QString& sMoudle, const QString& sMess, const QString& sIp, const int& nPort);
    bool createWss( const int nPort, const QString& sIp="", const QString& sName="");
    void freeWss();
    void sendto(QString ip, qint32 port, QString message);

protected:
    virtual void run();
    virtual void halt();

signals:
   // void onError(QString ip, quint32 port, QString errorString);
    void doShowLog(QString sMess);
    void doShowInfo(QString sMoudle, QString sMess, QString sIp, int nPort);

public slots:
    void onClosed();
    void onNewConnection();
    //
    void onDisconnected();
    void onError(QAbstractSocket::SocketError error);
    void onTextMessageReceived(const QString& message);
    //
};
