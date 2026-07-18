#pragma once

#include <QSplitter>
#include <appExt.h>

#include <JsonKit.h>
#include <bufPool.h>

#include "CommManager.h"
#include "appCmm.h"
#include "appWss.h"
#include "appWsc.h"
#include "appHts.h"

class CAppCfg : public CAppExt{
public:
	CAppCfg(const int& argc0, const char**& argv0);
	~CAppCfg();

    void setMain(void* p) { m_pMain = p; }
    void* getMain() { return m_pMain; }
    bool showLog(const QString& sLog,const bool show = false);
	bool run();
	void stop();
	void createSplitter(QVector<QWidget*> wgSub, QWidget* pParent);
    void createTree(QTreeView*& pTree, QWidget* pParent);
    void initTextBrowser(void*& pTb, const int n = 100);
    void initTree(void*& pTree,const QString& sHead);
 //   void initInfoTree(void*& pTree);
    void showInfo(const QString& sKey, const QString& sInfo,  const QString& sAddr = "");
    const bool ifQuitApp();
    int  SendReq_old(QString strUrl, QString strInput, QString& strResult,const bool bPost = true);
    int  SendReq(QString strUrl, QString strInput, QString& strResult, const bool bPost = true);

    CAppHts*& getHts() { return m_pAppHts; }
    CAppCmm*& getCmm() { return m_pAppCmm; }
private:
    void* m_pMain;

    QString m_sPathOrig;

	TCommManager* pcommainManager;
    CAppCmm* m_pAppCmm;
    CAppWss* m_pAppWss;
    CAppWsc* m_pAppWsc;
    CAppHts* m_pAppHts;

    int   m_nLog;
    void* m_pTb,*m_pTree;
	QSplitter* splitter;

    bool startCmm();
    bool startWss();
    bool startWsc();
    bool startHts();
    
    void freeCmm();

    bool findTreeKey(const QString& sKey, int& idxHead, int& idxSub);
};