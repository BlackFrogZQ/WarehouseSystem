#pragma once
#include "vmDef.h"
#include "communication/serialPort/typeDef.h"
#include <QObject>
class CModbusMaster;
class CSerialPort;
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

    CVMState vmState() const;

signals:
    void sigVMStateUpdate();
    void sigPlcSigUpdate();
    void sigRunType(QByteArray p_runType);

protected:
    CVM(QObject* p = nullptr);
    ~CVM();
    void changeState(CVMState nextState);
    void setRunType(QByteArray p_readType);

protected:
    CModbusMaster* m_pMaster;
    IAction* m_pResetAction;
    IAction* m_pAutoWorkAction;
    CVMState m_state;

    CSerialPort* m_pSerialPort;
};
CVM* vm();