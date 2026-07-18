#include "JsonKit.h"

/*
* /  :  根节点
* /1 或 /:1   :   数组
* /k      :   取数据或对象
* /k:1    :   取数组中的对象的数据
*/

CJsonKit::CJsonKit(const QString sJs, QObject* parent, const QString sSplNode, const QString sSplIdx) 
	: QObject(parent), m_sSplNode(sSplNode), m_sSplIdx(sSplIdx), m_bArray(false)
{
	this->loadStr(sJs);
}

QString CJsonKit::toString(QJsonValue* pJv) {
	QJsonValue* p = pJv == nullptr ? &m_jValCur : pJv;

	if (p->isArray()) {
		return QJsonDocument(p->toArray()).toJson();
	}
	else if (p->isObject()) {
		return QJsonDocument(p->toObject()).toJson();
	}
	else if (p->isBool()) {
		return QString("%1").arg(p->toBool() ? "true" : "false");
	}
	else if (p->isDouble()) {
		return QString::fromLocal8Bit("%1").arg(p->toDouble());
	}
	else if (p->isString()) {
		return p->toString();

	} else if(p->isNull()) {
		return "{}";
	}	

	return "";  //undifine
}

bool CJsonKit::loadStr(const QString sJs) {
	//{ 初始化
	m_nErr = JK_SUCC;
	m_sErr = QString::fromLocal8Bit("解析成功");
	m_sNodeCur = m_sSplNode;
	m_jValCur = m_jValCur0;
	m_jValRoot = m_jValCur;
	m_jErr.offset = 0;
	m_jErr.error = QJsonParseError::NoError;
	//}

	QJsonDocument& jDoc = m_jDoc;
	if (!sJs.isEmpty()) {
		jDoc = QJsonDocument::fromJson(sJs.toUtf8(), &m_jErr);
		if (this->isErr()) {
			return false;
		}
		if (jDoc.isNull() || jDoc.isEmpty()) {
			m_nErr = JK_NULL;
			m_sErr = QString::fromLocal8Bit("解析串为空(null|empty)");
			return false;
		}

		if (this->selectNode() == "") {
			m_nErr = 2;
			m_sErr = QString::fromLocal8Bit("获取节点失败");
			return false;
		}
	}

	return true;
}

//全路径：不存路径
//相对路径：组合后保存
QString CJsonKit::selectNode(const QString sNodes, const bool bSavePath) {
	m_nErr = JK_SUCC;

	QString strNodes = sNodes.trimmed();
	if (strNodes.isEmpty()) {
		strNodes = m_sNodeCur;
	}

	QJsonValue jRoot = m_jValRoot;
	int i0 = 1;
	if (strNodes.at(0) == m_sSplNode) {   // "" 或 "/"
		QJsonDocument& jDoc = m_jDoc;
		m_jValCur = QJsonValue::fromVariant(jDoc.isArray() ? (const QVariant&)jDoc.array() : (const QVariant&)jDoc.object());

		jRoot = m_jValCur;
		if (bSavePath) {   //save
			m_jValRoot = jRoot;
			m_bArray = m_jValRoot.isArray();
			m_sNodeCur = m_sSplNode;
		}
		if (strNodes == m_sSplNode) {   // "" 或 "/"
			return m_sNodeCur;
		}
	}
	else{
		strNodes = m_sNodeCur + strNodes + m_sSplNode;
		i0 = m_sNodeCur.split(m_sSplNode).length() - 1;

	}

	//
	QJsonValue jVal ;
	QJsonArray jArr;
	QString sKey,sKeyCur;
	QStringList arNode = strNodes.split(m_sSplNode), arCur;
	bool bIsInt;
	int i,n = arNode.length(), idx;

	m_jValCur = jRoot;
	for (i = i0 ; i < n; ++i) {
		sKeyCur = arNode[i].trimmed();
		if (sKeyCur == "") {
			continue;
		}
		if (m_jValCur.isArray()) {  //数组
			bIsInt = false;
			sKey = "";
			idx = sKeyCur.toInt(&bIsInt);
			if (!bIsInt && !sKeyCur.contains(m_sSplIdx)) {
				m_nErr = JK_ERR_SEL;
				return m_sNodeCur;
			}
			if (!bIsInt) {
				arCur = sKeyCur.split(m_sSplIdx);
				sKey = arCur[0].trimmed();
				idx = arCur.length() > 1 ? arCur[1].toInt() : 0;
				if (sKey.isEmpty()) {
					bIsInt = true;
				}
			}
			jArr = m_jValCur.toArray();
			if (idx >= jArr.count()) {
				m_nErr = JK_ERR_SEL;
				return m_sNodeCur;
			}	
			jVal = jArr.at(idx);
			if (!bIsInt) {
				if (!jVal.isObject() || !jVal.toObject().contains(sKey)) {
					m_nErr = JK_ERR_SEL;
					return m_sNodeCur;
				}
				jVal = jVal.toObject()[sKey];
			}
			m_jValCur = jVal;
			if(bSavePath){
				m_jValRoot = m_jValCur;
				m_sNodeCur += sKeyCur + m_sSplNode;    //save
			}			
		}
		else if (m_jValCur.isObject()) {
			sKey = sKeyCur;
			if (!m_jValCur.toObject().contains(sKey)) {
				m_nErr = JK_ERR_SEL;
				return m_sNodeCur;
			}
			m_jValCur = m_jValCur.toObject()[sKey];
			if (bSavePath) {
				m_jValRoot = m_jValCur;
				m_sNodeCur += sKey + m_sSplNode;    //save
			}
		}
		else {
			return m_sNodeCur;
		}
	}
	m_bArray = m_jValRoot.isArray();

	return m_sNodeCur;
}

bool CJsonKit::isCurNode(const QString sNode) {
	if (sNode.at(0) != m_sSplNode) {
		return false;
	}

	QString strNode = sNode;
	if (sNode.at(sNode.length() - 1) != m_sSplNode) {
		strNode += m_sSplNode;
	}

	return strNode == m_sNodeCur ;
}

QJsonValue CJsonKit::getVal(const QString sKey) {
	QJsonValue jv;
	if (sKey != "" && !this->isCurNode(sKey)) {
		this->selectNode(sKey,false);
		if (this->isErr()) {
			return jv;
		}
	}

	return m_jValCur;
}


QString CJsonKit::getStr(const QString sKey, const QString* sDft){
	const QString dft = sDft == nullptr ? m_vdfStr : (*sDft);

	QJsonValue jv = this->getVal(sKey);
	if (jv.isUndefined() || jv.isNull()) {
		return dft;
	}

	if (jv.isArray()) {
		return QJsonDocument(jv.toArray()).toJson();
	}
	else  if (jv.isObject()) {
		return QJsonDocument(jv.toObject()).toJson();
	}
	else if (jv.isDouble()) {
		return QString::fromLocal8Bit("%1").arg(jv.toDouble());
	}
	else if (jv.isBool()) {
		return QString::fromLocal8Bit("%1").arg(jv.toBool()?"true":"false");
	}
	
	return jv.toString();
}

int CJsonKit::getInt(const QString sKey, const int* nDft) {
	const int dft = nDft == nullptr ? m_vdfInt : (*nDft);
	QJsonValue jv = this->getVal(sKey);
	if (jv.isUndefined() || jv.isNull()) {
		return dft;
	}
	if (jv.isDouble()) {
		return jv.toInt();
	}
	
	return dft;
}

quint64  CJsonKit::getLong(const QString sKey, const quint64* nDft) {
    const quint64 dft = nDft == nullptr ? (quint64)m_vdfInt : (*nDft);
    QJsonValue jv = this->getVal(sKey);
    if (jv.isUndefined() || jv.isNull()) {
        return dft;
    }
    if (jv.isDouble()) {
        return jv.toVariant().toLongLong();
    }
    return dft;
}


double CJsonKit::getDouble(const QString sKey, const double* nDft) {
	const double dft = nDft == nullptr ? m_vdfDbl : (*nDft);
	QJsonValue jv = this->getVal(sKey);
	if (jv.isUndefined() || jv.isNull()) {
		return dft;
	}
	if (jv.isDouble()) {
		return jv.toDouble();
	}

	return dft;

}

bool CJsonKit::getBool(const QString sKey, const bool* bDft) {
	const bool dft = bDft == nullptr ? m_vdfBool : (*bDft);
	QJsonValue jv = this->getVal(sKey);
	if (jv.isUndefined() || jv.isNull()) {
		return dft;
	}
	if (jv.isBool()) {
		return jv.toBool();
	}

	return dft;
}
//----------------------
//put
QString CJsonKit::getKeyFull(const QString sKey){
	QString sKeyFull = m_sNodeCur;
	QStringList arNode = sKey.split(m_sSplNode);
	if (arNode[0] != "") {
		if (m_sNodeCur.at(m_sNodeCur.size() - 1) != m_sSplNode) {
			sKeyFull += m_sSplNode;
		}
		sKeyFull += sKey;
	}

	return sKeyFull;
}

void CJsonKit::pubVals(QJsonValue& jRoot, const QJsonValue& jVal, const QStringList& arNode, int& idx) {
	QStringList arKey;
	bool bArr = false,bAss = false, bOk;
	int iArr = -1,iIns = 0,n = arNode.length(),i,j,m;

	QString sJs = this->toString(&jRoot);

	QString sKey = arNode[idx];
	if (sKey.isEmpty()) {
		if (!jRoot.isArray()) {
			if (jVal.isArray()) {
				bAss = true;
			}
		}		
		if (jVal.isObject()) {
			if (jRoot.isArray() || jRoot.isNull()) {
				bArr = true;
			}

		} else {
			bArr = true;
		}		
	}
	else {
		if (sKey.contains(m_sSplIdx)) {
			arKey = sKey.split(m_sSplIdx);
			sKey = arKey.at(0);
			if (!arKey.at(1).isEmpty()) {
				iIns = arKey.at(1).toInt();
			}
		}		
		iArr = sKey.toInt(&bOk);
		bArr = bOk;
	}
	
	QJsonObject jObj;
	QJsonArray  jArr;
	QJsonValue  jSub, jv = jVal;

	if (bArr) {
		if (jRoot.isArray()) {
			jArr = jRoot.toArray();
            jSub = jArr.at(arNode.at(idx).toInt());
		}else{
			jRoot = QJsonValue::fromVariant(jArr);
		}
		m = jArr.count();
		for (j = m; j < iArr; ++j) {
			jArr.append("");
		}
		if (idx < n - 1) {
			pubVals(jSub, jv, arNode, ++idx);
			jv = jSub;
		}
		if (iArr >= 0 && iArr < m) {
			if (iIns > 0) {
				i = iArr + iIns - 1;
				jArr.insert(i, jv);
			}
			else {
				jArr.replace(iArr,jv);
			}
		}
		else {
			if (bAss) {
				jArr = jv.toArray();
			}
			else {
				jArr.append(jv);
			}
		}
		jRoot = QJsonValue::fromVariant(jArr);
	}
	else {
		if (jRoot.isObject()) {
			jObj = jRoot.toObject();
		}
		if (idx < n - 1) {
			if (!sKey.isEmpty() && jObj.contains(sKey)) {
				jSub = QJsonValue::fromVariant(jObj[sKey].toVariant());
			}
			pubVals(jSub, jv, arNode, ++idx);
			jv = jSub;
		}
		if (sKey.isEmpty()) {
			if (jv.isObject()) {
				jObj = jv.toObject();
			}			
		}else{
			jObj[sKey] = jv;
		}		
		jRoot = QJsonValue::fromVariant(jObj);
	}

}

//{
//1、doc根为空时，val按自身初始化
//2、doc根为数组或对象时，val按根加入
//3、/key1，在key1后添加对象；/key1/，在key1后添加数组
//4、/-1，追加数组项；/n(n>=0 && n<m)，修改；/n:1，n前插；/n:2，n后插
//5、key值相同，后值会覆盖前值
//6、bRm=true，删除key值
//}

bool CJsonKit::putObj(const QJsonValue& jVal, const QString& sKey, bool bRm) {
	QString sNodes = sKey;
	if (sNodes.isEmpty() || sNodes.at(0) != m_sSplNode) {
		sNodes = m_sNodeCur + sNodes;
	}
	QStringList arNode = sNodes.split(m_sSplNode);

	QJsonDocument& jDoc = m_jDoc;
	QJsonValue jRoot ;
	if (jDoc.isArray()) {
		jRoot = QJsonValue::fromVariant(jDoc.array());
	}
	else if (jDoc.isObject()) {
		jRoot = QJsonValue::fromVariant(jDoc.object());
	}

	int idx = 1;
	if (bRm) {
		this->removeVals(jRoot, arNode, idx);
	}
	else {
		this->pubVals(jRoot, jVal, arNode, idx);
	}	
	if (jRoot.isArray()) {
		jDoc.setArray(jRoot.toArray());
	}
	else {
		jDoc.setObject(jRoot.toObject());
	}
	this->selectNode();

	return true;
}

bool CJsonKit::putVal(const QString& sVal, const QString& sKey, const bool bAuto) {
	QJsonDocument   jDoc;
	bool bTrans = false;
	if (bAuto) {
		QJsonParseError jErr;
		jDoc = QJsonDocument::fromJson(sVal.toUtf8(), &jErr);
		if (jErr.error == QJsonParseError::NoError) {
			bTrans = true;
		}
	}

	if (bTrans) {
		if (jDoc.isArray()) {
			this->putObj(QJsonValue::fromVariant(jDoc.array()), sKey);
		}
		else if (jDoc.isObject()) {
			this->putObj(QJsonValue::fromVariant(jDoc.object()), sKey);
		}
		else {
			this->putObj(QJsonValue::fromVariant(sVal), sKey);
		}
	}
	else {
		this->putObj(QJsonValue::fromVariant(sVal), sKey);
	}

	return true;
}

bool CJsonKit::putBool(const bool bVal, const QString& sKey) {
	return this->putObj(QJsonValue::fromVariant(bVal), sKey);
}

bool CJsonKit::putNumber(const double dVal, const QString& sKey) {
	return this->putObj(QJsonValue::fromVariant(dVal), sKey);
}

bool CJsonKit::delVal(const QString& sKey) {
	QJsonDocument& jDoc = m_jDoc;

	this->putObj(QJsonValue::fromVariant(jDoc.object()), sKey, true);


	return true;
}

bool CJsonKit::removeVals(QJsonValue& jRoot, const QStringList& arNode, int& idx) {
	int n = n = arNode.length();
	if (idx < n - 1) {		
		return this->removeVals(jRoot, arNode, ++idx);
	}

	bool bRm = true;
	QString sKey = arNode[idx];
	if (sKey.isEmpty()) {
		if (jRoot.isArray()) {
		//	QJsonArray& jArr = (QJsonArray&)jRoot.toArray();
			int i,n = jRoot.toArray().count();
			while (!jRoot.toArray().empty()) {
				jRoot.toArray().removeLast();
			}
			bRm = n > 0;
			if (bRm) {
				jRoot = QJsonValue::fromVariant(jRoot.toArray());
			}
		}
		else if (jRoot.isObject()) {
		//	QJsonObject& jObj = (QJsonObject&)jRoot.toObject();
			QStringList arKey = jRoot.toObject().keys();
			int i, n = arKey.count();
			for (i = 0; i < n; ++i) {
				jRoot.toObject().remove(arKey[i]);
			}
			bRm = n>0 ;
			if (bRm) {
				jRoot = QJsonValue::fromVariant(jRoot.toObject());
			}			
		}
	}
	else {		
		if (jRoot.isArray()) {
		//	QJsonArray& jArr = (QJsonArray&)jRoot.toArray();
			bool bOk = false ;
			int i = sKey.toInt(&bOk),n = jRoot.toArray().count();
			if (bOk && i>=0 && i<n){
				jRoot.toArray().removeAt(i);
				jRoot = QJsonValue::fromVariant(jRoot.toArray());
			}
			
		} else if (jRoot.isObject()) {
		//	QJsonObject& jObj = (QJsonObject&)jRoot.toObject();
			jRoot.toObject().remove(sKey);
			jRoot = QJsonValue::fromVariant(jRoot.toObject());
		}
		else {
			bRm = false;
		}
	}

	return bRm;
}