#include "mainWindow.h"
#include "hal/camera/baslerCamera.h"
#include "hal/camera/baslerCameraLz.h"
#include "system/tool/halconTool.h"
#include "system/basic.h"
#include "imageProcess/imageProcess.h"
#include "para/define/roiRegionYcgDef.h"
#include "para/define/roiRegionLzDef.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QTextBrowser>
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
using namespace TIGER_HalconTool;
using namespace TIGER_ProcessTool;

CMainWindow *g_pMainWindow = nullptr;
CMainWindow *mainWindow()
{
    return g_pMainWindow;
};

CMainWindow::CMainWindow(QWidget *parent)
    : QDialog(parent)
{
    g_pMainWindow = this;
    // setFixedSize(1920, 1080);// 1626 1236
    m_controlWidget = new ControlWidget;
    m_plcStateLed = new CPlcStateLed;
    m_ycgRoiPath.addRect(TIGER_YcgRoiDef::ycgRoiParas()->x, TIGER_YcgRoiDef::ycgRoiParas()->y, TIGER_YcgRoiDef::ycgRoiParas()->w, TIGER_YcgRoiDef::ycgRoiParas()->h);
    m_ycgRoiPath.moveTo(TIGER_YcgRoiDef::ycgRoiParas()->x + (TIGER_YcgRoiDef::ycgRoiParas()->w)/2, TIGER_YcgRoiDef::ycgRoiParas()->y);
    m_ycgRoiPath.lineTo(TIGER_YcgRoiDef::ycgRoiParas()->x + (TIGER_YcgRoiDef::ycgRoiParas()->w)/2, TIGER_YcgRoiDef::ycgRoiParas()->y + TIGER_YcgRoiDef::ycgRoiParas()->h);
    m_lzRoiPath.addRect(TIGER_LzRoiDef::lzRoiParas()->x, TIGER_LzRoiDef::lzRoiParas()->y, TIGER_LzRoiDef::lzRoiParas()->w, TIGER_LzRoiDef::lzRoiParas()->h);
    m_lzRoiPath.moveTo(TIGER_LzRoiDef::lzRoiParas()->x + (TIGER_LzRoiDef::lzRoiParas()->w)/2, TIGER_LzRoiDef::lzRoiParas()->y);
    m_lzRoiPath.lineTo(TIGER_LzRoiDef::lzRoiParas()->x + (TIGER_LzRoiDef::lzRoiParas()->w)/2, TIGER_LzRoiDef::lzRoiParas()->y + TIGER_LzRoiDef::lzRoiParas()->h);
    initLayout();
    mainWindow()->slotVideoImage(baslerCamera()->getExposureTime(), baslerCameraLz()->getExposureTime());
    connect(baslerCamera(), &TIGER_BaslerCamera::CBaslerCamera::sigGrabImage, this, &CMainWindow::slotUpdateYcgImage);
    connect(baslerCameraLz(), &TIGER_BaslerCamera::CBaslerCameraLz::sigGrabImage, this, &CMainWindow::slotUpdateLzImage);
}

CMainWindow::~CMainWindow()
{
    delPtr(m_pOutMsg);
    delPtr(m_controlWidget);
    delPtr(m_plcStateLed);
}

void CMainWindow::printMsg(QString p_msg)
{
    m_pOutMsg->append(p_msg);
    m_pOutMsg->moveCursor(m_pOutMsg->textCursor().End);
}

void CMainWindow::initLayout()
{
    //图像窗口
    m_ycgImageLabel = new QLabel;
    m_ycgImageLabel->setFixedSize(950,722);
    m_lzImageLabel = new QLabel;
    m_lzImageLabel->setFixedSize(m_ycgImageLabel->size());

    QWidget *widgetAllImage = new QWidget;
    QHBoxLayout *pLayoutAllImage = new QHBoxLayout;
    pLayoutAllImage->addWidget(m_ycgImageLabel);
    pLayoutAllImage->addWidget(m_lzImageLabel);
    pLayoutAllImage->setMargin(0);
    pLayoutAllImage->setSpacing(2);
    widgetAllImage->setStyleSheet(cStyleSheet);
    widgetAllImage->setLayout(pLayoutAllImage);

    //输入输出窗口
    m_pOutMsg = new QTextBrowser;
    m_pOutMsg->setOpenLinks(false);
    m_pOutMsg->setOpenExternalLinks(false);
    m_pOutMsg->setStyleSheet(cStyleSheet);
    m_pOutMsg->setFixedWidth(m_ycgImageLabel->width());

    QWidget *controlWidget = new QWidget;
    QHBoxLayout *pControlLayout = new QHBoxLayout;
    pControlLayout->addWidget(m_pOutMsg);
    pControlLayout->addWidget(m_plcStateLed);
    pControlLayout->addWidget(m_controlWidget);
    pControlLayout->setMargin(0);
    pControlLayout->setSpacing(10);
    controlWidget->setLayout(pControlLayout);

    //汇总
    QVBoxLayout *pLayoutImgeAndOUtMsg = new QVBoxLayout;
    pLayoutImgeAndOUtMsg->addWidget(widgetAllImage);
    pLayoutImgeAndOUtMsg->addWidget(controlWidget);
    pLayoutImgeAndOUtMsg->setMargin(10);
    pLayoutImgeAndOUtMsg->setSpacing(2);
    this->setLayout(pLayoutImgeAndOUtMsg);
}

void CMainWindow::slotVideoImage(double p_YcgExposureTime, double p_LzExposureTime)
{
    baslerCamera()->setExposureTime(p_YcgExposureTime);
    if (!(baslerCamera()->connected() && baslerCamera()->acquireChange()))
    {
        qInfo() << cnStr("延长杆相机打开失败:%1").arg(baslerCamera()->getLastError());
    }
    baslerCameraLz()->setExposureTime(p_LzExposureTime);
    if (!(baslerCameraLz()->connected() && baslerCameraLz()->acquireChange()))
    {
        qInfo() << cnStr("螺柱相机打开失败:%1").arg(baslerCameraLz()->getLastError());
    }
}

void CMainWindow::slotUpdateYcgImage(const QImage &p_image)
{
    m_ycgImage = p_image;
    if(TIGER_YcgRoiDef::ycgRoiParas()->isShow)
    {
        QPainter painter(&m_ycgImage);
        painter.setPen(QPen(Qt::blue,4));
        painter.drawPath(m_ycgRoiPath);
    }
    m_ycgImageLabel->setPixmap(QPixmap::fromImage(m_ycgImage).scaled(m_ycgImageLabel->size(), Qt::KeepAspectRatio));
}

bool CMainWindow::slotGrabYcgImage()
{
    if (baslerCamera()->isAcquire())
    {
        qInfo() << cnStr("延长杆相机正在采集，正在尝试停止相机采集");
        if (baslerCamera()->acquireChange())
        {
            qInfo() << cnStr("停止相机采集成功");
        }
        else
        {
            qInfo() << cnStr("停止相机采集失败，无法拍照识别");
            return false;
        }
    }
    if (!(baslerCamera()->connected() && baslerCamera()->singleFrame()))
    {
        qInfo() << cnStr("相机采集失败，无法拍照识别");
        return false;
    }
    return true;
}

bool CMainWindow::slotYcgImageDiscern(bool &p_direction)
{
    if(!slotGrabYcgImage()){return false;}
    TIGER_ProcessTool::CStationMatch stationMatch;
    if (stationMatch.processImage(true, baslerCamera()->getLastImage(), m_ycgRoiPath))
    {
        myInfo << cnStr("延长杆处理成功。");
    }
    else
    {
        qInfo() << cnStr("延长杆处理失败：%1").arg(stationMatch.getErrorMsg());
        return false;
    }
    slotUpdateYcgImage(stationMatch.getMarkImage());
    p_direction = stationMatch.getDirection();
    return true;
}


void CMainWindow::slotUpdateLzImage(const QImage &p_image)
{
    m_lzImage = p_image;
    if(TIGER_LzRoiDef::lzRoiParas()->isShow)
    {
        QPainter painter(&m_lzImage);
        painter.setPen(QPen(Qt::blue,4));
        painter.drawPath(m_lzRoiPath);
    }
    m_lzImageLabel->setPixmap(QPixmap::fromImage(m_lzImage).scaled(m_lzImageLabel->size(), Qt::KeepAspectRatio));
}

bool CMainWindow::slotGrabLzImage()
{
    if (baslerCameraLz()->isAcquire())
    {
        qInfo() << cnStr("螺柱相机正在采集，正在尝试停止相机采集");
        if (baslerCameraLz()->acquireChange())
        {
            qInfo() << cnStr("停止相机采集成功");
        }
        else
        {
            qInfo() << cnStr("停止相机采集失败，无法拍照识别");
            return false;
        }
    }
    if (!(baslerCameraLz()->connected() && baslerCameraLz()->singleFrame()))
    {
        qInfo() << cnStr("相机采集失败，无法拍照识别");
        return false;
    }
    return true;
}

bool CMainWindow::slotLzImageDiscern(bool &p_direction)
{
    if(!slotGrabLzImage()){return false;}
    TIGER_ProcessTool::CStationMatch stationMatch;
    if (stationMatch.processImage(false, baslerCameraLz()->getLastImage(), m_lzRoiPath))
    {
        qInfo() << cnStr("螺柱处理成功。");
    }
    else
    {
        qInfo() << cnStr("螺柱处理失败：%1").arg(stationMatch.getErrorMsg());
        return false;
    }
    slotUpdateLzImage(stationMatch.getMarkImage());
    p_direction = stationMatch.getDirection();
    return true;
}