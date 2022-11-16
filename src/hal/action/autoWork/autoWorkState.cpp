#include "autoWorkState.h"
#include "autoWorkAction.h"
#include "hal/communication/plcSigDef.h"
#include "hal/vm.h"
#include <QTimer>
CAutoWorkIdleState::CAutoWorkIdleState(CAutoWorkAction *p_action) : m_action(p_action)
{
    assert(m_action != nullptr);
}

CAutoWorkIdleState::~CAutoWorkIdleState()
{
}

void CAutoWorkIdleState::run()
{
    myInfo << cnStr("自动工作结束");
    if (masterData()->disColis(cpdcYCGAction))
    {
        vm()->sendDisColis(cpdcYCGAction, false);
    }
    if (masterData()->disColis(cpdcYCGVision))
    {
        vm()->sendDisColis(cpdcYCGVision, false);
    }
    if (masterData()->disColis(cpdcLZAction))
    {
        vm()->sendDisColis(cpdcLZAction, false);
    }
    if (masterData()->disColis(cpdcLZVision))
    {
        vm()->sendDisColis(cpdcLZVision, false);
    }
    emit m_action->sigEnd();
}


void CWaitYCGVision::run()
{
    QTimer::singleShot(10, this, [this]
                       { runing(); });
}
void CWaitYCGVision::runing()
{
    if (m_action->m_bStop)
    {
        changeState(m_action->m_idle);
        return;
    }

    if (masterData()->colis(cpcYCGVision))
    {
        changeState(m_action->m_YCGVision);
    }
    else
    {
        changeState(m_action->m_waitLZVision);
    }
}

void CYCGVision::run()
{
    assert(masterData()->colis(cpcYCGVision) == true);
    vm()->sendDisColis(cpdcYCGVision, true);
    QTimer::singleShot(10, this, [this]
                       { runing(); });
}
void CYCGVision::runing()
{
    if (m_action->m_bStop)
    {
        changeState(m_action->m_idle);
        return;
    }

    if (masterData()->colis(cpcYCGVision) == false)
    {
        // TODO 识别
        myInfo << cnStr("识别延长杆中");

        vm()->sendHold(cphYCGType, 1);
        changeState(m_action->m_waitYCGAction);
    }
    else
    {
        QTimer::singleShot(10, this, [this]
                           { runing(); });
    }
}

void CWaitYCGAction::run()
{
    assert(masterData()->colis(cpcYCGVision) == false);
    assert(masterData()->disColis(cpdcYCGAction) == false);
    assert(masterData()->disColis(cpdcYCGVision) == true);
    vm()->sendDisColis(cpdcYCGVision, false);
    QTimer::singleShot(10, this, [this]
                       { runing(); });
}
void CWaitYCGAction::runing()
{
    if (m_action->m_bStop)
    {
        changeState(m_action->m_idle);
        return;
    }

    if (masterData()->disColis(cpdcYCGVision) == false)
    {
        if (masterData()->disColis(cpdcYCGAction) == false)
        {
            vm()->sendDisColis(cpdcYCGAction, true);
        }
        else
        {
            if (masterData()->colis(cpcStratYCGAction))
            {
                myInfo << cnStr("夹取延长杆");
                vm()->sendDisColis(cpdcYCGAction, false);
                changeState(m_action->m_waitLZVision);
                return;
            }
        }
    }
    QTimer::singleShot(10, this, [this]
                       { runing(); });
}