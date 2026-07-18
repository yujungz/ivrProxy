#include "wshProxy.h"

CAppHts::CAppHts(CWorkLog*& pLog)
	: CWorkTrd(pLog)
{
    connect(this, &CAppHts::doDirectRequestData, this, &CAppHts::onDirectRequestData, Qt::QueuedConnection);
}

CAppHts::~CAppHts()
{

}


void CAppHts::onDirectRequestData(const long sid, const int cid, const QString sData) {
    CAppCfg*& pCfg = gpCfg;
    wshProxy* pMain = (wshProxy*)pCfg->getMain();
    
    pMain->doShowLog(QString("[RecvDirectRequestData] sessionId(0x%1)cid(%2)data(%3)")
        .arg(sid, 8, 16, QChar('0')).arg(cid).arg(sData));

    QString strResult,sData0 = sData;

    //sData0 = "http://www.ai-seed.cn/tp/public/TestRws/|post_prm1=post_val1&post_prm2=post_val2";

    QStringList arUrl = sData0.split("?");
    int nUrl = arUrl.length();
    QString sPost = "";
    if (nUrl > 1 && 1 == cid ) {
        sPost = arUrl.at(1);
    }
    
    bool bPost = (0 < cid);
    QString sUrl = (1 == cid) ? arUrl.at(0) : sData0;
    int ret = pCfg->SendReq(sUrl, sPost, strResult, bPost);
    //int ret = pCfg->SendReq("http://www.ai-seed.cn1/", "", strResult);

    pCfg->getCmm()->SendRespDirectRequestData(sid, ret, (char*)strResult.toLatin1().constData());
    pMain->doShowLog(QString("[RespDirectRequestData] ret(%1)data(%2)")
        .arg(ret).arg(strResult));

}