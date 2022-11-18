#pragma once
#include "vmDef.h"
#include <QObject>
class CModbusMaster;
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

protected:
    CVM(QObject* p = nullptr);
    ~CVM();
    void changeState(CVMState nextState);

protected:
    CVMState m_state;
    CModbusMaster* m_pMaster;
    IAction* m_pResetAction;
    IAction* m_pAutoWorkAction;
};
CVM* vm();