#include "resetState.h"
#include "resetAction.h"
#include "hal/communication/plcSigDef.h"
#include "hal/vm.h"
#include <QTimer>
CResetIdleState::CResetIdleState(CResetAction *p_resetAction) : m_resetAction(p_resetAction)
{
    assert(m_resetAction != nullptr);
}

CResetIdleState::~CResetIdleState()
{
}

void CResetIdleState::run()
{
    myInfo << cnStr("复位结束") ;
    if (masterData()->disColis(cpdcReset))
    {
        vm()->sendDisColis(cpdcReset,false);
    }
    emit m_resetAction->sigEnd();
}

void CResetWaitResponseState::run()
{
    assert (masterData()->colis(cpcReset) == false);
    assert (masterData()->disColis(cpdcReset) == false);
    myInfo << cnStr("等待复位反馈") ;
    vm()->sendDisColis(cpdcReset,true);
    QTimer::singleShot(10,this,[this]{runing();});
}

void CResetWaitResponseState::runing()
{
    if (m_resetAction->m_bStop)
    {
        changeState(m_resetAction->m_idle);
        return;
    }

    if (masterData()->colis(cpcReset))
    {
        changeState(m_resetAction->m_waitFinised);
    }
    else
    {
        QTimer::singleShot(10,this,[this]{runing();});
    }
}

void CResetWaitFinisedState::run()
{
    assert (masterData()->disColis(cpdcReset) == true);
    myInfo << cnStr("plc开始复位，等待复位完成") ;
    vm()->sendDisColis(cpdcReset,false);
    QTimer::singleShot(10,this,[this]{runing();});
}

void CResetWaitFinisedState::runing()
{
    if (m_resetAction->m_bStop)
    {
        changeState(m_resetAction->m_idle);
        return;
    }

    if (masterData()->disColis(cpdcReset) == false && masterData()->colis(cpcReset) == false)
    {
        changeState(m_resetAction->m_idle);
    }
    else
    {
        QTimer::singleShot(10,this,[this]{runing();});
    }
}