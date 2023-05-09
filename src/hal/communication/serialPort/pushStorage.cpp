#include "pushStorage.h"

CPushStorage::CPushStorage(QObject *p): QObject(p)
{
    m_serialPort = new QSerialPort(this);
    connect(m_serialPort, &QSerialPort::errorOccurred, this, [this]
    {
        myDebug << "CPushStorage error:" << m_serialPort->errorString();
    });
    connect(m_serialPort, &QSerialPort::readyRead, this, &CPushStorage::slotReceiveInfo);
}

CPushStorage::~CPushStorage()
{
}

bool CPushStorage::slotOpenPort(const QString &p_portName)
{
    if(m_serialPort->isOpen())
    {
        m_serialPort->clear();
        m_serialPort->close();
        return false;
    }

    m_serialPort->setPortName(p_portName);
    if(!m_serialPort->open(QIODevice::ReadOnly))
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

void CPushStorage::slotReceiveInfo()
{
    QByteArray readData = m_serialPort->readAll();
    if(!readData.isNull())
    {
        int readType = readData.toInt();
        emit pushStorageSignal(readType);
        readData.clear();
    }
}
