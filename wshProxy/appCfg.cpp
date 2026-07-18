#include <QLayout>
#include <QDir>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include "wshProxy.h"

CAppCfg::CAppCfg(const int& argc0, const char**& argv0) : CAppExt(argc0, argv0), pcommainManager(NULL) {

    m_nLog = 0;

    m_pMain = nullptr;
    m_pTree = nullptr;
    m_pTb = nullptr;

    splitter = nullptr;

    m_pAppCmm = nullptr;
    m_pAppWss = nullptr;
    m_pAppWsc = nullptr;
    m_pAppHts = nullptr;

    m_sPathOrig = "";
    QString sPathApp = this->getAppPath(),sPathCur = QDir::currentPath().replace("/", "\\") + "\\";
    if (sPathApp != sPathCur) {
        m_sPathOrig = sPathCur;
        QDir::setCurrent(sPathApp);
    }
}

CAppCfg::~CAppCfg() {
    YQ_FREE_OBJ(splitter);

    this->stop();
    if (!m_sPathOrig.isEmpty()) {
        QDir::setCurrent(m_sPathOrig);
    }
}

const bool CAppCfg::ifQuitApp() {
    return QMessageBox::information(nullptr, QString::fromLocal8Bit("提示")
        , QString::fromLocal8Bit("是否确认退出？"),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes;
}

QString getBaseUrl(const QString& fullUrl)
{
    QUrl url(fullUrl);

    // 拼接：scheme( http/https ) + "://" + host(IP/域名) + port(如果有)
    QString baseUrl = url.scheme() + "://" + url.host();

    // 如果有端口，加上端口
    if (url.port() != -1) {
        baseUrl += ":" + QString::number(url.port());
    }

    return baseUrl;
}

void  CAppCfg::initTextBrowser(void*& pTb, const int n) {
    m_pTb = pTb;  
    m_nLog = n; 

    QTextBrowser*& p = (QTextBrowser*&)m_pTb;
    p->document()->setMaximumBlockCount(n);
    p->setLineWrapMode(QTextBrowser::NoWrap);
}

int  CAppCfg::SendReq_old(QString strUrl, QString strInput, QString& strResult, const bool bPost) {
    QNetworkRequest oNetRequest;

    //oNetRequest.setUrl(QUrl(strUrl));
   // oNetRequest.setRawHeader("Content-Type",(bPost ? "application/x-www-form-urlencoded": "application/json"));
    /*
    oNetRequest.setRawHeader("CLIENTVERSION", ("V1"));
    if (!sTk.isEmpty()) {
        oNetRequest.setRawHeader("x_auth_token", sTk.toLocal8Bit().data());
    }
    oNetRequest.setRawHeader("Platform-Info", ZW_PLATFORM_INFO);
    */
    //this->log(QString::fromLocal8Bit("req8url：url=%1,prm=%2,token=%3").arg(strUrl).arg(strInput).arg(sTk));

    QNetworkAccessManager oNetAccessManager;
    oNetAccessManager.setNetworkAccessible(QNetworkAccessManager::Accessible);
    QNetworkReply* oNetReply;
    if (bPost) {
        QString sUrl = strUrl;
        QByteArray  postData;
        if (strInput == "") {
            if (strUrl.contains("?")) {  //input
                QStringList arUrl = strUrl.split("?");
                sUrl = arUrl.at(0);

                int i, n;
                n = arUrl.length();
                if (n > 1) {
                    QStringList arData = arUrl.at(1).split("&");
                    n = arData.length();
                    if (n > 0) {
                        CJsonKit jk;
                        for (i = 0; i < n; ++i) {
                            QStringList arItem = arData.at(i).split("=");
                            jk.putVal((arItem.length() > 1 ? arItem.at(0) : ""), "/" + arItem.at(0));
                        }
                        postData.append(jk.toString());
                        strInput = jk.toString();
                    }
                }
            }
        }else{
            postData.append(strInput);
        }
        oNetRequest.setUrl(QUrl(sUrl));
		oNetRequest.setRawHeader("Content-Type", (bPost ? "application/x-www-form-urlencoded" : "application/json"));
		if (!bPost) {
			oNetRequest.setRawHeader("Accept", "application/json");
		}
    //    oNetRequest.setRawHeader("Content-Type", "application/x-www-form-urlencoded" );
        oNetReply = oNetAccessManager.post(oNetRequest, postData);
    }
    else {
        oNetRequest.setUrl(QUrl(strUrl));
        oNetRequest.setRawHeader("Content-Type",  "application/json");
        oNetReply = oNetAccessManager.get(oNetRequest);
    }


    QEventLoop loop;
    wshProxy*& pMain = (wshProxy*&)m_pMain;
    pMain->connect(oNetReply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    int httpsCode = oNetReply->attribute(QNetworkRequest::Http2DirectAttribute).toInt();
    QNetworkReply::NetworkError e = oNetReply->error();
    if (e == QNetworkReply::NoError) {
        strResult = oNetReply->readAll();
    }
    else {
        strResult = oNetReply->errorString();
        if (httpsCode == 0) {
            httpsCode = e ;
        }
        else {
            if (httpsCode == 301 || httpsCode == 302 || httpsCode == 307){
                QString redirectUrl = oNetReply->header(QNetworkRequest::LocationHeader).toString();
                if (redirectUrl.startsWith("/")){
                    // 自动补全完整地址
                    QString baseUrl = getBaseUrl(strUrl); // 提取基地址
                    redirectUrl = baseUrl + redirectUrl;
                }
                this->log(QString::fromLocal8Bit("redirect：url=%1,prm=%2,post=%3").arg(strUrl).arg(strInput).arg(bPost));
            //    httpsCode = this->SendReq2(redirectUrl, strInput, strResult, bPost);
            }
        }
    }
    //this->log(QString::fromLocal8Bit("rsp8url：ret=%1,err=%2 ").arg(strResult).arg(strErrInfo));

    return httpsCode;
}

int  CAppCfg::SendReq(QString strUrl, QString strInput, QString& strResult, const bool bPost) {
    QNetworkRequest oNetRequest;
    oNetRequest.setUrl(QUrl(strUrl));
    oNetRequest.setRawHeader("Content-Type", (bPost ? "application/x-www-form-urlencoded" : "application/json"));
    if (!bPost) {
        oNetRequest.setRawHeader("Accept", "application/json");
    }

    QNetworkAccessManager oNetAccessManager;
    oNetAccessManager.setNetworkAccessible(QNetworkAccessManager::Accessible);

    QByteArray  postData;
    if (strInput != "") {
        postData.append(strInput);
    }
    QNetworkReply* oNetReply = oNetAccessManager.post(oNetRequest, postData);

    QEventLoop loop;
    wshProxy*& pMain = (wshProxy*&)m_pMain;
    pMain->connect(oNetReply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    int httpsCode = oNetReply->attribute(QNetworkRequest::Http2DirectAttribute).toInt();
    QNetworkReply::NetworkError e = oNetReply->error();
    if (e == QNetworkReply::NoError) {
        strResult = oNetReply->readAll();
    }
    else {
        strResult = oNetReply->errorString();
        if (httpsCode == 0) {
            httpsCode = e;
        }
    }
    this->log(QString::fromLocal8Bit("CAppCfg::SendReq：strUrl(%1), strInput(%2), bPost(%3), strResult(%4), httpsCode(%5)").arg(strUrl).arg(strInput).arg(bPost).arg(strResult).arg(httpsCode));

    return httpsCode;
}
/*
void CAppCfg::initInfoTree(void*& pTree) {
    m_pTree = pTree;

   
}
*/
bool CAppCfg::showLog(const QString& sLog, const bool show) {

    QTextBrowser*& pTb = (QTextBrowser*&)m_pTb;
    if (show && m_pTb) {
        pTb->append(QTime::currentTime().toString("[HH:mm:ss] ") + sLog);
    }
    
    return CAppExt::log("[SHOW] " + sLog);
}

void CAppCfg::initTree(void*& pTree, const QString& sHead) {
    QTreeView*& pT = (QTreeView*&)m_pTree;
    pT = (QTreeView*&)pTree ;
    if (nullptr == pT) {
        return;
    }
    
    QStandardItemModel* pM = (QStandardItemModel*)pT->model();
    if (nullptr != pM) {
        return ;
    }

    QStringList slTitle;

    CJsonKit jk(sHead);
    QString sTmp;
    int n = 0;
    while (""!= (sTmp = jk.getStr(QString("/%1/title").arg(n++)))) {
        slTitle << sTmp;
    }
    pM = new QStandardItemModel(pT->parent());
    pM->setHorizontalHeaderLabels(slTitle);
    pT->setModel(pM);

    n = 0;
    while ("" != (sTmp = jk.getStr(QString("/%1/width").arg(n)))) {
        pT->setColumnWidth(n, sTmp.toInt());
        ++n;
    }

}

bool CAppCfg::findTreeKey(const QString& sKey, int& idxHead, int& idxSub) {
    idxSub =
    idxHead = -1 ;
    bool bRet = false;

    QTreeView*& pTree = (QTreeView*&)m_pTree;
    QStringList slKey = sKey.split("/");
    int len = slKey.length();
    if (len < 2 || slKey[1].isEmpty() || nullptr == pTree) {
        return bRet;
    }

    int i, n;
    QStandardItemModel* m = (QStandardItemModel*)pTree->model();
    QStandardItem* itRoot = m->invisibleRootItem();//获取根节点

    n = itRoot->rowCount();
    //idxHead = n ;
    for (i = 0; i < n; ++i) {
        if (0 == m->item(i)->text().compare(slKey[1])) {
            idxHead = i ;
            bRet = true;
            break;
        }
    }
    if (len > 2 && bRet) {
        bRet = false;
        if (itRoot->hasChildren()) {            
            QStandardItem* item = m->item(idxHead);
            n = item->rowCount();
            //idxSub = n; 
            for (i = 0; i < n; ++i) {
                if (0 == item->child(i)->text().compare(slKey[2])) {
                    idxSub = i;
                    bRet = true;
                    break;
                }
            }
            
        }
    }

    return bRet;
}

void CAppCfg::showInfo(const QString& sKey, const QString& sInfo, const QString& sAddr) {
    QTreeView*& pTree = (QTreeView*&)m_pTree;
    if (nullptr == pTree) {
        return ;
    }
    
    QStringList slKey = sKey.split("/");
    bool bSub = slKey.length() > 2;
    QString key = slKey.at(1);
    int idxHead,idxSub,i,n,idx,nItem;
    //this->findTreeKey("/" + key, idxHead, idxSub);
    this->findTreeKey(sKey, idxHead, idxSub);

    QStandardItemModel* model = (QStandardItemModel*)pTree->model();

    QStringList slVal = sAddr.split(":");
    n = slVal.length();
    QString arVal[] = {
        key,slVal.at(0),(n<2 ? "": slVal.at(1)),sInfo
    };
    n = ARRAYSIZE(arVal);
    if (idxHead<0 || !bSub) {
        nItem = 0;
        for (i = 0 ; i < n; ++i) {
            if (bSub && i > 0) {
                break;
            }
            if (arVal[i].isEmpty()) {
                continue;
            }
            if (idxHead < 0) {
                if (i < 1) {
                    model->appendRow(new QStandardItem(arVal[i]));
                    nItem = model->rowCount() - 1;
                }
                else {
                    model->setItem(nItem, i, new QStandardItem(arVal[i]));
                }                
            }
            else {
                if (i > 0) {
                    model->setItem(idxHead, i, new QStandardItem(arVal[i]));
                }                
            }
        }
    }

    if (bSub) {
        nItem = 0;
        arVal[0] = slKey.at(2);
        QStandardItem* curTopItem = model->item(idxHead);
        for (i = 0; i < n; ++i) {
            if (arVal[i].isEmpty()) {
                continue;
            }
            if (idxSub < 0) {
                if (i < 1) {
                    curTopItem->appendRow(new QStandardItem(arVal[i]));
                    nItem = curTopItem->rowCount() - 1;
                }
                else {
                    curTopItem->setChild(nItem, i, new QStandardItem(arVal[i]));
                }                
            }
            else {
                if (i > 0) {
                    curTopItem->setChild(idxSub, i, new QStandardItem(arVal[i]));
                }
            }
            
           // curTopItem->appendRow();
           // curTopItem->setChild(idxSub, i, new QStandardItem(arVal[i]));
          //  curTopItem->setChild(0, i, new QStandardItem(arVal[i]));
        }
    }
    /*
    QStandardItem* curTopItem = model->item(idxHead);
    curTopItem->setChild(0, 0, new QStandardItem("node1"));
    curTopItem->setChild(0, 1, new QStandardItem("node2"));
   
    QList<QStandardItem*> childList;
    childList << new QStandardItem(QString::fromLocal8Bit("子节点:%1-1").arg(curTopItem->rowCount()))
        << new QStandardItem(QString::fromLocal8Bit("子节点:%1-2").arg(curTopItem->rowCount()));
    curTopItem->appendRow(childList);
    */


    //QStandardItem* item = model->item;
    
    /*
    QStandardItem* item1 = new QStandardItem(QString::fromLocal8Bit("CMM"));
    QStandardItem* item1a = new QStandardItem(QString::fromLocal8Bit("connected"));

    model->setItem(0, 0, item1);
    model->setItem(0, 1, item1a);

    QStandardItemModel* m = (QStandardItemModel*)pTree->model();
    if (sParent.isEmpty()) {
        QStandardItem* itRoot = m->invisibleRootItem();//获取根节点
     //   itRoot->
    }
    else {

    }
    */

    /*
    QStandardItemModel* model = (QStandardItemModel*)pTree->model();
   // this->initTree((void*&)pTree, (void*&)model);
    if (sParent.isEmpty()) {
        //pTree
        QStandardItem* itRoot = model->invisibleRootItem();//获取根节点
        QStandardItem* itModule = new QStandardItem(sModule);
        itRoot->appendRow(itModule);//给item建立分支节点    
        itRoot->appendRow(new QStandardItem(sAddr));
        itRoot->appendRow(new QStandardItem(sAddr));
        itRoot->appendRow(new QStandardItem(sInfo)); 
        //itModule->appendRow(new QStandardItem(sInfo));
       // QStandardItem* item1 = new QStandardItem(QString::fromLocal8Bit("CMM"));
    }
    else {

    }
   */
}

bool CAppCfg::run() {
    if (!this->startCmm()) {
        return false;
    }
    
    this->startHts();
    /*
    this->startWss();  
    this->startWsc();  //test
    */

    return true;
}

void CAppCfg::stop() {
    if (nullptr != m_pAppWsc) {
        m_pAppWsc->stop();
    }
    if (nullptr != m_pAppWss) {
        m_pAppWss->stop();
    }
    if (nullptr != m_pAppHts) {
        m_pAppHts->stop();
    }

    this->freeCmm();
    YQ_FREE_OBJ(m_pAppWsc);
    YQ_FREE_OBJ(m_pAppWss);
    YQ_FREE_OBJ(m_pAppHts);
}

bool  CAppCfg::startCmm() {
    QString sNetIni = QString("%1..\\..\\%2").arg(this->getAppPath()).arg("network.ini");
    pcommainManager = new TCommManager(sNetIni.toLocal8Bit().data());
    if (!pcommainManager || !pcommainManager->Run()) {
        this->log("run communication manager fail !");
        //	PrintConsole(0, "Start Communication Module Failed! Please Exit Now");
        //	ExitFromError();
        //	AfxMessageBox("Warning:Start Communication Failed!", MB_OK, 0);
        return false;  //立即退出程序
    }
    this->log("run communication manager succ !");

    CAppCmm*& pCmm = (CAppCmm*&)m_pAppCmm;
    pCmm = new CAppCmm(pcommainManager,this);
    if (nullptr == pCmm || !pCmm->Run()) {
        this->log("run communication moudle fail !");
        return false;  //立即退出程序
    }
    this->log("run communication moudle succ !");

    wshProxy*& pMain = (wshProxy*&)m_pMain;
    pMain->doShowLog("common module started !");

    return true;
}

bool CAppCfg::startWsc() {
    CAppWsc*& pApp = m_pAppWsc;
    if (nullptr != pApp) {
        return true;
    }

    CWorkLog* pLog = this->newLog("wsc");
    pApp = new CAppWsc(pLog,"127.0.0.1",9601);
    wshProxy*& pMain = (wshProxy*&)m_pMain;
    pApp->connect(pApp, &CAppWsc::doShowLog, pMain, &wshProxy::onShowLog, Qt::QueuedConnection);
    pApp->connect(pApp, &CAppWsc::doShowInfo, pMain, &wshProxy::onShowInfo, Qt::QueuedConnection);
    pApp->start();

    return true;
}

bool CAppCfg::startWss() {
    CAppWss*& pApp = m_pAppWss;
    if(nullptr != pApp){
        return true;
    }

    CWorkLog *pLog = this->newLog("wss");
    pApp = new CAppWss(pLog,9601);
    wshProxy*& pMain = (wshProxy*&)m_pMain;
    pApp->connect(pApp, &CAppWss::doShowLog , pMain, &wshProxy::onShowLog , Qt::QueuedConnection);
    pApp->connect(pApp, &CAppWss::doShowInfo, pMain, &wshProxy::onShowInfo, Qt::QueuedConnection);
    pApp->start();

    pApp->waitSignal(3000);

    return true;
}

bool CAppCfg::startHts() {
    CAppHts*& pApp = m_pAppHts;
    if (nullptr != pApp) {
        return true;
    }

    CWorkLog *pLog = this->newLog("hts");
    pApp = new CAppHts(pLog);
    pApp->start();

    wshProxy*& pMain = (wshProxy*&)m_pMain;
    pMain->doShowLog("http gateway started !");
   // pMain->doShowLog("http gateway started -2!");

    return true;
}

void CAppCfg::freeCmm() {
//    YQ_FREE_OBJ(m_pAppCmm);
    if (NULL == pcommainManager) {
        return;
    }

    pcommainManager->Stop();
    ::Sleep(500);
    delete pcommainManager;
    pcommainManager = NULL;

    YQ_FREE_OBJ(m_pAppCmm);
}

void CAppCfg::createSplitter(QVector<QWidget*> wgSub, QWidget* pParent) {
	splitter = new QSplitter(pParent);
	pParent->layout()->addWidget(splitter);
	splitter->setOrientation(Qt::Vertical);
	splitter->setOpaqueResize(true);//默认的true

	QWidget* pPrt;
	int i, n = wgSub.length(),nAdd = 0;
	QList<int> list;
	for (i = 0; i < n; ++i) {
		splitter->addWidget(wgSub.at(i));
		if (n - 1 == i) {
			pPrt = (QWidget*)pParent->parent();
			if (nullptr == pPrt) {
				list << (((QWidget*)pParent)->height() - nAdd);
			}
			else {
				list << (pPrt->height() - nAdd) ;
			}			
		}
		else {
			nAdd += wgSub.at(i)->minimumHeight();
			list << wgSub.at(i)->minimumHeight();
		}
		splitter->setSizes(list);
	}

}

void CAppCfg::createTree(QTreeView*& pTree,QWidget* pParent) {
    /*
    QStandardItemModel* model = new QStandardItemModel(pParent);
    model->setHorizontalHeaderLabels(QStringList() << QString::fromLocal8Bit("模块") << QString::fromLocal8Bit("状态") << QString::fromLocal8Bit("信息"));
    pTree->setModel(model);

    QStandardItem* item1 = new QStandardItem(QString::fromLocal8Bit("CMM"));
    QStandardItem* item1a = new QStandardItem(QString::fromLocal8Bit("connected"));

    model->setItem(0, 0, item1);
    model->setItem(0, 1, item1a);
    
    QStandardItem* item00 = new QStandardItem(QString::fromLocal8Bit("张三"));
    QStandardItem* item10 = new QStandardItem(QString::fromLocal8Bit("张四"));
    QStandardItem* item20 = new QStandardItem(QString::fromLocal8Bit("张五"));

    QStandardItem* item01 = new QStandardItem(QString::fromLocal8Bit("男"));
    QStandardItem* item11 = new QStandardItem(QString::fromLocal8Bit("女"));
    QStandardItem* item21 = new QStandardItem(QString::fromLocal8Bit("男"));

    QStandardItem* item02 = new QStandardItem(QString::fromLocal8Bit("15"));
    QStandardItem* item12 = new QStandardItem(QString::fromLocal8Bit("14"));
    QStandardItem* item22 = new QStandardItem(QString::fromLocal8Bit("16"));

    model->item(0, 0)->setChild(0, 0, item00);
    model->item(0, 0)->setChild(1, 0, item10);
    model->item(0, 0)->setChild(2, 0, item20);

    model->item(0, 0)->setChild(0, 1, item01);
    model->item(0, 0)->setChild(1, 1, item11);
    model->item(0, 0)->setChild(2, 1, item21);

    model->item(0, 0)->setChild(0, 2, item02);
    model->item(0, 0)->setChild(1, 2, item12);
    model->item(0, 0)->setChild(2, 2, item22);


    QStandardItem* item2 = new QStandardItem(QString::fromLocal8Bit("WSS"));
    model->setItem(1, 0, item2);

    QStandardItem* item200 = new QStandardItem(QString::fromLocal8Bit("李三"));
    QStandardItem* item210 = new QStandardItem(QString::fromLocal8Bit("李四"));
    QStandardItem* item220 = new QStandardItem(QString::fromLocal8Bit("李五"));

    QStandardItem* item201 = new QStandardItem(QString::fromLocal8Bit("男"));
    QStandardItem* item211 = new QStandardItem(QString::fromLocal8Bit("女"));
    QStandardItem* item221 = new QStandardItem(QString::fromLocal8Bit("男"));

    QStandardItem* item202 = new QStandardItem("15");
    QStandardItem* item212 = new QStandardItem("14");
    QStandardItem* item222 = new QStandardItem("16");

    model->item(1, 0)->setChild(0, 0, item200);
    model->item(1, 0)->setChild(1, 0, item210);
    model->item(1, 0)->setChild(2, 0, item220);

    model->item(1, 0)->setChild(0, 1, item201);
    model->item(1, 0)->setChild(1, 1, item211);
    model->item(1, 0)->setChild(2, 1, item221);

    model->item(1, 0)->setChild(0, 2, item202);
    model->item(1, 0)->setChild(1, 2, item212);
    model->item(1, 0)->setChild(2, 2, item222);

    pTree->setColumnWidth(0, 200);

    pTree->expandAll();
    */
}