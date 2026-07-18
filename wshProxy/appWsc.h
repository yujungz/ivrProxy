#pragma once

#include <WorkTrd.h>

class CAppWsc  : public CWorkTrd
{
    Q_OBJECT

public:
	CAppWsc(CWorkLog*& pLog, const QString& sIp, const int nPort);
	~CAppWsc();

protected:
    virtual void run();
    virtual void halt();

private:
    void* m_pWsc;
    int m_nPort;
    QString m_sIp;

    void showLog(const QString& sLog);
    void showInfo(const QString& sMoudle, const QString& sMess, const QString& sIp, const int& nPort);
    void createWsc(const QString& sIp, const int nPort);
    void freeWsc();
    void setConnect(void*& p);
signals:
    void doShowLog(QString sMess);
    void doShowInfo(QString sMoudle, QString sMess, QString sIp, int nPort);

public slots:
    void onConnected();
    void onDisconnected();
    void onTextMessageReceived(QString sMess);
};