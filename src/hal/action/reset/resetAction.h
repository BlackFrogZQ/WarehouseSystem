#pragma once
#include "../iAction.h"
class IActionState;
class CResetAction : public IAction
{
    friend class CResetIdleState;
    friend class CResetWaitResponseState;
    friend class CResetWaitFinisedState;
    Q_OBJECT
public:
    CResetAction(CVM *p_vm);
    ~CResetAction();
    virtual void start() override;
protected:
    IActionState* m_waitResponse;
    IActionState* m_waitFinised;
    IActionState* m_idle;
};
