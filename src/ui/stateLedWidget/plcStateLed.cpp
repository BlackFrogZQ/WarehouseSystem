#include "plcStateLed.h"
#include "ui/mainWindowDef.h"
#include "hal/communication/plcSigDef.h"
#include "hal/vm.h"
#include <QVBoxLayout>

CPlcStateLed::CPlcStateLed(QWidget *parent) : QLabel(parent)
{
    this->setObjectName("PlcStateWidget");
    this->setStyleSheet(QString("#PlcStateWidget{%1};").arg(cStyleSheet ));
    
    auto getLed = [](QString name)
    {
        auto led = new QLabel(name);
        led->setStyleSheet("QLabel{background-color:rgb(0,255,0)}QLabel:disabled{background:transparent};");
        led->setFixedHeight(50);
        led->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        led->setAlignment(Qt::AlignCenter);
        return led;
    };

    m_states[reset] = getLed(cnStr("  复位  "));
    m_states[YCGVision] = getLed(cnStr("延长杆\n 识别 "));
    m_states[YCGAction] = getLed(cnStr("延长杆\n 装配 "));
    m_states[LZVision] = getLed(cnStr("螺柱 \n 识别 "));
    m_states[LZAction] = getLed(cnStr("螺柱 \n 装配 "));
    m_states[autoRun] = getLed(cnStr("自动运行"));
    initLayout();
    connect(vm(),&CVM::sigVMStateUpdate,this,&CPlcStateLed::plcStateUpdate);
    connect(vm(),&CVM::sigPlcSigUpdate,this,&CPlcStateLed::plcStateUpdate);
    plcStateUpdate();
}

CPlcStateLed::~CPlcStateLed()
{
}

void CPlcStateLed::initLayout()
{
    //信号
    QWidget *plcStateWidget = new QWidget;
    QGridLayout *plcStateLayout = new QGridLayout;
    plcStateLayout->addWidget(m_states[reset], 0, 0, 1, 1);
    plcStateLayout->addWidget(m_states[autoRun], 0, 1, 1, 1);
    plcStateLayout->addWidget(m_states[YCGVision], 1, 0, 1, 1);
    plcStateLayout->addWidget(m_states[YCGAction], 1, 1, 1, 1);
    plcStateLayout->addWidget(m_states[LZVision], 2, 0, 1, 1);
    plcStateLayout->addWidget(m_states[LZAction], 2, 1, 1, 1);
    plcStateLayout->setMargin(0);
    plcStateLayout->setVerticalSpacing(30);
    plcStateLayout->setSizeConstraint(QGridLayout::SetMinimumSize);
    plcStateWidget->setLayout(plcStateLayout);

    //标题
    QWidget *titleWidget = new QWidget;
    QLabel *titleLable = new QLabel(cnStr("指示信号"));
    titleLable->setStyleSheet("QLabel{background-color:rgb(255, 255, 255,15)};");
    QFont titleFont;
    titleFont.setPointSize(12);
    titleLable->setFont(titleFont);
    titleLable->setFixedHeight(20);
    titleLable->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    titleLable->setAlignment(Qt::AlignCenter);

    QHBoxLayout *pTitleLayout = new QHBoxLayout;
    pTitleLayout->addWidget(titleLable);
    pTitleLayout->setMargin(0);
    pTitleLayout->setSizeConstraint(QHBoxLayout::SetMinimumSize);
    titleWidget->setLayout(pTitleLayout);

    //汇总
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(titleWidget);
    pLayout->addWidget(plcStateWidget);
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    pLayout->setSizeConstraint(QVBoxLayout::SetMinimumSize);
    this->setMinimumSize(pLayout->sizeHint());
    this->setLayout(pLayout);
}

void CPlcStateLed::setAllLed(bool p_enable)
{
    for (size_t i = 0; i < cltMax; i++)
    {
        m_states[i]->setEnabled(p_enable);
    }
}

void CPlcStateLed::plcStateUpdate()
{
    switch (vm()->vmState())
    {
    case vmIdle:
        setAllLed(false);
        break;
    case vmReset:
        setAllLed(false);
        m_states[reset]->setEnabled(true);
        break;
    case vmAutoWork:
        setAllLed(false);
        m_states[autoRun]->setEnabled(true);
        m_states[YCGVision]->setEnabled(masterData()->colis(cpcYCGVision));
        m_states[YCGAction]->setEnabled(!m_states[YCGVision]->isEnabled());
        m_states[LZVision]->setEnabled(masterData()->colis(cpcLZVision));
        m_states[LZAction]->setEnabled(!m_states[LZVision]->isEnabled());
        break;
    default:
        setAllLed(false);
        break;
    }
}