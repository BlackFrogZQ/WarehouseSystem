#include "mainWindow.h"
#include "system/tool/halconTool.h"
#include "system/basic.h"
#include "para/define/roiRegionYcgDef.h"
#include "para/define/roiRegionLzDef.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QTextBrowser>
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
using namespace TIGER_HalconTool;

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
    m_ycgStateLed = new CYcgStateLed;
    m_assembleStateLed = new CAssembleStateLed;
    initLayout();
}

CMainWindow::~CMainWindow()
{
    delPtr(m_pOutMsg);
    delPtr(m_ycgStateLed);
    delPtr(m_assembleStateLed);
}

void CMainWindow::printMsg(QString p_msg)
{
    m_pOutMsg->append(p_msg);
    m_pOutMsg->moveCursor(m_pOutMsg->textCursor().End);
}

void CMainWindow::initLayout()
{
    //输入输出窗口
    m_pOutMsg = new QTextBrowser;
    m_pOutMsg->setOpenLinks(false);
    m_pOutMsg->setOpenExternalLinks(false);
    m_pOutMsg->setStyleSheet(cStyleSheet);
    // m_pOutMsg->setFixedWidth(m_ycgImageLabel->width());

    QWidget *controlWidget = new QWidget;
    QHBoxLayout *pControlLayout = new QHBoxLayout;
    // pControlLayout->addWidget(m_pOutMsg);
    pControlLayout->addWidget(m_ycgStateLed);
    pControlLayout->addWidget(m_assembleStateLed);
    // pControlLayout->addWidget(m_controlWidget);
    pControlLayout->setMargin(0);
    pControlLayout->setSpacing(10);
    controlWidget->setLayout(pControlLayout);

    //汇总
    QVBoxLayout *pLayoutImgeAndOUtMsg = new QVBoxLayout;
    pLayoutImgeAndOUtMsg->addWidget(controlWidget);
    pLayoutImgeAndOUtMsg->setMargin(10);
    pLayoutImgeAndOUtMsg->setSpacing(2);
    this->setLayout(pLayoutImgeAndOUtMsg);
}