#include "autoWorkAction.h"
#include "autoWorkState.h"
#include "ui/controlWidget/controlWidget.h"
#include "hal/vm.h"

CAutoWorkAction::CAutoWorkAction(CVM *p_vm): IAction(p_vm), m_runType(0xffff)
{
    m_idle = new CAutoWorkIdleState(this);
    m_autoStop = new CAutoStop(this);
    m_sendRunType = new CSendRunType(this);

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
    delPtr(m_autoStop);
    delPtr(m_sendRunType);
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
    m_runType = controlPara()->runType();
    m_twist = controlPara()->twist();
    m_sendRunType->run();
}

void CAutoWorkAction::stop()
{
    IAction::stop();
}
