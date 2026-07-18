#pragma once

#include <WorkTrd.h>

class CAppHtc  : public CWorkTrd
{
	Q_OBJECT

public:
    CAppHtc(CWorkLog*& pLog);
	~CAppHtc();

public slots:
    void onSend2User(const QString sUrl,const QString sData);

signals:
    void doSend2User(const QString sUrl,const QString sData);
};
