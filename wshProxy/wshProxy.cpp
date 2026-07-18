#include <QSplitter>
#include <QDateTime>
#include <QDir>
#include <QCloseEvent>
#include <QMessageBox>
#include <QMediaPlayer>

#include "wshProxy.h"

wshProxy::wshProxy(CAppCfg*& pCfg, QWidget *parent)
    : QMainWindow(parent), m_pCfg(pCfg)
{
    ui.setupUi(this);
    pCfg->setMain(this);

    this->initVar();
    connect(this, &wshProxy::doShowEvent, this, &wshProxy::onShowEvent, Qt::QueuedConnection);


    /*
    CBufPool buf;
    buf.init(1);
    char* b1 = buf.create(80);
    b1 = buf.create(60);
    b1 = buf.create(60);
    b1 = buf.create(90);
    */
}

wshProxy::~wshProxy()
{
    YQ_FREE_OBJ(m_pCfg);
}

void wshProxy::showEvent(QShowEvent* e) {
    QMainWindow::showEvent(e);

    if (m_bShowInit) {
        return;
    }
    m_bShowInit = true;
    //-----------------
    emit doShowEvent();
}

void wshProxy::onShowEvent() {
    this->initIntf();
    this->setConnect();


}

void wshProxy::closeEvent(QCloseEvent* e) {
    if (!m_bQueryClose) {
        return;
    }

    if (getCfg()->ifQuitApp()) {
        e->accept();
    }else {
        e->ignore();
    }
}

void wshProxy::initVar() {
    m_bShowInit = false;
    m_bQueryClose = false;
}

void wshProxy::initIntf() {
    QVector<QWidget*> wgSub;
    wgSub.append(ui.treeView);
    wgSub.append(ui.textBrowser);

    m_pCfg->createSplitter(wgSub, ui.centralWidget);
    m_pCfg->createTree(ui.treeView, this);

    /*
    jk.putVal(QString::fromLocal8Bit("val00"), "/fd0/fd0");
    jk.putVal(QString::fromLocal8Bit("val01"), "/fd0/fd1");

    jk.putVal(QString::fromLocal8Bit("val10"), "/fd1/fd0");
    */
    /*
    jk.putVal(QString::fromLocal8Bit("val00"), "/0/0/fd00");
    jk.putVal(QString::fromLocal8Bit("val01"), "/0/1/fd01");

    jk.putVal(QString::fromLocal8Bit("val10"), "/1/0/fd10");
    jk.putVal(QString::fromLocal8Bit("val11"), "/1/1/fd11");
    */

    /*
    jk.putVal(QString::fromLocal8Bit("模块"), "/0/title");
    jk.putNumber(80, "/0/width");
    jk.putVal(QString::fromLocal8Bit("地址"), "/1/title");
    jk.putNumber(150, "/1/width");
    */

    //jk.putVal(QString::fromLocal8Bit("模块"), "/0/title");
   // jk.putNumber(80, "/0/width");
   // jk.putNumber(80, "/0/width");

    QString sTitle[][2] = {
         {QString::fromLocal8Bit("模块"),"160"}
        ,{QString::fromLocal8Bit("地址"),"150"}
        ,{QString::fromLocal8Bit("端口"),"60"}
        ,{QString::fromLocal8Bit("信息"),"600"}
    };
    int i, n = ARRAYSIZE(sTitle);

    CJsonKit jk;

    for (i = 0; i< n; ++i) {
        jk.putVal(sTitle[i][0], QString("/%1/title").arg(i));
        jk.putNumber(sTitle[i][1].toInt(), QString("/%1/width").arg(i));
    }
    

    m_pCfg->initTree((void*&)ui.treeView,jk.toString());
    m_pCfg->initTextBrowser((void*&)ui.textBrowser);

    //
}

void wshProxy::setConnect() {
    connect(ui.actionStart, &QAction::triggered, this, &wshProxy::on_act_start);
    connect(ui.actionStop, &QAction::triggered, this, &wshProxy::on_act_stop);
    connect(ui.actionExit, &QAction::triggered, this, &wshProxy::on_act_exit);
    connect(ui.actionConfig, &QAction::triggered, this, &wshProxy::on_act_config);
    //  connect(ui.textBrowser, &QTextEdit::textChanged, this, &wshProxy::on_text_change);

    connect(this, &wshProxy::doShowLog, this, &wshProxy::onShowLog, Qt::QueuedConnection);
    connect(this, &wshProxy::doShowInfo, this, &wshProxy::onShowInfo, Qt::QueuedConnection);
}

void wshProxy::on_act_start() {

    m_bQueryClose = true ;
    m_pCfg->run();
    /*
    //ui.textBrowser->append(QString::fromLocal8Bit("%1").arg(QDateTime::currentMSecsSinceEpoch()));
    m_pCfg->log(QString::fromLocal8Bit("%1").arg(QDateTime::currentMSecsSinceEpoch()));

    m_pCfg->showInfo("/CMM", QString::fromLocal8Bit("1-%1").arg(QDateTime::currentMSecsSinceEpoch()), "*:8000");
    m_pCfg->showInfo("/CMM/clt1", QString::fromLocal8Bit("1.1-%1").arg(QDateTime::currentMSecsSinceEpoch()), "*:8000");
    m_pCfg->showInfo("/CMM/clt2", QString::fromLocal8Bit("1.2-%1").arg(QDateTime::currentMSecsSinceEpoch()), "*:8000");
    m_pCfg->showInfo("/CMM/clt1", QString::fromLocal8Bit("1.1a-%1").arg(QDateTime::currentMSecsSinceEpoch()), "*:8000");
    m_pCfg->showInfo("/CMM", QString::fromLocal8Bit("1b-%1").arg(QDateTime::currentMSecsSinceEpoch()));
    m_pCfg->showInfo("/WSS", QString::fromLocal8Bit("2-%1").arg(QDateTime::currentMSecsSinceEpoch()),  "127.0.0.1:8001");
    m_pCfg->showInfo("/CMM2", QString::fromLocal8Bit("2-%1").arg(QDateTime::currentMSecsSinceEpoch()));
    */

    ui.treeView->expandAll();
}

void wshProxy::on_act_stop() {
    m_pCfg->stop();
    m_bQueryClose = false;
}

void wshProxy::on_act_exit() {
    if (!getCfg()->ifQuitApp()) {
        return;
    }
    m_bQueryClose = false;
    this->close();
}

void wshProxy::on_act_config() {
    //
    //QMessageBox::warning(nullptr,"prompt","on_act_config");
    /*
    QMediaPlayer* player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile("path/to/audio/file"));

    connect(player, &QMediaPlayer::stateChanged, [=](QMediaPlayer::State state) {
        if (state == QMediaPlayer::StoppedState) {
            qDebug() << "Audio playback finished.";
        }
        });

    player->play();
   */ 
}

void wshProxy::on_text_change() {
  //  ui.textEdit->moveCursor(QTextCursor::End);
}

void wshProxy::onShowLog(QString sMess) {

    getCfg()->showLog(sMess,true);
}

void wshProxy::onShowInfo(QString sMoudle, QString sMess, QString sIp, int nPort) {

    getCfg()->showInfo(sMoudle, sMess, QString("%1:%2").arg(sIp).arg(nPort));
}