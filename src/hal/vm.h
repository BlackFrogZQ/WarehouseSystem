#pragma once
#include "communication/serialPort/typeDef.h"
#include <QObject>

class CModbusMaster;
class CSerialPort;
class CPushStorage;
class IAction;

class CVM:public QObject
{
    friend class CVmControl;
    Q_OBJECT
public:
    bool sendDisColis(int p_addr,bool p_value);
    bool sendHold(int p_addr,quint16 p_value);

    void reset();
    void stoReset();
    void autoWork();
    void stopWork();

signals:
    void sigVMStateUpdate();
    void sigPlcSigUpdate();
    void sigRunType(const int &p_readType);
    void sigPushStorageType(const int &p_readType);

protected:
    CVM(QObject* p = nullptr);
    ~CVM();

protected:
    CModbusMaster* m_pMaster;

    CSerialPort* m_pSerialPort;
    CPushStorage* m_pPushStorage;
};
CVM* vm();