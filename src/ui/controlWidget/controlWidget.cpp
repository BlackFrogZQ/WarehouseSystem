#include "controlWidget.h"
#include "ui/mainWindow.h"
#include "ui/inputDialog/inputDialog.h"
#include "hal/camera/baslerCamera.h"
#include "hal/camera/baslerCameraLz.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QThread>
#include <QKeyEvent>
#include <QTextCursor>
using namespace HalconCpp;

ControlWidget::ControlWidget():m_modbusTcp(nullptr), m_pData(0), m_ycgIsTrigger(false), m_lzIsTrigger(false)
{
    m_modbusTcp = new CModbusTcp;
    m_timer = new QTimer();
    initLayout();
    connect(m_timer, &QTimer::timeout, this, &ControlWidget::slotChangeWrite);
}

ControlWidget::~ControlWidget()
{
    delPtr(m_modbusTcp);
}

void ControlWidget::initLayout()
{
    //曝光与录制
    m_pYcgExposureTime = baslerCamera()->getExposureTime();
    m_pYcgExposureTimeButton = new QPushButton(cYcgExposureTimeName.arg(m_pYcgExposureTime / 1000));
    connect(m_pYcgExposureTimeButton, &QPushButton::clicked, [=](){slotExposureTimePB(true);});
    m_pLzExposureTime = baslerCameraLz()->getExposureTime();
    m_pLzExposureTimeButton = new QPushButton(cLzExposureTimeName.arg(m_pLzExposureTime / 1000));
    connect(m_pLzExposureTimeButton, &QPushButton::clicked, [=](){slotExposureTimePB(false);});
    m_pVideoImageButton = new QPushButton(cStartAcquireName);
    connect(m_pVideoImageButton, &QPushButton::clicked, [=](){mainWindow()->slotVideoImage(m_pYcgExposureTime, m_pLzExposureTime);});
    connect(baslerCamera(), &TIGER_BaslerCamera::CBaslerCamera::sigAcquireStateChanged, this, [this]()
            { m_pVideoImageButton->setText(baslerCamera()->isAcquire() ? cStopAcquireName : cStartAcquireName); });

    //通信
    QRegExp rx("^[A-Za-z0-9`~!@#$%^&*()_-+=<>,.\\\\]+$");//禁止用中文
    QRegExpValidator *latitude = new QRegExpValidator(rx, this);
    QLabel *pLabelPort = new QLabel(cnStr("扫描型号："));
    pLabelPort->setAlignment(Qt::AlignCenter);
    m_ycgType = new QLineEdit;
    m_ycgType->setPlaceholderText(cnStr("请扫描延长杆型号"));
    m_ycgType->installEventFilter(this);
    m_ycgType->setValidator(latitude);
    m_lzType = new QLineEdit;
    m_lzType->setPlaceholderText(cnStr("请扫描螺柱型号"));
    m_lzType->installEventFilter(this);
    m_lzType->setValidator(latitude);
    connect(m_ycgType, &QLineEdit::textChanged, this, &ControlWidget::slotScanYcgQRCode);
    connect(m_lzType, &QLineEdit::textChanged, this, &ControlWidget::slotScanLzQRCode);

    m_connectModbus = new QPushButton(cConnectModbusName);
    m_connectModbus->setEnabled(false);
    connect(m_connectModbus, &QPushButton::clicked, [=](){m_connectModbus->setText(slotConnectModbus() ? cDisconnectModbusName : cConnectModbusName);});

    //类型选择与运行
    QPushButton *pDrawRoi = new QPushButton(cnStr("绘制ROI"));
    connect(pDrawRoi, &QPushButton::clicked, this,[=](){ mainWindow()->drawRectangle2Region();});
    QPushButton *m_pDiscernImageButton = new QPushButton(cnStr("算法测试"));
    connect(m_pDiscernImageButton, &QPushButton::clicked, this,[=](){ mainWindow()->slotGrabImage(); mainWindow()->slotImageDiscern(true, false, m_ycgDirection);});
    m_pStartRun = new QPushButton(cStartRun);
    m_pStartRun->setEnabled(false);
    connect(m_pStartRun, &QPushButton::clicked, this, [=](){ m_pStartRun->setText(slotStartRun() ? cStopRun : cStartRun); });

    //PLC输入输出
    m_write = new QPushButton(cnStr("写数据"));
    m_write->setEnabled(false);
    connect(m_write, &QPushButton::clicked, [=](){slotInitialData();});
    m_read = new QPushButton(cnStr("读数据"));
    m_read->setEnabled(false);
    connect(m_read, &QPushButton::clicked, [=](){slotReadModbusRegisters();});
    m_writeRead = new QPushButton(cnStr("读写数据"));
    m_writeRead->setEnabled(false);
    connect(m_writeRead, &QPushButton::clicked, [=](){slotInitialData(); QThread::sleep(1); slotReadModbusRegisters();});

    //表格
    m_pTableWrite = new QTableWidget(this);//写表格
    m_pTableWrite->setColumnCount(TableColumnNum);
    m_pTableWrite->setRowCount(TableRowNum);
    m_pTableWrite->setHorizontalHeaderLabels(QStringList() << cnStr("写入数据"));
    m_pTableWrite->setStyleSheet(cTableStyleSheet);
    m_pTableWrite->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_pTableRead = new QTableWidget(this);//读表格
    m_pTableRead->setColumnCount(TableColumnNum);
    m_pTableRead->setRowCount(TableRowNum);
    m_pTableRead->setHorizontalHeaderLabels(QStringList() << cnStr("读取数据"));
    m_pTableRead->setStyleSheet(cTableStyleSheet);
    m_pTableRead->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_pTableRead->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for(int i = 0; i < TableRowNum; i++)
    {
        m_pTableWrite->setItem(i, 0, new QTableWidgetItem(cnStr("00")));
        m_pTableRead->setItem(i, 0, new QTableWidgetItem(cnStr("00")));
    }

    //汇总
    QWidget *pWidgetButton = new QWidget;
    QGridLayout *pLayoutButton = new QGridLayout();
    pLayoutButton->addWidget(pLabelPort, 0, 0, 1, 1);
    pLayoutButton->addWidget(m_ycgType, 0, 1, 1, 2);
    pLayoutButton->addWidget(m_lzType, 0, 3, 1, 2);
    pLayoutButton->addWidget(m_connectModbus, 0, 5, 1, 1);
    pLayoutButton->addWidget(m_pYcgExposureTimeButton, 1, 0, 1, 2);
    pLayoutButton->addWidget(m_pLzExposureTimeButton, 1, 2, 1, 2);
    pLayoutButton->addWidget(m_pVideoImageButton, 1, 4, 1, 2);
    pLayoutButton->addWidget(pDrawRoi, 2, 0, 1, 2);
    pLayoutButton->addWidget(m_pDiscernImageButton, 2, 2, 1, 2);
    pLayoutButton->addWidget(m_pStartRun, 2, 4, 1, 2);
    pLayoutButton->addWidget(m_write, 3, 0, 1, 2);
    pLayoutButton->addWidget(m_read, 3, 2, 1, 2);
    pLayoutButton->addWidget(m_writeRead, 3, 4, 1, 2);
    pLayoutButton->setVerticalSpacing(10);
    pWidgetButton->setLayout(pLayoutButton);

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(pWidgetButton);
    pLayout->addWidget(m_pTableWrite);
    pLayout->addWidget(m_pTableRead);
    this->setLayout(pLayout);
}

void ControlWidget::slotExposureTimePB(bool p_ycgExposureTime)
{
    double exposureTime = 0;
    if (CInputDialog::getDouble(exposureTime, cnStr("设置曝光"), cnStr("相机曝光时间(ms):"), (p_ycgExposureTime == true ? m_pYcgExposureTime : m_pLzExposureTime) / 1000, 1, 280, 1))
    {
        if(p_ycgExposureTime == true)
        {
            m_pYcgExposureTime = exposureTime * 1000;
            baslerCamera()->setExposureTime(m_pYcgExposureTime);
            m_pYcgExposureTimeButton->setText(cYcgExposureTimeName.arg(exposureTime));
        }
        else
        {
            m_pLzExposureTime = exposureTime * 1000;
            baslerCameraLz()->setExposureTime(m_pLzExposureTime);
            m_pLzExposureTimeButton->setText(cLzExposureTimeName.arg(exposureTime));
        }
    }
}

void ControlWidget::slotScanYcgQRCode(QString p_ycgType)
{
    if(m_ycgType->text().size() > TypeLength)
    {
        m_ycgType->setText(p_ycgType.right(TypeLength));
    }
    if(m_lzType->text().isEmpty())
    {
        if(m_ycgType->text().size() == TypeLength)
        {
            m_lzType->setFocus ();
        }
        return;
    }
    m_connectModbus->setEnabled(true);
}
void ControlWidget::slotScanLzQRCode(QString p_LzType)
{
    if(m_lzType->text().size() > TypeLength)
    {
        m_lzType->setText(p_LzType.right(TypeLength));
    }
    if(m_ycgType->text().isEmpty())
    {
        if(m_lzType->text().size() == TypeLength)
        {
            m_ycgType->setFocus ();
        }
        return;
    }
    m_connectModbus->setEnabled(true);
}
bool ControlWidget::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == m_ycgType)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() == Qt::LeftButton)
            {
                m_ycgType->setFocus();
                QTimer::singleShot(0, m_ycgType, &QLineEdit::selectAll);
                return true;
            }
        }
    }
    else if(obj == m_lzType)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() == Qt::LeftButton)
            {
                m_lzType->setFocus();
                QTimer::singleShot(0, m_lzType, &QLineEdit::selectAll);
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

bool ControlWidget::slotConnectModbus()
{
    if(!(m_ycgType->text().mid(0,2) == m_lzType->text().mid(0,2) && ((m_ycgType->text().mid(2,1) == "Y" && m_lzType->text().mid(2,1) == "L")
        || (m_ycgType->text().mid(2,1) == "L" && m_lzType->text().mid(2,1) == "Y"))))
    {
        qInfo() << cnStr("延长杆与螺柱的类型不匹配！");
        return false;
    }

    if(!(m_modbusTcp->connectModbus(IPAddress, MACAddress)))
    {
        m_ycgType->setEnabled(true);
        m_lzType->setEnabled(true);
        m_write->setEnabled(false);
        m_read->setEnabled(false);
        m_writeRead->setEnabled(false);
        m_pStartRun->setEnabled(false);
        return false;
    }
    m_ycgType->setEnabled(false);
    m_lzType->setEnabled(false);
    m_write->setEnabled(true);
    m_read->setEnabled(true);
    m_writeRead->setEnabled(true);
    m_pStartRun->setEnabled(true);

    m_pTableWrite->setItem(0, 0, new QTableWidgetItem(m_ycgType->text().mid(0,2)));
    m_pTableWrite->setItem(1, 0, new QTableWidgetItem(cnStr("01")));
    return true;
}

void ControlWidget::slotInitialData()
{
    m_writeContent.clear();
    for (int i = 0; i < 6; i++)
    {
        m_writeContent += m_pTableWrite->item(i, 0)->text().trimmed() + " ";
    }
    slotWriteModbusRegisters();
}
void ControlWidget::slotWriteModbusRegisters()
{
    if (!(m_modbusTcp->writeModbusTcpHoldingRegisters(m_writeContent, starAddress, valuesNumber)))
    {
        qInfo() << cnStr("写入数据失败");
        return;
    }
    qInfo() << m_writeContent;
}

void ControlWidget::slotReadModbusRegisters()
{
    if (!(m_modbusTcp->readModbusTcpHoldingRegisters(starAddress, valuesNumber, MACAddress)))
    {
        qInfo() << cnStr("读数据失败");
        return;
    }
    connect(m_modbusTcp, &CModbusTcp::readRegistersFinished, this, &ControlWidget::slotReadModbusData);
}
void ControlWidget::slotReadModbusData(vector<uint8_t> &p_readData)
{
    int pRow = 0;
    for (vector<uint8_t>::iterator it = p_readData.begin(); it != p_readData.end(); it++)
    {
        if(it == p_readData.begin())
        {
            m_readData.remove(starAddress, valuesNumber);
        }
        m_pTableRead->item(pRow, 0)->setText(QString("%1").arg(*it));
        m_readData.append(QString("%1").arg(*it));
        pRow++;
        if(it == p_readData.end() - 1)
        {
            m_pData = m_readData.toInt();
        }
    }
    p_readData.erase(p_readData.begin(), p_readData.end());
    return;
}

bool ControlWidget::slotStartRun()
{
    if (m_timer->isActive() == true)
    {
        m_timer->stop();
        return false;
    }
    m_lzIsTrigger = false;
    m_ycgIsTrigger = false;
    m_writeContent = "00 00 00 00 00 00";
    m_writeContent.replace(0, 2, m_ycgType->text().mid(0,2));
    m_timer->start(1100);
    return true;
}
void ControlWidget::slotChangeWrite()
{
    slotReadModbusRegisters();

    if (((m_pTableRead->item(2, 0)->text().trimmed()).toInt() == 1 && (m_pTableRead->item(3, 0)->text().trimmed()).toInt() == 0) ||
        ((m_pTableRead->item(4, 0)->text().trimmed()).toInt() == 1 && (m_pTableRead->item(5, 0)->text().trimmed()).toInt() == 0))
    {
        m_timer->stop();

        if ((m_pTableRead->item(2, 0)->text().trimmed()).toInt() == 1 && (m_pTableRead->item(3, 0)->text().trimmed()).toInt() == 0)
        {
            if(m_lzIsTrigger == false)
            {
                m_writeContent.replace(6, 2, "01");
                m_lzPosture = mainWindow()->slotImageDiscern(false, false, m_lzDirection);
                if(m_lzPosture == true && m_lzDirection == true)
                {
                    m_writeContent.replace(9, 2, "11");
                }
                else if(m_lzPosture == true && m_lzDirection == false)
                {
                    m_writeContent.replace(9, 2, "12");
                }
                else
                {
                    m_writeContent.replace(9, 2, "20");
                }
                m_lzIsTrigger = true;
            }
        }
        else if((m_pTableRead->item(2, 0)->text().trimmed()).toInt() == 0 || (m_pTableRead->item(3, 0)->text().trimmed()).toInt() != 0)
        {
            m_writeContent.replace(6, 2, "00");
            m_writeContent.replace(9, 2, "00");
            m_lzIsTrigger = false;
        }

        if ((m_pTableRead->item(4, 0)->text().trimmed()).toInt() == 1 && (m_pTableRead->item(5, 0)->text().trimmed()).toInt() == 0)
        {
            if(m_ycgIsTrigger == false)
            {
                m_writeContent.replace(12, 2, "01");
                m_ycgPosture = mainWindow()->slotImageDiscern(false, true, m_ycgDirection);
                if(m_ycgPosture == true && m_ycgDirection == true)
                {
                    m_writeContent.replace(15, 2, "11");
                }
                else if(m_ycgPosture == true && m_ycgDirection == false)
                {
                    m_writeContent.replace(15, 2, "12");
                }
                else
                {
                    m_writeContent.replace(15, 2, "20");
                }
                m_ycgIsTrigger = true;
            }
        }
        else if((m_pTableRead->item(4, 0)->text().trimmed()).toInt() == 0 || (m_pTableRead->item(5, 0)->text().trimmed()).toInt() != 0)
        {
            m_writeContent.replace(12, 2, "00");
            m_writeContent.replace(15, 2, "00");
            m_ycgIsTrigger = false;
        }

        slotWriteModbusRegisters();
        QThread::msleep(1000);
        m_timer->start();
    }
}