#include "modbusMaster.h"
#include "plcSigDef.h"
#include <QModbusTcpServer>

CModbusMaster::CModbusMaster(QObject *p) : QObject(p), m_pServer(nullptr), m_isConnected(masterData()->isConnected)
{
    assert(m_isConnected == false);
    m_pServer = new QModbusTcpServer(this);
    QModbusDataUnitMap reg;
    reg.insert(QModbusDataUnit::Coils, {QModbusDataUnit::Coils, 0, 10});
    reg.insert(QModbusDataUnit::DiscreteInputs, {QModbusDataUnit::DiscreteInputs, 0, 10});
    reg.insert(QModbusDataUnit::InputRegisters, {QModbusDataUnit::InputRegisters, 0, 10});
    reg.insert(QModbusDataUnit::HoldingRegisters, {QModbusDataUnit::HoldingRegisters, 0, 10});
    m_pServer->setMap(reg);

    connect(m_pServer, &QModbusTcpServer::dataWritten, this, &CModbusMaster::dataChanged);
    connect(m_pServer, &QModbusTcpServer::stateChanged, this, [this](QModbusDevice::State state)
            { m_isConnected = (m_pServer->state() != QModbusDevice::UnconnectedState); });
    connect(m_pServer, &QModbusTcpServer::errorOccurred, this, [this]
            { myDebug << "modbusMaster error:" << m_pServer->errorString(); });
}

CModbusMaster::~CModbusMaster()
{
    if (m_pServer)
        m_pServer->disconnectDevice();
    delPtr(m_pServer);
}

bool CModbusMaster::sendDisColis(int p_addr, bool p_value)
{
    return writeData(QModbusDataUnit::DiscreteInputs, p_addr, QList<quint16>() << p_value);
}

bool CModbusMaster::sendHold(int p_addr, quint16 p_value)
{
    return writeData(QModbusDataUnit::HoldingRegisters, p_addr, QList<quint16>() << p_value);
}

bool CModbusMaster::readData(QModbusDataUnit::RegisterType p_type, int p_startAddr, QList<quint16> &p_readDatas)
{
    if (!m_isConnected)
    {
        return false;
    }

    for (size_t i = 0; i < p_readDatas.size(); i++)
    {
        if (!m_pServer->data(p_type, p_startAddr + i, &p_readDatas[i]))
        {
            return false;
        }
    }
    return true;
}

bool CModbusMaster::writeData(QModbusDataUnit::RegisterType p_type, int p_startAddr, QList<quint16> p_readDatas)
{
    if (!m_isConnected)
    {
        return false;
    }

    for (size_t i = 0; i < p_readDatas.size(); i++)
    {
        if (!m_pServer->setData(p_type, p_startAddr + i, p_readDatas[i]))
        {
            return false;
        }
    }
    return true;
}

bool CModbusMaster::listen(QString p_ip, quint16 p_port, quint8 p_masterAddr)
{
    if (m_isConnected)
    {
        m_pServer->disconnectDevice();
        m_isConnected = false;
    }
    m_pServer->setConnectionParameter(QModbusDevice::NetworkAddressParameter, p_ip);
    m_pServer->setConnectionParameter(QModbusDevice::NetworkPortParameter, p_port);
    m_pServer->setServerAddress(p_masterAddr);
    return m_pServer->connectDevice();
}

void CModbusMaster::dataChanged(QModbusDataUnit::RegisterType table, int address, int size)
{
    for (size_t i = 0; i < size; i++)
    {
        const int addr = address + i;
        quint16 vaule = 0;
        m_pServer->data(table, addr, &vaule);
        switch (table)
        {
        case QModbusDataUnit::Coils:
            assert(addr >= 0 && addr < cpcMax);
            masterData()->plcColis[addr] = (vaule != 0);
            break;
        case QModbusDataUnit::DiscreteInputs:
            assert(addr >= 0 && addr < cpdcMax);
            masterData()->pcDisColis[addr] = (vaule != 0);
            break;
        case QModbusDataUnit::HoldingRegisters:
            assert(addr >= 0 && addr < cphMax);
            masterData()->pcHold[addr] = vaule;
            break;
        default:
            break;
        }
    }
    emit sigDataUpdate();
}
