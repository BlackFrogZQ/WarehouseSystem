#pragma once
#include "../iAction.h"
class IActionState;
class CStopAction : public IAction
{
    friend class CStopIdleState;
    friend class CStopWaitResponseState;
    friend class CStopWaitFinisedState;
    Q_OBJECT
public:
    CStopAction(CVM *p_vm);
    ~CStopAction();
    virtual void start() override;
protected:
    IActionState* m_waitResponse;
    IActionState* m_waitFinised;
    IActionState* m_idle;
};
