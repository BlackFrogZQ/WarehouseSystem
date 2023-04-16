#include "stopState.h"
#include "stopAction.h"
#include "hal/communication/plcSigDef.h"
#include "hal/vm.h"
#include <QTimer>

CStopIdleState::CStopIdleState(CStopAction *p_resetAction) : m_resetAction(p_resetAction)
{
    assert(m_resetAction != nullptr);
}

CStopIdleState::~CStopIdleState()
{
}

void CStopIdleState::run()
{
    myInfo << cnStr("设备停止") ;
    if (masterData()->disColis(cpdcStopRun))
    {
        vm()->sendDisColis(cpdcStopRun,false);
    }
    emit m_resetAction->sigEnd();
}

void CStopWaitResponseState::run()
{
    assert (masterData()->colis(cpcStopRun) == false);
    assert (masterData()->disColis(cpdcStopRun) == false);
    myInfo << cnStr("等待停止反馈") ;
    vm()->sendDisColis(cpdcStopRun,true);
    QTimer::singleShot(10,this,[this]{runing();});
}

void CStopWaitResponseState::runing()
{
    if (masterData()->colis(cpcStopRun))
    {
        changeState(m_resetAction->m_waitFinised);
    }
    else
    {
        QTimer::singleShot(10,this,[this]{runing();});
    }
}

void CStopWaitFinisedState::run()
{
    assert (masterData()->disColis(cpdcStopRun) == true);
    myInfo << cnStr("plc开始停止，等待停止完成") ;
    vm()->sendDisColis(cpdcStopRun,false);
    QTimer::singleShot(10,this,[this]{runing();});
}

void CStopWaitFinisedState::runing()
{
    if (masterData()->disColis(cpdcStopRun) == false && masterData()->colis(cpcStopRun) == false)
    {
        changeState(m_resetAction->m_idle);
    }
    else
    {
        QTimer::singleShot(10,this,[this]{runing();});
    }
}