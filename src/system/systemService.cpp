﻿#include "system/systemService.h"
#include "logSystem/logDef.h"
#include "ui/basic.h"
#include "para/para.h"
#include "hal/camera/baslerCamera.h"
#include "hal/camera/baslerCameraLz.h"
#include "hal/communication/serialPort/serialPortLed.h"
#include "hal/vm.h"
#include "para/define/cameraDef.h"
#include "para/define/cameraLzDef.h"
#include "ui/mainWindow.h"
#include <HalconCpp.h>
#include <QDateTime>
#include <QProcess>
#include <QApplication>

void logOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

class CVmControl : public ISystemService
{
public:
    CVmControl();
    void init();
    virtual void printMsg(QString p_msg);
    virtual void updateUiImage(const QImage &p_image);
    virtual void setOnlyImage(bool p_isOnlyImage = true);
    virtual void restartSys() const;

    void load();
    void save();

protected:
    ~CVmControl();

protected:
    QStringList m_msgBuffer;
    CVM* m_pVm;
};



CVmControl::CVmControl()
    : ISystemService()
{
}

CVmControl::~CVmControl()
{
    delPtr(m_pVm);
    qWarning() << cnStr("程序结束");
}

void CVmControl::init()
{
    m_pVm = new CVM;
    qWarning() << cnStr("程序开始");
}

void CVmControl::printMsg(QString p_msg)
{
    CLogDef::writeLog(CLogDef::getInfoLogName(), QDateTime::currentDateTime().toString("[hh.mm.ss]:%1").arg(p_msg));
    p_msg = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss]:%1").arg(p_msg);
    m_msgBuffer << p_msg;

    if (mainWindow() != nullptr)
    {
        for (const auto &msg : m_msgBuffer)
        {
            mainWindow()->printMsg(msg);
        }
        m_msgBuffer.clear();
    }
}

void CVmControl::updateUiImage(const QImage &p_image)
{
    if (mainWindow() != nullptr)
    {
        mainWindow()->slotUpdateYcgImage(p_image);
        mainWindow()->slotUpdateLzImage(p_image);
    }
}

void CVmControl::setOnlyImage(bool p_isOnlyImage)
{
    if (mainWindow() != nullptr)
    {
        // mainWindow()->slotIsOnlyShowImage(p_isOnlyImage);
    }
}

void CVmControl::restartSys() const
{
    qApp->exit();
    QProcess::startDetached(qApp->applicationFilePath(), QStringList());
}


void CVmControl::load()
{
    paraService()->load();
}

void CVmControl::save()
{
    paraService()->save();
}

ISystemService *g_sysService = nullptr;
ISystemService *sys()
{
    return g_sysService;
};



void initSystemService()
{
    try
    {
        HalconCpp::SetSystem("clip_region", "false");
    }
    catch (HalconCpp::HException &except)
    {
        CLogDef::writeLog(CLogDef::getInfoLogName(), QDateTime::currentDateTime().toString("[hh.mm.ss]:%1").arg(QString(except.ErrorMessage().Text())));
    }

    CUiBasic::updateScale(1920, 1080);

    g_sysService = new CVmControl;
    qInstallMessageHandler(logOutput);
    sys()->load();
    ((CVmControl *)g_sysService)->init();

    baslerCamera()->setIP(TIGER_CameraDef::cameraParas()->ip);
    baslerCamera()->setExposureTime(TIGER_CameraDef::cameraParas()->exposureTime);
    baslerCamera()->setHeartbeatTimeout(TIGER_CameraDef::cameraParas()->heartbeatTime);
    baslerCameraLz()->setIP(TIGER_CameraLzDef::cameraParasLz()->ip);
    baslerCameraLz()->setExposureTime(TIGER_CameraLzDef::cameraParasLz()->exposureTime);
    baslerCameraLz()->setHeartbeatTimeout(TIGER_CameraLzDef::cameraParasLz()->heartbeatTime);
}

void closeSystemService()
{
    led()->setAll(CLED::clsOFF, CLED::clsOFF, CLED::clsOFF, CLED::clsOFF);
    sys()->save();
    delete g_sysService;
    g_sysService = nullptr;
}