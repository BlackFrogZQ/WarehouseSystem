#include "iAction.h"
#include "../vm.h"
#include "reset/resetAction.h"
#include "autoWork/autoWorkAction.h"
#include "stopWork/stopAction.h"
IAction::IAction(CVM *p_vm) : QObject(nullptr), m_pVM(p_vm), m_bStop(false)
{
    assert(m_pVM != nullptr);
}

IAction::~IAction()
{
}

void IAction::start()
{
    m_bStop = false;
}

void IAction::stop()
{
    m_bStop = true;
}

CActionCreater::CActionCreater(CVM *p_vm) : m_pVM(p_vm)
{
    assert(m_pVM != nullptr);
}

IAction * CActionCreater::resetAction()
{
    return new CResetAction(m_pVM);
}

IAction *CActionCreater::autoWorkAction()
{
    return new CAutoWorkAction(m_pVM);
}

IAction * CActionCreater::stopAction()
{
    return new CStopAction(m_pVM);
}


CAutoRun* autoRun()
{
    static CAutoRun gAutoRun;
    return &gAutoRun;
}

bool CAutoRun::getAutoRun() const
{
    return isAutoRun;
}