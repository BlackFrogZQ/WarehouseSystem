#include "controlWidget.h"
#include "ui/mainWindow.h"
#include "hal/camera/baslerCamera.h"
#include "hal/camera/baslerCameraLz.h"
#include "hal/vm.h"
#include "system/systemService.h"
#include "ui/setParaWindow/setParaWindow.h"
#include "para/define/paraDef.h"
#include <QGridLayout>
#include <QDoubleSpinBox>
#include <QMessageBox>

ControlWidget::ControlWidget(): QLabel(nullptr)
{
    this->setObjectName("ControlWidget");
    this->setStyleSheet(QString("#ControlWidget{%1};").arg(cStyleSheet ));
    initLayout();
    connect(vm(), &CVM::sigVMStateUpdate, this, &ControlWidget::vmStateUpdate);
    connect(vm(), &CVM::sigRunType, this, &ControlWidget::setRunTtpe);
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
    m_typeMate = getLed(cnStr("类型匹配"));
    m_typeMate->setFixedHeight(140);
    m_typeMate->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    //PLC控制
    m_reset = new QPushButton(cnStr("复位"));
    setAttr(m_reset);
    connect(m_reset, &QPushButton::clicked, this, [=](){vm()->reset(); });
    m_startRun = new QPushButton(cnStr("开始运行"));
    setAttr(m_startRun);
    connect(m_startRun, &QPushButton::clicked, this, [=](){vm()->autoWork(); });
    m_crashStop = new QPushButton(cnStr("停止运行"));
    setAttr(m_crashStop);
    connect(m_crashStop, &QPushButton::clicked, this, [=](){vm()->stopWork(); });

    //扭矩
    QLabel *pLabelTwist = new QLabel;
    pLabelTwist = getLed(cnStr("设置扭矩值："));
    QDoubleSpinBox* twist = new QDoubleSpinBox(this);
    twist->setMinimum(0.5);
    twist->setMaximum(5);
    twist->setSingleStep(0.5);
    twist->setValue(2);
    twist->setDecimals(1);
    twist->setSuffix(" Nm");
    setAttr(twist);
    twist->setAlignment(Qt::AlignCenter);
    twist->setStyleSheet(cStyleSheet);
    twist->setFocusPolicy(Qt::NoFocus);
    connect(twist, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &ControlWidget::setTwistPara);

    //系统参数设置
    m_systemPara = new QPushButton(cnStr("系统参数设置"));
    setAttr(m_systemPara);
    connect(m_systemPara, &QPushButton::clicked, this, [=](){
        CSetParaWindow setParaWindow;
        setParaWindow.setShowNode(TIGER_ParaDef::paraRootNode());
        setParaWindow.exec();
        if (QMessageBox::Yes ==
        QMessageBox::question(this, cnStr("保存参数"), cnStr("是否重启系统保存参数？"), QMessageBox::No | QMessageBox::Yes, QMessageBox::No))
        {
            sys()->restartSys();
        }});

    //汇总
    QGridLayout *pLayoutButton = new QGridLayout();
    pLayoutButton->addWidget(m_reset, 0, 0, 1, 1);
    pLayoutButton->addWidget(m_startRun, 0, 1, 1, 1);
    pLayoutButton->addWidget(m_crashStop, 0, 2, 1, 1);
    pLayoutButton->addWidget(pLabelTwist, 1, 0, 1, 1);
    pLayoutButton->addWidget(twist, 1, 1, 1, 1);
    pLayoutButton->addWidget(m_systemPara, 1, 2, 1, 1);
    pLayoutButton->addWidget(pLabelYcgType, 2, 0, 1, 1);
    pLayoutButton->addWidget(m_ycgType, 2, 1, 1, 1);
    pLayoutButton->addWidget(pLabelLzType, 3, 0, 1, 1);
    pLayoutButton->addWidget(m_lzType, 3, 1, 1, 1);
    pLayoutButton->addWidget(m_typeMate, 2, 2, 2, 1);
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
        m_crashStop->setEnabled(true);
        m_startRun->setEnabled(m_ycgType->text().isEmpty() == true ? false : true);
        // m_systemPara->setEnabled(true);
        break;
    case vmReset:
        m_reset->setEnabled(false);
        m_startRun->setEnabled(false);
        m_crashStop->setEnabled(true);
        // m_systemPara->setEnabled(false);
        break;
    case vmAutoWork:
        m_reset->setEnabled(false);
        m_startRun->setEnabled(false);
        m_crashStop->setEnabled(true);
        // m_systemPara->setEnabled(false);
        break;
    default:
        m_reset->setEnabled(false);
        m_startRun->setEnabled(false);
        m_crashStop->setEnabled(false);
        // m_systemPara->setEnabled(false);
        break;
    }
}

void ControlWidget::setRunTtpe(QByteArray p_runType)
{
    if(vm()->vmState() != vmIdle)
    {
        return;
    }

    if(p_runType.left(2).toInt() > typeTotalCount)
    {
        m_lzType->clear();
        m_lzType->setText(cLzTypeName[p_runType.toInt() - typeTotalCount - 1]);
    }
    else
    {
        m_ycgType->clear();
        m_ycgType->setText(cYcgTypeName[p_runType.toInt() - 1]);
    }

    if(!(m_lzType->text().isEmpty() || m_ycgType->text().isEmpty()))
    {
        if(cYcgTypeName.indexOf(m_ycgType->text()) == cLzTypeName.indexOf(m_lzType->text()) )
        {
            m_startRun->setEnabled(true);
            m_typeMate->setText(cnStr("类型匹配成功"));
            m_typeMate->setStyleSheet("QLabel{background-color:rgb(0,255,0)}");
            controlPara()->m_runType = cYcgTypeName.indexOf(m_ycgType->text()) + 1;
        }
        else
        {
            m_startRun->setEnabled(false);
            m_typeMate->setText(cnStr("类型匹配失败"));
            m_typeMate->setStyleSheet("QLabel{background-color:rgb(255,0,0)}");
        }
    }
}

void ControlWidget::setTwistPara(double p_twistPara)
{
    controlPara()->m_twist = p_twistPara * 10;
}


CControlPara* controlPara()
{
    static CControlPara gPortState;
    return &gPortState;
}

quint16 CControlPara::runType() const
{
    return m_runType;
}

quint16 CControlPara::twist() const
{
    return m_twist;
}