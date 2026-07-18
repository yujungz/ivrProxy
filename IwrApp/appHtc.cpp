#include "IwrApp.h"

CAppHtc::CAppHtc(CWorkLog*& pLog)
	: CWorkTrd(pLog)
{
    connect(this, &CAppHtc::doSend2User, this, &CAppHtc::onSend2User, Qt::QueuedConnection);
}

CAppHtc::~CAppHtc()
{

}

void CAppHtc::onSend2User(const QString sUrl,const QString sData) {
    CAppCfg*& pCfg = gpCfg;
    IwrApp* pMain = (IwrApp*)pCfg->getMain();
    
    pMain->doShowLog(QString("[Send2User] url(%1)data(%2)")
        .arg(sUrl).arg(sData));

    QString strResult;
    int ret = pCfg->SendReq(sUrl, sData, strResult, false);
    //int ret = pCfg->SendReq("http://www.ai-seed.cn1/", "", strResult);

    //pCfg->getCmm()->SendRespDirectRequestData(sid, ret, (char*)strResult.toLatin1().constData());
    pMain->doShowLog(QString("[RespUserData] ret(%1)data(%2)")
        .arg(ret).arg(strResult));

}