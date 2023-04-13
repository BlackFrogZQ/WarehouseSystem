#pragma once
#include "../iAction.h"
class IActionState;
class CAutoWorkAction : public IAction
{
    friend class CAutoWorkIdleState;
    friend class CSendRunType;
    friend class CPlcFinish;
    friend class CWaitYCGVision;
    friend class CWaitLZVision;
    friend class CYCGVision;
    friend class CLZVision;
    friend class CWaitYCGAction;
    friend class CWaitLZAction;

    Q_OBJECT
public:
    CAutoWorkAction(CVM *p_vm);
    ~CAutoWorkAction();

    virtual void start() override;
    virtual void stop() override;

protected:
    IActionState* m_idle;
    IActionState* m_plcFinish;
    IActionState* m_sendRunType;
    quint16 m_runType;
    quint16 m_twist;

    IActionState* m_waitYCGVision;
    IActionState* m_YCGVision;
    IActionState* m_waitYCGAction;

    IActionState* m_waitLZVision;
    IActionState* m_LZVision;
    IActionState* m_waitLZAction;
};
