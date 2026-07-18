#pragma once

#include <qobject.h>

#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>


#define  JK_SUCC    0
#define  JK_NULL    1
#define  JK_ERR_SEL    3

class CJsonKit :
    public QObject
{
public:
    CJsonKit(const QString sJs = "", QObject* parent = nullptr , const QString sSplNode="/", const QString sSplIdx = ":");

    bool isDigit(const QString s) { return s.contains(QRegExp("^\\d+$")); }
    bool isErr() { return  m_nErr !=0 || m_jErr.error != QJsonParseError::NoError ;  }
    QString getErr() { return m_jErr.error == QJsonParseError::NoError ? m_sErr :  m_jErr.errorString(); }

    void  setSplNode(const QString sSpl = "/") { m_sSplNode = sSpl; }
    void  setSplIdx (const QString sSpl = ":") { m_sSplIdx  = sSpl; }
    QString toString(QJsonValue* pJv = nullptr);

    bool loadStr(const QString sJs);
    bool isArray() { return m_bArray; }
    int  getArrayCount() { return m_jValRoot.toArray().count(); }

    QString   selectNode(const QString sNodes = "",const bool bSavePath = true);
    const QString  getNodeStr() { return m_sNodeCur; }

    QJsonValue getVal(const QString sKey="");
    bool       putVal(const QString& sVal, const QString& sKey = "",const bool bAuto = true);
    bool       putObj(const QJsonValue& jVal, const QString& sKey = "",bool bRm = false);
    bool       putBool(const bool bVal, const QString& sKey = "");
    bool       putNumber(const double dVal, const QString& sKey = "");
    bool       delVal(const QString& sKey="");

    QString getStr(const QString sKey, const QString* sDft=nullptr);
    int     getInt(const QString sKey, const int* nDft = nullptr);
    quint64     getLong(const QString sKey, const quint64* nDft = nullptr);
    double  getDouble(const QString sKey, const double* nDft = nullptr);
    bool    getBool(const QString sKey, const bool* bDft = nullptr);

private:
    QJsonValue      m_jValCur, m_jValRoot, m_jValCur0;
    QString         m_sNodeCur;

    QJsonParseError m_jErr;
    QJsonDocument   m_jDoc; 

    QString   m_sSplNode, m_sSplIdx;

    bool    m_bArray;
    int     m_nErr ;
    QString m_sErr ;
    QString m_vdfStr = "";
    int     m_vdfInt = 0 ;
    double  m_vdfDbl = 0.0 ;
    bool    m_vdfBool = false;
    QVariant  m_vdfVal = "";

    QString getKeyFull(const QString sKey);
    bool isCurNode(const QString sNode);
    void pubVals(QJsonValue& jRoot, const QJsonValue& jVal, const QStringList& arNode, int& idx);
    bool removeVals(QJsonValue& jRoot, const QStringList& arNode, int& idx);
};

