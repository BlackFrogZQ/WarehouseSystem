#include "mainWindow.h"
#include "hal/camera/baslerCamera.h"
#include "hal/camera/baslerCameraLz.h"
#include "system/tool/halconTool.h"
#include "imageProcess/createRoi/createRoi.h"
#include "imageProcess/discernDirection/iDiscernDirection.h"
#include "imageProcess/imageProcess.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QTextBrowser>
#include <QEvent>
#include <QMouseEvent>
using namespace TIGER_HalconTool;
using namespace TIGER_ProcessTool;

CMainWindow *g_pMainWindow = nullptr;
CMainWindow *mainWindow()
{
    return g_pMainWindow;
};

CMainWindow::CMainWindow(QWidget *parent)
    : QDialog(parent), m_drawWindowHandle(cDefHandleValue), m_showYcg(true)
{
    g_pMainWindow = this;
    this->setWindowTitle("TestVision");
    setFixedSize(1100, 800);
    m_controlWidget = NULL;
    m_controlWidget = new ControlWidget();
    initLayout();
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint&~Qt::WindowMaximizeButtonHint);
}

CMainWindow::~CMainWindow()
{
    delPtr(m_pImageLabel);
    delPtr(m_pOutMsg);
    delPtr(m_controlWidget);
}

void CMainWindow::printMsg(QString p_msg)
{
    m_pOutMsg->append(p_msg);
    m_pOutMsg->moveCursor(m_pOutMsg->textCursor().End);
}

void CMainWindow::initLayout()
{
    m_pImageLabel = new QLabel;
    m_pImageLabel->setScaledContents(true);
    m_pImageLabel->setStyleSheet(cStyleSheet);

    m_ycgImageLabel = new QLabel;
    m_lzImageLabel = new QLabel;
    m_ycgImageLabel->installEventFilter(this);
    m_lzImageLabel->installEventFilter(this);
    m_ycgImageLabel->setScaledContents(true);
    m_lzImageLabel->setScaledContents(true);
    m_ycgImageLabel->setStyleSheet(cStyleSheet);
    m_lzImageLabel->setStyleSheet(cStyleSheet);
    QWidget *widgetTwoImage = new QWidget;
    QHBoxLayout *pLayoutTwoImage = new QHBoxLayout;
    QSpacerItem * sparcer_item = new QSpacerItem(400, 0, QSizePolicy::Fixed, QSizePolicy::Expanding);
    pLayoutTwoImage->addItem(sparcer_item);
    pLayoutTwoImage->addWidget(m_ycgImageLabel);
    pLayoutTwoImage->addWidget(m_lzImageLabel);
    pLayoutTwoImage->setMargin(2);
    widgetTwoImage->setLayout(pLayoutTwoImage);
    widgetTwoImage->setFixedHeight(100);

    QWidget *widgetAllImage = new QWidget;
    QVBoxLayout *pLayoutAllImage = new QVBoxLayout;
    pLayoutAllImage->addWidget(m_pImageLabel);
    pLayoutAllImage->addWidget(widgetTwoImage);
    pLayoutAllImage->setMargin(2);
    pLayoutAllImage->setSpacing(1);
    widgetAllImage->setObjectName("widgetAllImage");
    widgetAllImage->setStyleSheet(cWindowStyleSheet);
    widgetAllImage->setLayout(pLayoutAllImage);

    m_pOutMsg = new QTextBrowser;
    m_pOutMsg->setOpenLinks(false);
    m_pOutMsg->setOpenExternalLinks(false);
    m_pOutMsg->setStyleSheet(cStyleSheet);
    m_pOutMsg->setFixedHeight(200);
    m_pOutMsg->document()->setMaximumBlockCount(300);

    QVBoxLayout *pLayoutImgeAndOUtMsg = new QVBoxLayout;
    pLayoutImgeAndOUtMsg->addWidget(widgetAllImage);
    pLayoutImgeAndOUtMsg->addWidget(m_pOutMsg);
    pLayoutImgeAndOUtMsg->setMargin(2);
    pLayoutImgeAndOUtMsg->setSpacing(10);
    QWidget *widgetImgeAndOUtMsg = new QWidget;
    widgetImgeAndOUtMsg->setLayout(pLayoutImgeAndOUtMsg);

    QWidget *widgetMainWindow = new QWidget;
    m_controlWidget->setFixedWidth(400);
    QHBoxLayout *pLayoutMainWindow = new QHBoxLayout;
    pLayoutMainWindow->addWidget(widgetImgeAndOUtMsg);
    pLayoutMainWindow->addWidget(m_controlWidget);
    widgetMainWindow->setLayout(pLayoutMainWindow);
    this->setLayout(pLayoutMainWindow);

    connect(baslerCamera(), &TIGER_BaslerCamera::CBaslerCamera::sigGrabImage, this, &CMainWindow::slotUpdateYcgImage);
    connect(baslerCameraLz(), &TIGER_BaslerCamera::CBaslerCameraLz::sigGrabImage, this, &CMainWindow::slotUpdateLzImage);
}

void CMainWindow::slotUpdateYcgImage(const QImage &p_image)
{
    m_ycgImage = p_image;
    if(m_showYcg == true)
    {
        m_pImageLabel->setPixmap(QPixmap::fromImage(p_image).scaled(m_pImageLabel->size(), Qt::KeepAspectRatio));
    }
    m_ycgImageLabel->setPixmap(QPixmap::fromImage(p_image).scaled(m_ycgImageLabel->size(), Qt::KeepAspectRatio));
}

void CMainWindow::slotUpdateLzImage(const QImage &p_image)
{
    m_lzImage = p_image;
    if(m_showYcg == false)
    {
        m_pImageLabel->setPixmap(QPixmap::fromImage(p_image).scaled(m_pImageLabel->size(), Qt::KeepAspectRatio));
    }
    m_lzImageLabel->setPixmap(QPixmap::fromImage(p_image).scaled(m_lzImageLabel->size(), Qt::KeepAspectRatio));
}

void CMainWindow::slotVideoImage(double p_YcgExposureTime, double p_LzExposureTime)
{
    baslerCamera()->setExposureTime(p_YcgExposureTime);
    if (!(baslerCamera()->connected() && baslerCamera()->acquireChange()))
    {
        qInfo() << cnStr("相机打开失败:%1").arg(baslerCamera()->getLastError());
    }

    baslerCameraLz()->setExposureTime(p_LzExposureTime);
    if (!(baslerCameraLz()->connected() && baslerCameraLz()->acquireChange()))
    {
        qInfo() << cnStr("相机打开失败:%1").arg(baslerCameraLz()->getLastError());
    }
}

void CMainWindow::slotGrabImage()
{
    if (baslerCamera()->isAcquire())
    {
        qInfo() << cnStr("相机正在采集，正在尝试停止相机采集");
        if (baslerCamera()->acquireChange())
        {
            qInfo() << cnStr("停止相机采集成功");
        }
        else
        {
            qInfo() << cnStr("停止相机采集失败，无法拍照识别");
        }
    }
    if (!(baslerCamera()->connected() && baslerCamera()->singleFrame()))
    {
        qInfo() << cnStr("相机采集失败，无法拍照识别");
    }

    if (baslerCameraLz()->isAcquire())
    {
        qInfo() << cnStr("相机正在采集，正在尝试停止相机采集");
        if (baslerCameraLz()->acquireChange())
        {
            qInfo() << cnStr("停止相机采集成功");
        }
        else
        {
            qInfo() << cnStr("停止相机采集失败，无法拍照识别");
        }
    }
    if (!(baslerCameraLz()->connected() && baslerCameraLz()->singleFrame()))
    {
        qInfo() << cnStr("相机采集失败，无法拍照识别");
    }

    slotUpdateYcgImage(baslerCamera()->getLastImage());
    slotUpdateLzImage(baslerCameraLz()->getLastImage());
}

void CMainWindow::drawRectangle2Region()
{
    setWidgetHandle(m_drawWindowHandle, m_pImageLabel);
    slotGrabImage();
    CCreateRoi createRoi;
    if(!createRoi.createRoi(m_drawWindowHandle, m_showYcg))
    {
        qInfo() << cnStr("保存失败:1%").arg(createRoi.getErrorMsg());
    }
}

bool CMainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == m_ycgImageLabel)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() == Qt::LeftButton)
            {
                m_showYcg = true;
                slotUpdateYcgImage(m_ycgImage);
                qInfo() << cnStr("主窗口显示的是延长杆");
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    else if(obj == m_lzImageLabel)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() == Qt::LeftButton)
            {
                m_showYcg = false;
                slotUpdateLzImage(m_lzImage);
                qInfo() << cnStr("主窗口显示的是螺柱");
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

bool CMainWindow::slotImageDiscern(const bool &p_useWindow, const bool &p_processYcg, bool &p_direction)
{
    if(p_useWindow == false)
    {
        m_showYcg = p_processYcg;
    }

    TIGER_ProcessTool::CStationMatch stationMatch;
    if (stationMatch.processImage(m_showYcg))
    {
        if(m_showYcg == true)
        {
            qInfo() << cnStr("延长杆处理成功。");
        }
        else
        {
            qInfo() << cnStr("螺柱处理成功。");
        }
    }
    else
    {
        if(m_showYcg == true)
        {
            slotUpdateYcgImage(stationMatch.getMarkImage());
            qInfo() << cnStr("延长杆处理失败：%1").arg(stationMatch.getErrorMsg());
        }
        else
        {
            slotUpdateLzImage(stationMatch.getMarkImage());
            qInfo() << cnStr("螺柱处理失败：%1").arg(stationMatch.getErrorMsg());
        }
        return false;
    }
    p_direction = stationMatch.getDirection();
    qInfo() << p_direction;
    return true;
}