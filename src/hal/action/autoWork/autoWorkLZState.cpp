#include "autoWorkState.h"
#include "autoWorkAction.h"
#include "ui/mainWindow.h"
#include "hal/camera/baslerCameraLz.h"
#include "hal/communication/plcSigDef.h"
#include "hal/vm.h"
#include "discernDirectionDef.h"
#include <QTimer>

void CSendRunType::run()
{
    // dirTest()->init(17,17,50);
    assert(masterData()->colis(cpcStartRun) == false);
    vm()->sendHold(cphRunType, m_action->m_runType);
    vm()->sendHold(cphTwist, m_action->m_twist);
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
        if ((masterData()->colis(cpcStartRun) == false) && (autoRun()->getAutoRun() == false))
        {
            vm()->sendDisColis(cpdcStartRun,true);
        }
        else
        {
            autoRun()->isAutoRun = true;
            vm()->sendDisColis(cpdcStartRun,false);
            changeState(m_action->m_waitYCGVision);
            return;
        }
    }
    QTimer::singleShot(10, this, [this]{ runing(); });
}


void CWaitLZVision::run()
{
    QTimer::singleShot(10, this, [this]{ runing(); });
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
    vm()->sendHold(cphLZType, cddUndef);
    QTimer::singleShot(10, this, [this]{ runing(); });
}
void CLZVision::runing()
{
    if (m_action->m_bStop)
    {
        changeState(m_action->m_idle);
        return;
    }

    if (masterData()->hold(cphLZType) == cddUndef)
    {
        if (masterData()->colis(cpcLZVision) == false)
        {
            myInfo << cnStr("识别螺柱中");
            CDiscernDirection lzType = cddPoseError;
            bool direction;
            if(mainWindow()->slotLzImageDiscern(direction))
            {
                lzType = direction ? cddNegativeDirection: cddPositiveDirection;
            }
            vm()->sendHold(cphLZType, lzType);
            // vm()->sendHold(cphLZType, dirTest()->getTestType());
            changeState(m_action->m_waitLZAction);
            return;
        }
    }
    QTimer::singleShot(10, this, [this]{ runing(); });
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
                myInfo << cnStr("夹取螺柱");
                vm()->sendDisColis(cpdcLZAction, false);
                changeState(m_action->m_waitLZVision);
                return;
            }
        }
    }
    QTimer::singleShot(10, this, [this]{ runing(); });
}