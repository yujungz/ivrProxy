#pragma once

#include <WorkTrd.h>

class CAppHts  : public CWorkTrd
{
	Q_OBJECT

public:
	CAppHts(CWorkLog*& pLog);
	~CAppHts();

public slots:
    void onDirectRequestData(const long sid, const int cid, const QString sData);

signals:
    void doDirectRequestData(const long sid, const int cid, const QString sData);
};
