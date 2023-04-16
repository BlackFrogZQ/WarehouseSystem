#include "assemblyTypeWindow.h"
#include "hal/communication/serialPort/typeDef.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QVBoxLayout>

CAssemblyType::CAssemblyType(QWidget *parent) : QDialog(parent)
{
    setWindowFlag(Qt::WindowStaysOnTopHint, false);
    setWindowFlag(Qt::WindowCloseButtonHint, false);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    setWindowTitle(cnStr("选择匹配类型"));
    setFixedSize(600, 840);
    setAutoFillBackground(true);
    initLayout();
}

CAssemblyType::~CAssemblyType()
{
}

void CAssemblyType::initLayout()
{
    m_pChoiceType = new QTableWidget(this);
    m_pChoiceType->setColumnCount(2);
    m_pChoiceType->setRowCount(25);
    m_pChoiceType->setHorizontalHeaderLabels(QStringList() << cnStr("延长杆型号")<< cnStr("螺柱型号"));
    m_pChoiceType->setStyleSheet("QHeaderView::section{background:transparent; color:black;}");
    m_pChoiceType->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_pChoiceType->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pChoiceType->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pChoiceType->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for(int col = 0; col < 2; col++)
    {
        for(int row = 0; row < cytMax - 1; row++)
        {
            m_pChoiceType->setItem(row, col, new QTableWidgetItem(col == 0? cYcgTypeName[row] : cLzTypeName[row]));
            m_pChoiceType->item (row, col)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }
    }
    connect(m_pChoiceType,&QTableWidget::clicked,this,&CAssemblyType::setCurrentItem);

    m_pSave = new QPushButton(cnStr("确定"));
    m_pSave->setEnabled(false);
    connect(m_pSave,&QPushButton::clicked,this,[this](){m_isSave = true; this->close();});
    auto pCancel= new QPushButton(cnStr("取消"));
    connect(pCancel,&QPushButton::clicked,this,[this](){m_isSave = false; this->close();});
    QHBoxLayout *pbLayout = new QHBoxLayout;
    pbLayout->addStretch();
    pbLayout->addWidget(m_pSave);
    pbLayout->addWidget(pCancel);
    pbLayout->setMargin(0);

    QVBoxLayout *hLayout = new QVBoxLayout;
    hLayout->addWidget(m_pChoiceType);
    hLayout->addLayout(pbLayout);
    this->setLayout(hLayout);
}

void CAssemblyType::setCurrentItem(const QModelIndex  &p_index)
{
    m_pSave->setEnabled(true);
    m_item = p_index.row();
}

bool CAssemblyType::getCurrentItem(int &p_item)
{
    this->exec();
    p_item = m_item;
    return m_isSave;
}