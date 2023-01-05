#include "serialPortLed.h"
#include "para/define/serialPortDef.h"
#include "system/basic.h"
#include <QTimer>
#include <string>
using namespace std;

SINGLETON_GARBO(CLED);
CLED::CLED():m_isSending(false)
{
    m_pSerialPort = new QSerialPort;
    connect(m_pSerialPort, &QSerialPort::errorOccurred, this, [this]{ myDebug << "modbusMaster error:" << m_pSerialPort->errorString(); },Qt::QueuedConnection);
    connect(m_pSerialPort, &QSerialPort::readyRead, this, &CLED::slotReceive,Qt::QueuedConnection);

    m_pSerialPort->setPortName(TIGER_SerialPortDef::serialPortParas()->indicatorLightPort);
    if(m_pSerialPort->open(QIODevice::ReadWrite))
    {
        m_pSerialPort->setBaudRate(9600, QSerialPort::AllDirections);
        m_pSerialPort->setParity(QSerialPort::NoParity);
        m_pSerialPort->setDataBits(QSerialPort::Data8);
        m_pSerialPort->setStopBits(QSerialPort::OneStop);
        m_pSerialPort->setFlowControl(QSerialPort::NoFlowControl);
    }
}

CLED::~CLED()
{
    if(m_pSerialPort->isOpen())
    {
        setAll(clsOFF,clsOFF,clsOFF,clsOFF);
        while(isSending()){}
        m_pSerialPort->clear();
        m_pSerialPort->close();
    }
}

void CLED::setGreen(COrderState p_state)
{
    setOne(cltGreen,p_state);
}

void CLED::setRed(COrderState p_state)
{
    setOne(cltRed,p_state);
}

void CLED::setYellow(COrderState p_state)
{
    setOne(cltYellow,p_state);
}

void CLED::setBuzzer(COrderState p_state)
{
    setOne(cltBuzzer,p_state);
}

bool CLED::isSending() const
{
    return m_delaySendBuffer.size()>0 || m_isSending;
}

void CLED::setAll(COrderState p_green, COrderState p_yellow, COrderState p_red, COrderState p_buzzer)
{
    m_delaySendBuffer.clear();
    m_delaySendBuffer.append( getOrder(cltGreen,p_green));
    m_delaySendBuffer.append( getOrder(cltYellow,p_yellow));
    m_delaySendBuffer.append( getOrder(cltRed,p_red));
    m_delaySendBuffer.append( getOrder(cltBuzzer,p_buzzer));
    sendOrders(m_delaySendBuffer);
}

QByteArray CLED::getOrder(COrderType type,COrderState p_state)
{
    const int cOrderSize = 8;
    const char cOrder[cltMax][clsMax][cOrderSize] = 
    {
        {
            {0x01,0x05,0x00,0x02,0xff,0x00,0x2D,0xFA},
            {0x01,0x05,0x00,0x02,0x00,0x00,0x6C,0x0A},
            {0x01,0x05,0x00,0x02,0xf2,0x00,0x29,0x6A}
        },
        {
            {0x01,0x05,0x00,0x00,0xff,0x00,0x8C,0x3A},
            {0x01,0x05,0x00,0x00,0x00,0x00,0xCD,0xCA},
            {0x01,0x05,0x00,0x00,0xf2,0x00,0x88,0xAA}
        },
        {
            {0x01,0x05,0x00,0x01,0xff,0x00,0xDD,0xFA},
            {0x01,0x05,0x00,0x01,0x00,0x00,0x9C,0x0A},
            {0x01,0x05,0x00,0x01,0xf2,0x00,0xD9,0x6A}
        },
        {
            {0x01,0x05,0x00,0x03,0xff,0x00,0x7C,0x3A},
            {0x01,0x05,0x00,0x03,0x00,0x00,0x3D,0xCA},
            {0x01,0x05,0x00,0x03,0xf2,0x00,0x78,0xAA}
        }
    };
    QByteArray sendBuffer;
    switch(p_state)
    {
        case clsON:
        sendBuffer = QByteArray(cOrder[type][clsON],cOrderSize);
        break;
        case clsFlashed:
        sendBuffer = QByteArray(cOrder[type][clsFlashed],cOrderSize);
        break;
        case clsOFF:
        default:
        sendBuffer = QByteArray(cOrder[type][clsOFF],cOrderSize);
        break;
    };
    return sendBuffer;
}

void CLED::setOne(COrderType type,COrderState p_state)
{
    if(!m_isSending)
    {
        m_isSending = true;
        sendOrder(getOrder(type,p_state));
    }
}

void CLED::sendOrder(QByteArray p_data)
{
    m_sendBuffer = p_data;
    if(m_pSerialPort->write(m_sendBuffer, m_sendBuffer.size()) == m_sendBuffer.size())
    {
        QTimer::singleShot(500,this,[this]()
            {
                if(m_isSending)
                {
                    sendOrder(m_sendBuffer);
                }
            });
    }
    else
    {
        QTimer::singleShot(200,this,[this]()
        {
            sendOrder(m_sendBuffer);
        });
    }
}

void CLED::sendOrders(QQueue<QByteArray> p_datas)
{
    // assert(m_isSending == false);
    m_delaySendBuffer = p_datas;
    if(m_delaySendBuffer.size()>0 )
    {
        if(!m_isSending)
        {
            m_isSending = true;
            sendOrder(m_delaySendBuffer.dequeue());
        }
        QTimer::singleShot(500,this,[this]()
        {
            sendOrders(m_delaySendBuffer);
        });
    }
}

void CLED::slotReceive()
{
    QByteArray readData = m_pSerialPort->readAll();
    if(m_isSending)
    {
        m_isSending = !(readData == m_sendBuffer);
    }
}

CLED *led()
{
    return CLED::instance();
}