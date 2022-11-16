#include "modbusTcp.h"
#include <QUrl>
#include <QDebug>

namespace TIGER_Communication
{
    CModbusTcp::CModbusTcp() : m_tcpClient(nullptr)
    {
        m_tcpClient = new QModbusTcpClient();
        connect(m_tcpClient, &QModbusClient::stateChanged, this, &CModbusTcp::onStateChanged);
    }

    CModbusTcp::~CModbusTcp()
    {
    }

    bool CModbusTcp::connectModbus(QString p_iPAddressPort, int p_serverAddress)
    {
        if (!m_tcpClient)
        {
            qInfo() << cnStr("m_tcpClient为空");
            return false;
        }

        if (m_tcpClient->state() != QModbusDevice::ConnectedState)
        {
            const QUrl url = QUrl::fromUserInput(p_iPAddressPort);
            m_tcpClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, url.port());
            m_tcpClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, url.host());

            if (!m_tcpClient->connectDevice())
            {
                qInfo() << cnStr("连接modbus设备失败");
                return false;
            }
            else
            {
                qInfo() << cnStr("成功连接到modbs设备");
                return true;
            }
        }
        else
        {
            m_tcpClient->disconnectDevice();
            qInfo() << cnStr("modbs设备断开连接成功");
            return false;
        }
    }

    void CModbusTcp::onStateChanged()
    {
        if (m_tcpClient->state() == QModbusDevice::ConnectedState)
        {
            emit statechangeOn();
        }
        else
        {
            emit statechangeOff();
        }
    }

    bool CModbusTcp::writeModbusTcpCoils(QString p_writeCoilsData, int p_starAddress, int p_valuesNumber)
    {
        quint16 number = static_cast<quint16>(p_valuesNumber); // C++中的数据类型转换
        QModbusDataUnit writeUnit(QModbusDataUnit::Coils, p_starAddress, number);

        for (uint i1 = 0; i1 < writeUnit.valueCount(); i1++)
        {
            int j1 = 2 * i1;
            QString str = p_writeCoilsData.mid(j1, 1);
            bool ok;
            quint16 hex1 = str.toInt(&ok, 16);
            writeUnit.setValue(i1, hex1);
        }

        if (auto *reply = m_tcpClient->sendWriteRequest(writeUnit, 64))
        {
            if (!reply->isFinished())
            {
                connect(reply, &QModbusReply::finished, this, [this, reply]()
                        {
                        if (reply->error() == QModbusDevice::ProtocolError)
                        {
                            myDebug << cnStr("Write response error: %1 (Mobus exception: 0x%2)").arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16);
                        }
                        else if (reply->error() != QModbusDevice::NoError)
                        {
                            myDebug << cnStr("Write response error: %1 (code: 0x%2)").arg(reply->errorString()).arg(reply->error(), -1, 16);
                        }
                        reply->deleteLater(); });
            }
            else
            {
                reply->deleteLater();
            }
        }
        else
        {
            myDebug << cnStr("Write error: ") + m_tcpClient->errorString();
            return false;
        }
        return true;
    }

    bool CModbusTcp::writeModbusTcpHoldingRegisters(QString p_writeRegistersData, int p_starAddress, int p_valuesNumber)
    {
        QByteArray str2 = QByteArray::fromHex(p_writeRegistersData.toLatin1().data());
        QString str3 = str2.toHex().data();

        quint16 number = static_cast<quint16>(p_valuesNumber);
        QModbusDataUnit writeUnit(QModbusDataUnit::HoldingRegisters, p_starAddress, number);
        int j1 = 0;
        for (uint i1 = 0; i1 < writeUnit.valueCount(); i1++)
        {
            if (i1 == 0)
            {
                j1 = static_cast<int>(2 * i1);
            }
            else
            {
                j1 = j1 + 3;
            }

            QString stt = p_writeRegistersData.mid(j1, 2);
            bool ok;
            quint16 hex1 = static_cast<quint16>(stt.toInt(&ok, 16));
            writeUnit.setValue(static_cast<int>(i1), hex1);
        }

        if (auto *reply = m_tcpClient->sendWriteRequest(writeUnit, 64))
        {
            if (!reply->isFinished())
            {
                connect(reply, &QModbusReply::finished, this, [this, reply]()
                        {
                        if (reply->error() == QModbusDevice::ProtocolError) 
                        {
                            myDebug << tr("Write response error: %1 (Mobus exception: 0x%2)").arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16);
                        }
                        else if (reply->error() != QModbusDevice::NoError) 
                        {
                            myDebug << tr("Write response error: %1 (code: 0x%2)").arg(reply->errorString()).arg(reply->error(), -1, 16);
                        }
                        reply->deleteLater(); });
            }
            else
            {
                reply->deleteLater();
            }
        }
        else
        {
            myDebug << (tr("Write error: ") + m_tcpClient->errorString(), 5000);
            return false;
        }
        return true;
    }

    bool CModbusTcp::readModbusTcpHoldingRegisters(int p_starAddress, quint16 p_valuesNumber, int serverAddress)
    {
        if (!m_tcpClient->state() == QModbusDevice::ConnectedState)
        {
            return false;
        }

        QModbusDataUnit ReadUnit(QModbusDataUnit::HoldingRegisters, p_starAddress, p_valuesNumber);

        if (auto *reply = m_tcpClient->sendReadRequest(ReadUnit, serverAddress))
        {
            if (!reply->isFinished())
            {
                QObject::connect(reply, &QModbusReply::finished, this, &CModbusTcp::readReadyHoldingRegisters);
                return true;
            }
            else
            {
                delete reply;
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    void CModbusTcp::readReadyHoldingRegisters()
    {
        QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
        if (!reply)
        {
            return;
        }

        vector<uint8_t> pReadData;
        pReadData.erase(pReadData.begin(), pReadData.end());
        if (reply->error() == QModbusDevice::NoError)
        {
            const QModbusDataUnit unit = reply->result();
            for (uint16_t i = 0; i < unit.valueCount();)
            {
                uint16_t res = unit.value(i);
                pReadData.push_back(static_cast<uint8_t>(res));
                i++;
            }
        }
        else
        {
            return;
        }

        reply->deleteLater();
        emit readRegistersFinished(pReadData);
    }
};