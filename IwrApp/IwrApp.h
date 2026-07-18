#pragma once

#include <QtWidgets/QMainWindow>
#include <QTimer>
#include "ui_IwrApp.h"

#include "appCfg.h"

class IwrApp : public QMainWindow
{
    Q_OBJECT

public:
    IwrApp(CAppCfg*& pCfg, QWidget *parent = nullptr);
    ~IwrApp();

    CAppCfg*& getCfg() { return m_pCfg; }

protected:
    void showEvent(QShowEvent* e);
    void closeEvent(QCloseEvent* e);

private:
    Ui::IwrAppClass ui;

    CAppCfg*& m_pCfg;
    int m_bShowInit;
    bool m_bQueryClose;

    QTimer m_tmTest;

    void initVar();
    void initIntf();
    void setConnect();
public slots:
    void onShowEvent();

    void on_act_start();
    void on_act_stop();
    void on_act_exit();
    void on_act_config();

    void onShowLog(QString sMess);

    void ot_do_test();

    //void onPredialCall(const QString InCaller);
    void onPredialCall(void* tapDial);
    void onFreeDial(int tranId);
signals:
    void doShowEvent();
    void doShowLog(QString sMess);
    //void doPredialCall(const QString InCaller);
    void doPredialCall(void* tapDial);
    void doFreeDial(int tranId);
};

extern CAppCfg* gpCfg;