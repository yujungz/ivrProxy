#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_wshProxy.h"
#include "appCfg.h"

class wshProxy : public QMainWindow
{
    Q_OBJECT

public:
    wshProxy(CAppCfg*& pCfg,QWidget *parent = nullptr);
    ~wshProxy();

    CAppCfg*& getCfg() { return m_pCfg; }
private:
    Ui::wshProxyClass ui;

    int m_bShowInit;
    bool m_bQueryClose;

    CAppCfg*& m_pCfg;

    void initVar();
    void initIntf();
    void setConnect();
protected:
    void showEvent(QShowEvent* e);
    void closeEvent(QCloseEvent* e);
public slots:
    void onShowEvent();

    void on_act_start();
    void on_act_stop();
    void on_act_exit();
    void on_act_config();

    void on_text_change();

    void onShowLog(QString sMess);
    void onShowInfo(QString sMoudle, QString sMess, QString sIp, int nPort);
signals:
    void doShowEvent();
    void doShowLog(QString sMess);
    void doShowInfo(QString sMoudle, QString sMess, QString sIp, int nPort);
};

extern CAppCfg* gpCfg;