#pragma once
#include "../iActionState.h"
class CStopAction;
class CStopIdleState:public IActionState
{
    Q_OBJECT
public:
    CStopIdleState(CStopAction* p_resetAction);
    ~CStopIdleState();
    virtual void run() override;
protected:
    CStopAction* m_resetAction;
};

class CStopWaitResponseState:public CStopIdleState
{
    Q_OBJECT
public:
    using CStopIdleState::CStopIdleState;
    virtual void run() override;
    virtual void runing() override;
};

class CStopWaitFinisedState:public CStopIdleState
{
    Q_OBJECT
public:
    using CStopIdleState::CStopIdleState;
    virtual void run() override;
    virtual void runing() override;
};