#include "autoWorkAction.h"
#include "autoWorkState.h"

#include "hal/vm.h"

CAutoWorkAction::CAutoWorkAction(CVM *p_vm):IAction(p_vm),m_runType(0xffff)
{
    m_sendRunType = new CSendRunType(this);

    m_idle = new CAutoWorkIdleState(this);

    m_waitYCGVision= new CWaitYCGVision(this);
    m_YCGVision= new CYCGVision(this);
    m_waitYCGAction= new CWaitYCGAction(this);

    m_waitLZVision= new CWaitLZVision(this);
    m_LZVision= new CLZVision(this);
    m_waitLZAction= new CWaitLZAction(this);
}

CAutoWorkAction::~CAutoWorkAction()
{
    delPtr(m_idle);
    delPtr(m_waitYCGVision);
    delPtr(m_YCGVision);
    delPtr(m_waitYCGAction);
    delPtr(m_waitLZVision);
    delPtr(m_LZVision);
    delPtr(m_waitLZAction);
}

void CAutoWorkAction::start()
{
    IAction::start();
    // TODO 启动类型
    m_runType = 7;
    m_sendRunType->run();
}
