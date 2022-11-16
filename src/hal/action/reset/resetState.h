#pragma once
#include "../iActionState.h"
class CResetAction;
class CResetIdleState:public IActionState
{
    Q_OBJECT
public:
    CResetIdleState(CResetAction* p_resetAction);
    ~CResetIdleState();
    virtual void run() override;
protected:
    CResetAction* m_resetAction;
};

class CResetWaitResponseState:public CResetIdleState
{
    Q_OBJECT
public:
    using CResetIdleState::CResetIdleState;
    virtual void run() override;
    virtual void runing() override;
};

class CResetWaitFinisedState:public CResetIdleState
{
    Q_OBJECT
public:
    using CResetIdleState::CResetIdleState;
    virtual void run() override;
    virtual void runing() override;
};