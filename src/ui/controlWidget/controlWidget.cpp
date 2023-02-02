#include "controlWidget.h"
#include "ui/mainWindow.h"
#include "ui/setParaWindow/setParaWindow.h"
#include "hal/communication/serialPort/serialPortLed.h"
#include "hal/vm.h"
#include "system/systemService.h"
#include "para/define/paraDef.h"
#include <QGridLayout>
#include <QDoubleSpinBox>
#include <QMessageBox>

ControlWidget::ControlWidget(): QLabel(nullptr)
{
    this->setObjectName("ControlWidget");
    this->setStyleSheet(QString("#ControlWidget{%1};").arg(cStyleSheet ));
    initLayout();
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

    //PLC控制
    m_reset = new QPushButton(cnStr("复位"));
    setAttr(m_reset);
    connect(m_reset, &QPushButton::clicked, this, [=](){vm()->reset(); });
    m_startRun = new QPushButton(cnStr("开始运行"));
    setAttr(m_startRun);
    connect(m_startRun, &QPushButton::clicked, this, [=](){vm()->autoWork(); });
    m_crashStop = new QPushButton(cnStr("停止运行"));
    setAttr(m_crashStop);
    // connect(m_crashStop, &QPushButton::clicked, this, [=](){
    //     if(vm()->vmState() == vmIdle){return;}
    //     vm()->stopWork();});

    //装配型号
    QLabel *pLabelYcgType = new QLabel;
    pLabelYcgType = getLed(cnStr("延长杆型号："));
    m_ycgType = getLineEdit();
    m_ycgType->setPlaceholderText(cnStr("延长杆型号"));
    QLabel *pLabelLzType = new QLabel;
    pLabelLzType = getLed(cnStr("螺柱型号："));
    m_lzType = getLineEdit();
    m_lzType->setPlaceholderText(cnStr("螺柱型号"));
    m_assemblyType = new QPushButton(cnStr("选择装配类型"));
    setAttr(m_assemblyType);

    //扭矩
    QLabel *pLabelTwist = new QLabel;
    pLabelTwist = getLed(cnStr("设置装配扭矩："));
    QDoubleSpinBox* twist = new QDoubleSpinBox(this);
    twist->setMinimum(0.5);
    twist->setMaximum(30);
    twist->setSingleStep(0.5);
    twist->setValue(2);
    twist->setDecimals(1);
    twist->setSuffix(" Nm");
    setAttr(twist);
    twist->setAlignment(Qt::AlignCenter);
    twist->setStyleSheet(cStyleSheet);
    twist->setFocusPolicy(Qt::NoFocus);
    //系统参数设置
    m_systemPara = new QPushButton(cnStr("系统参数设置"));
    setAttr(m_systemPara);
    connect(m_systemPara, &QPushButton::clicked, this, [=](){
        CSetParaWindow setParaWindow;
        setParaWindow.setShowNode(TIGER_ParaDef::paraRootNode());
        bool isSave = setParaWindow.isSave();
        isSave ? sys()->save() : sys()->load();
        if(isSave)
        {
            if(QMessageBox::Yes ==
            QMessageBox::question(this, cnStr("加载参数"), cnStr("是否重启系统加载参数？"), QMessageBox::No | QMessageBox::Yes, QMessageBox::No))
            {
                sys()->restartSys();
            }
        }});

    //计数
    QLabel *pLabelAll = new QLabel;
    pLabelAll = getLed(cnStr("装配总数："));
    QLabel *pLabelOk = new QLabel;
    pLabelOk = getLed(cnStr("合格个数："));
    QLabel *pLabelNg = new QLabel;
    pLabelNg = getLed(cnStr("不合格数："));
    m_allCount = new QLabel("0");
    setAttr(m_allCount);
    m_allCount->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_okCount = new QLabel("0");
    setAttr(m_okCount);
    m_okCount->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_ngCount = new QLabel("0");
    setAttr(m_ngCount);
    m_ngCount->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    //汇总
    QGridLayout *pLayoutButton = new QGridLayout();
    pLayoutButton->addWidget(m_reset, 0, 0, 1, 2);
    pLayoutButton->addWidget(m_startRun, 0, 2, 1, 2);
    pLayoutButton->addWidget(m_crashStop, 0, 4, 1, 2);

    pLayoutButton->addWidget(m_ycgType, 1, 0, 1, 2);
    pLayoutButton->addWidget(m_lzType, 1, 2, 1, 2);
    pLayoutButton->addWidget(m_assemblyType, 1, 4, 1, 2);

    pLayoutButton->addWidget(pLabelTwist, 2, 0, 1, 2);
    pLayoutButton->addWidget(twist, 2, 2, 1, 2);
    pLayoutButton->addWidget(m_systemPara, 2, 4, 1, 2);

    pLayoutButton->addWidget(pLabelAll, 3, 0, 1, 1);
    pLayoutButton->addWidget(m_allCount, 3, 1, 1, 1);
    pLayoutButton->addWidget(pLabelOk, 3, 2, 1, 1);
    pLayoutButton->addWidget(m_okCount, 3, 3, 1, 1);
    pLayoutButton->addWidget(pLabelNg, 3, 4, 1, 1);
    pLayoutButton->addWidget(m_ngCount, 3, 5, 1, 1);
    pLayoutButton->setMargin(0);
    pLayoutButton->setSizeConstraint(QGridLayout::SetMinimumSize);
    this->setMinimumSize(pLayoutButton->sizeHint());
    this->setLayout(pLayoutButton);
}