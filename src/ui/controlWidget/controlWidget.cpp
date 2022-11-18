#include "controlWidget.h"
#include "ui/mainWindow.h"
#include "ui/inputDialog/inputDialog.h"
#include "hal/camera/baslerCamera.h"
#include "hal/camera/baslerCameraLz.h"
#include "hal/vm.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QHeaderView>
#include <QKeyEvent>
using namespace HalconCpp;

ControlWidget::ControlWidget():QLabel(nullptr)
{
    this->setObjectName("ControlWidget");
    this->setStyleSheet(QString("#ControlWidget{%1};").arg(cStyleSheet ));
    initLayout();
    connect(vm(), &CVM::sigVMStateUpdate, this, &ControlWidget::vmStateUpdate);
    vmStateUpdate();
}

ControlWidget::~ControlWidget()
{
}

void ControlWidget::initLayout()
{
    auto setAttr = [](QWidget* p_w)
    {
        p_w->setFixedHeight(50);
        p_w->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    };
    auto getLed = [setAttr](QString name)
    {
        auto led = new QLabel(name);
        setAttr(led);
        led->setAlignment(Qt::AlignCenter);
        return led;
    };
    auto getLineEdit = [setAttr]()
    {
        auto led = new QLineEdit;
        setAttr(led);
        led->setAlignment(Qt::AlignCenter);
        led->setReadOnly(true);
        led->setStyleSheet(cStyleSheet);
        return led;
    };

    //装配型号
    QLabel *pLabelYcgType = new QLabel;
    pLabelYcgType = getLed(cnStr("延长杆型号："));
    m_ycgType = getLineEdit();
    QLabel *pLabelLzType = new QLabel;
    pLabelLzType = getLed(cnStr("螺柱型号："));
    m_lzType = getLineEdit();
    QLabel *pTypeMate = new QLabel;
    pTypeMate = getLed(cnStr("类型匹配成功"));
    pTypeMate->setStyleSheet("border:1px groove gray;\nborder-radius:5px;QLabel{background-color:rgb(0,255,0)}QLabel:disabled{background:transparent};");
    pTypeMate->setEnabled(false);
    pTypeMate->setFixedHeight(160);
    pTypeMate->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    //PLC控制
    m_reset = new QPushButton(cnStr("复位"));
    setAttr(m_reset);
    connect(m_reset, &QPushButton::clicked, this, [=](){vm()->reset(); });
    m_startRun = new QPushButton(cnStr("开始运行"));
    setAttr(m_startRun);
    connect(m_startRun, &QPushButton::clicked, this, [=](){vm()->autoWork(); });
    m_crashStop = new QPushButton(cnStr("停止运行"));
    setAttr(m_crashStop);

    //汇总
    QGridLayout *pLayoutButton = new QGridLayout();
    pLayoutButton->addWidget(m_reset, 0, 0, 1, 1);
    pLayoutButton->addWidget(m_startRun, 0, 1, 1, 1);
    pLayoutButton->addWidget(m_crashStop, 0, 2, 1, 1);
    pLayoutButton->addWidget(pLabelYcgType, 1, 0, 1, 1);
    pLayoutButton->addWidget(m_ycgType, 1, 1, 1, 1);
    pLayoutButton->addWidget(pLabelLzType, 2, 0, 1, 1);
    pLayoutButton->addWidget(m_lzType, 2, 1, 1, 1);
    pLayoutButton->addWidget(pTypeMate, 1, 2, 2, 1);
    pLayoutButton->setMargin(0);
    pLayoutButton->setSizeConstraint(QGridLayout::SetMinimumSize);
    this->setMinimumSize(pLayoutButton->sizeHint());
    this->setLayout(pLayoutButton);
}

void ControlWidget::vmStateUpdate()
{
    switch (vm()->vmState())
    {
    case vmIdle:
        m_reset->setEnabled(true);
        m_startRun->setEnabled(true);
        m_crashStop->setEnabled(true);
        break;
    case vmReset:
        m_reset->setEnabled(false);
        m_startRun->setEnabled(false);
        m_crashStop->setEnabled(true);
        break;
    case vmAutoWork:
        m_reset->setEnabled(false);
        m_startRun->setEnabled(false);
        m_crashStop->setEnabled(true);
        break;
    default:
        m_reset->setEnabled(false);
        m_startRun->setEnabled(false);
        m_crashStop->setEnabled(false);
        break;
    }
}