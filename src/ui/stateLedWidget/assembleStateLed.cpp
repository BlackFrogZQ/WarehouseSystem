#include "assembleStateLed.h"
#include "ui/mainWindowDef.h"
#include "ui/setParaWindow/setParaWindow.h"
#include "hal/vm.h"
#include "para/define/roiRegionLzDef.h"
#include <QVBoxLayout>

CAssembleStateLed::CAssembleStateLed(QWidget *parent) : QLabel(parent)
{
    this->setObjectName("PlcStateWidget");
    this->setStyleSheet(QString("#PlcStateWidget{%1};").arg(cStyleSheet ));
    auto getLed = [](QString name)
    {
        auto led = new QLabel(name);
        led->setStyleSheet("QLabel{background-color:rgb(0,255,0)}QLabel:disabled{background:transparent};");
        led->setFixedHeight(100);
        led->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        led->setAlignment(Qt::AlignCenter);
        return led;
    };
    for(int i = 0; i < cytMax - 1; i++)
    {
        m_states[i] = getLed(cAssembleTypeName[i]);
    }
    initLayout();
    connect(vm(), &CVM::sigRunType, this, &CAssembleStateLed::assembleReduceStorage);
    connect(vm(), &CVM::sigPushStorageType, this, &CAssembleStateLed::assembleAddStorage);
}

CAssembleStateLed::~CAssembleStateLed()
{
}

void CAssembleStateLed::initLayout()
{
    //类型
    QWidget *plcStateWidget = new QWidget;
    QGridLayout *plcStateLayout = new QGridLayout;
    for(int i = 0; i < cytMax - 1; i++)
    {
        plcStateLayout->addWidget(m_states[i], i/5, i%5, 1, 1);
    }
    plcStateLayout->setMargin(0);
    plcStateLayout->setVerticalSpacing(100);
    plcStateLayout->setSizeConstraint(QGridLayout::SetMaximumSize);
    plcStateWidget->setLayout(plcStateLayout);

    //标题
    QWidget *titleWidget = new QWidget;
    QLabel *titleLable = new QLabel(cnStr("装配件仓库"));
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
    setAllLed();
}

void CAssembleStateLed::setAllLed()
{
    for (size_t i = 0; i < cytMax-1; i++)
    {
        m_states[i]->setEnabled(TIGER_LzRoiDef::lzRoiParas()->assemblyStates[i]);
    }
}

void CAssembleStateLed::assembleReduceStorage(const int &p_readType)
{
    if(p_readType < typeTotalCount*2)
    {
        return;
    }
    m_states[p_readType-typeTotalCount*2-1]->setEnabled(false);
    TIGER_LzRoiDef::lzRoiParas()->assemblyStates[p_readType-typeTotalCount*2-1] = false;
    CSetParaWindow setParaWindow;
    setParaWindow.setShowNode(TIGER_ParaDef::paraRootNode());
    setParaWindow.isSave();
}

void CAssembleStateLed::assembleAddStorage(const int &p_readType)
{
    if(p_readType < typeTotalCount*2)
    {
        return;
    }
    m_states[p_readType-typeTotalCount*2-1]->setEnabled(true);
    TIGER_LzRoiDef::lzRoiParas()->assemblyStates[p_readType-typeTotalCount*2-1] = true;
    CSetParaWindow setParaWindow;
    setParaWindow.setShowNode(TIGER_ParaDef::paraRootNode());
    setParaWindow.isSave();
}