#include "pullStorage.h"

CPullStorage::CPullStorage(QObject *p): QObject(p)
{
    m_serialPort = new QSerialPort(this);
    connect(m_serialPort, &QSerialPort::errorOccurred, this, [this]
    {
        myDebug << "CPullStorage error:" << m_serialPort->errorString();
    });
    connect(m_serialPort, &QSerialPort::readyRead, this, &CPullStorage::slotReceiveInfo);
}

CPullStorage::~CPullStorage()
{
}

bool CPullStorage::slotOpenPort(const QString &p_portName)
{
    if(m_serialPort->isOpen())
    {
        m_serialPort->clear();
        m_serialPort->close();
        return false;
    }

    m_serialPort->setPortName(p_portName);
    if(!m_serialPort->open(QIODevice::ReadWrite))
    {
        return false;
    }
    m_serialPort->setBaudRate(115200, QSerialPort::AllDirections);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
    return true;
}

void CPullStorage::slotReceiveInfo()
{
    QByteArray readData = m_serialPort->readAll();
    if(!readData.isNull())
    {
        int readType = readData.toInt();
        emit pullStorageSignal(readType);
        readData.clear();
    }
}
