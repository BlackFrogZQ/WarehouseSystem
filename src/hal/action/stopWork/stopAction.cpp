#include "stopAction.h"
#include "stopState.h"
#include "hal/vm.h"

CStopAction::CStopAction(CVM *p_vm):IAction(p_vm)
{
    m_idle = new CStopIdleState(this);
    m_waitResponse = new CStopWaitResponseState(this);
    m_waitFinised = new CStopWaitFinisedState(this);
}

CStopAction::~CStopAction()
{
    delPtr(m_waitResponse);
    delPtr(m_waitFinised);
    delPtr(m_idle);
}

void CStopAction::start()
{
    IAction::start();
    autoRun()->isAutoRun = false;
    m_waitResponse->run();
}