#include "resetAction.h"
#include "resetState.h"

#include "hal/vm.h"
CResetAction::CResetAction(CVM *p_vm):IAction(p_vm)
{
    m_idle = new CResetIdleState(this);
    m_waitResponse = new CResetWaitResponseState(this);
    m_waitFinised = new CResetWaitFinisedState(this);
}

CResetAction::~CResetAction()
{
    delPtr(m_waitResponse);
    delPtr(m_waitFinised);
    delPtr(m_idle);
}

void CResetAction::start()
{
    IAction::start();
    m_waitResponse->run();
}