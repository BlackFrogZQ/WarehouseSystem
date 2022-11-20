#pragma once
#include "../iAction.h"
class IActionState;
class CAutoWorkAction : public IAction
{
    friend class CAutoWorkIdleState;
    friend class CWaitYCGVision;
    friend class CWaitLZVision;
    friend class CYCGVision;
    friend class CLZVision;
    friend class CWaitYCGAction;
    friend class CWaitLZAction;
    friend class CSendRunType;

    Q_OBJECT
public:
    CAutoWorkAction(CVM *p_vm);
    ~CAutoWorkAction();

    virtual void start() override;

protected:
    IActionState* m_sendRunType;
    quint16 m_runType;

    IActionState* m_waitYCGVision;
    IActionState* m_YCGVision;
    IActionState* m_waitYCGAction;

    IActionState* m_waitLZVision;
    IActionState* m_LZVision;
    IActionState* m_waitLZAction;

    IActionState* m_idle;
};
