#pragma once
#include "communication/serialPort/typeDef.h"
#include <QObject>

class CPullStorage;
class CPushStorage;

class CVM:public QObject
{
    friend class CVmControl;
    Q_OBJECT

protected:
    CVM(QObject* p = nullptr);
    ~CVM();

signals:
    void sigPushStorageType(const int &p_readType);
    void sigPullStorageType(const int &p_readType);

protected:
    CPushStorage* m_pPushStorage;
    CPullStorage* m_pPullStorage;
};
CVM* vm();