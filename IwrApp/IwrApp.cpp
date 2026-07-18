#include <QCloseEvent>

#include "IwrApp.h"

IwrApp::IwrApp(CAppCfg*& pCfg, QWidget *parent)
    : QMainWindow(parent), m_pCfg(pCfg)
{
    ui.setupUi(this);
    pCfg->setMain(this);

    this->initVar();
    connect(this, &IwrApp::doShowEvent, this, &IwrApp::onShowEvent, Qt::QueuedConnection);

    
    ui.teDetail->setHidden(true);
    
    connect(&m_tmTest, SIGNAL(timeout()), this, SLOT(ot_do_test()));
    m_tmTest.setInterval(5000);

    m_tmTest.start();
    
}

IwrApp::~IwrApp()
{
    YQ_FREE_OBJ(m_pCfg);
}

void IwrApp::showEvent(QShowEvent* e) {
    QMainWindow::showEvent(e);

    if (m_bShowInit) {
        return;
    }
    m_bShowInit = true;
    //-----------------
    emit doShowEvent();
}

void IwrApp::onShowEvent() {
    this->initIntf();
   this->setConnect();

}


void IwrApp::closeEvent(QCloseEvent* e) {
    if (!m_bQueryClose) {
        return;
    }

    if (getCfg()->ifQuitApp()) {
        e->accept();
    }
    else {
        e->ignore();
    }
}


void IwrApp::initVar() {
    m_bShowInit = false;
    m_bQueryClose = false;
}

void IwrApp::initIntf() {



    m_pCfg->initTextBrowser((void*&)ui.tbLog);

    //
}

void IwrApp::setConnect() {
    connect(ui.actionStart, &QAction::triggered, this, &IwrApp::on_act_start);
    connect(ui.actionStop, &QAction::triggered, this, &IwrApp::on_act_stop);
    connect(ui.actionExit, &QAction::triggered, this, &IwrApp::on_act_exit);
    connect(ui.actionConfig, &QAction::triggered, this, &IwrApp::on_act_config);
    //  connect(ui.textBrowser, &QTextEdit::textChanged, this, &wshProxy::on_text_change);
    
    connect(this, &IwrApp::doShowLog, this, &IwrApp::onShowLog, Qt::QueuedConnection);
    //connect(this, &wshProxy::doShowInfo, this, &wshProxy::onShowInfo, Qt::QueuedConnection);
    
    connect(this, &IwrApp::doPredialCall, this, &IwrApp::onPredialCall, Qt::QueuedConnection);
    connect(this, &IwrApp::doFreeDial, this, &IwrApp::onFreeDial, Qt::QueuedConnection);
}


void IwrApp::on_act_start() {

    m_bQueryClose = true;
    m_pCfg->run();

}

void IwrApp::on_act_stop() {
    m_pCfg->stop();
    m_bQueryClose = false;
}

void IwrApp::on_act_exit() {
    if (!getCfg()->ifQuitApp()) {
        return;
    }
    m_bQueryClose = false;
    this->close();
}

void IwrApp::on_act_config() {
    //
 //   getCfg()->getHtc()->doSend2User("http://localhost:8064/tp/public/TestRws/?gp1=gv1&gp2=gv2","{\"jp1\":\"jv1\"}");
 
}

void IwrApp::onShowLog(QString sMess) {

    getCfg()->showLog(sMess, true);
}

void IwrApp::ot_do_test() {
    m_tmTest.stop();
    ui.teDetail->setHidden(false);
}

//void IwrApp::onPredialCall(const QString InCaller) {
void IwrApp::onPredialCall(void* tapDial){
    //char* caller, char* called, char* serviceid, char* pridata, unsigned int predialid, char* displaytel

    struct stTapDial* ptd = (struct stTapDial*)tapDial;
    const struct stTapDial& td = *ptd;

  //  const struct stTapDial& td1 = getCfg()->getDialInfo(QString("%1").arg(td.tranID));

    int predialid = 1;
    char caller[80], called[80], serviceid[80], pridata[300], displaytel[80];
    strcpy(caller,td.InCaller);
    strcpy(called, td.InCalled);
    strcpy(serviceid, td.ServiceId);
    strcpy(displaytel, td.InCallerShow);
    int callType = td.CallType;
    switch (callType)
    {
    case 1:
        sprintf(pridata, "%d#%s#%s", (int)td.tranID, td.InCaller, td.InParam);
        break;
    case 2:
        sprintf(pridata, "%d#%s", (int)td.tranID,  td.InParam);
        break;
    case 3:
        sprintf(pridata, "%d#%s#%s", (int)td.tranID,  td.InCaller,  td.InParam);
        break;
    default:
        sprintf(pridata, "%d#%s#%s", (int)td.tranID, td.InCalled,  td.InParam);
        break;
    }

    
   // sprintf(pridata, "%d$%s$%s$%s", td.CallType, td.InCaller, td.InCalled, td.InParam);


    CAppCmm*& pCmm = getCfg()->getCmm();
    pCmm->SendPredialCallToMG(caller, called, serviceid, pridata, predialid, displaytel,td.tranID);


    //pCmm->SendPredialCallToMG((char*)td.InCaller, (char*)td.InCalled, (char*)td.InCalled, pridata, predialid, (char*)td.InCallerShow);
    /*
    QString sTmp;

    sTmp = td.InCaller;
    sTmp = td.InCalled;
    sTmp = td.CallType;
    sTmp = td.InParam;
    sTmp = td.InCallerShow;
    
    
    QString caller = td.InCaller,
        called = td.InCalled , serviceid = td.InCalled
        , pridata = QString::fromLocal8Bit("%1$%2$%3$%4").arg(td.CallType).arg(td.InCaller).arg(td.InCalled).arg(td.InParam)
        , displaytel = td.InCallerShow
        ;
  //  int predialid = td.ServerType ;
    int predialid = 1;


    CAppCmm*& pCmm = getCfg()->getCmm();
    pCmm->SendPredialCallToMG(caller.toLocal8Bit().data(), called.toLocal8Bit().data()
        , called.toLocal8Bit().data(), pridata.toLocal8Bit().data(), predialid, displaytel.toLocal8Bit().data());
    */
}

void IwrApp::onFreeDial(int tranId) {
    getCfg()->freeDial(tranId);
}