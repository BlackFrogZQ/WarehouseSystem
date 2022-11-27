#pragma once
#include "../iActionState.h"
#include "discernDirectionDef.h"
class CAutoWorkAction;

class CAutoWorkIdleState:public IActionState
{
    Q_OBJECT
public:
    CAutoWorkIdleState(CAutoWorkAction* p_action);
    ~CAutoWorkIdleState();
    virtual void run() override;
protected:
    CAutoWorkAction* m_action;
};


class CSendRunType:public CAutoWorkIdleState
{
    Q_OBJECT
public:
    using CAutoWorkIdleState::CAutoWorkIdleState;
    virtual void run() override;
    virtual void runing() override;
};


class CWaitYCGVision:public CAutoWorkIdleState
{
    Q_OBJECT
public:
    using CAutoWorkIdleState::CAutoWorkIdleState;
    virtual void run() override;
    virtual void runing() override;
};

class CYCGVision:public CAutoWorkIdleState
{
    Q_OBJECT
public:
    using CAutoWorkIdleState::CAutoWorkIdleState;
    virtual void run() override;
    virtual void runing() override;
};

class CWaitYCGAction:public CAutoWorkIdleState
{
    Q_OBJECT
public:
    using CAutoWorkIdleState::CAutoWorkIdleState;
    virtual void run() override;
    virtual void runing() override;
};


class CWaitLZVision:public CAutoWorkIdleState
{
    Q_OBJECT
public:
    using CAutoWorkIdleState::CAutoWorkIdleState;
    virtual void run() override;
    virtual void runing() override;
};

class CLZVision:public CAutoWorkIdleState
{
    Q_OBJECT
public:
    using CAutoWorkIdleState::CAutoWorkIdleState;
    virtual void run() override;
    virtual void runing() override;
};

class CWaitLZAction:public CAutoWorkIdleState
{
    Q_OBJECT
public:
    using CAutoWorkIdleState::CAutoWorkIdleState;
    virtual void run() override;
    virtual void runing() override;
};

class CAutoStop:public CAutoWorkIdleState
{
    Q_OBJECT
public:
    using CAutoWorkIdleState::CAutoWorkIdleState;
    virtual void run() override;
    virtual void runing() override;
};