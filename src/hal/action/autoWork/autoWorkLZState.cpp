#include "autoWorkState.h"
#include "autoWorkAction.h"
#include "hal/communication/plcSigDef.h"
#include "hal/vm.h"
#include <QTimer>

void CSendRunType::run()
{
    assert(masterData()->colis(cpcAuotRun) == false);
    vm()->sendHold(cphRunType,m_action->m_runType);
    QTimer::singleShot(10, this, [this]{ runing(); });
}
void CSendRunType::runing()
{
    if (m_action->m_bStop)
    {
        changeState(m_action->m_idle);
        return;
    }
    if (masterData()->hold(cphRunType) == m_action->m_runType)
    {
        if (masterData()->colis(cpcAuotRun) == false)
        {
            vm()->sendDisColis(cpdcAuotRun,true);
        }
        else
        {
            vm()->sendDisColis(cpdcAuotRun,false);
            changeState(m_action->m_waitYCGVision);
            return;
        }
    }
    QTimer::singleShot(10, this, [this]{ runing(); });
}


void CWaitLZVision::run()
{
    QTimer::singleShot(10, this, [this]
                       { runing(); });
}
void CWaitLZVision::runing()
{
    if (m_action->m_bStop)
    {
        changeState(m_action->m_idle);
        return;
    }

    if (masterData()->colis(cpcLZVision))
    {
        changeState(m_action->m_LZVision);
    }
    else
    {
        changeState(m_action->m_waitYCGVision);
    }
}

void CLZVision::run()
{
    assert(masterData()->colis(cpcLZVision) == true);
    vm()->sendDisColis(cpdcLZVision, true);
    QTimer::singleShot(10, this, [this]
                       { runing(); });
}
void CLZVision::runing()
{
    if (m_action->m_bStop)
    {
        changeState(m_action->m_idle);
        return;
    }

    if (masterData()->colis(cpcLZVision) == false)
    {
        myInfo << cnStr("识别螺柱中");
        // TODO 识别
        vm()->sendHold(cphLZType, 1);
        changeState(m_action->m_waitLZAction);
    }
    else
    {
        QTimer::singleShot(10, this, [this]
                           { runing(); });
    }
}

void CWaitLZAction::run()
{
    assert(masterData()->colis(cpcLZVision) == false);
    assert(masterData()->disColis(cpdcLZAction) == false);
    assert(masterData()->disColis(cpdcLZVision) == true);
    vm()->sendDisColis(cpdcLZVision, false);
    QTimer::singleShot(10, this, [this]{ runing(); });
}
void CWaitLZAction::runing()
{
    if (m_action->m_bStop)
    {
        changeState(m_action->m_idle);
        return;
    }

    if (masterData()->disColis(cpdcLZVision) == false)
    {
        if (masterData()->disColis(cpdcLZAction) == false)
        {
            vm()->sendDisColis(cpdcLZAction, true);
        }
        else
        {
            if (masterData()->colis(cpcStratLZAction))
            {
                myDebug << cnStr("开始螺柱");
                vm()->sendDisColis(cpdcLZAction, false);
                changeState(m_action->m_waitLZVision);
                return;
            }
        }
    }
    QTimer::singleShot(10, this, [this]
                       { runing(); });
}