#pragma once
#include "src/system/sigleClass.h"
#include "src/system/basic.h"
#include <QObject>
#include <QModbusTcpClient>
#include <QModbusDataUnit>
#include <vector>
using namespace std;
class QTcpServer;
namespace TIGER_Communication
{
    class CTcpClient;
    class CModbusTcp : public QObject
    {
        Q_OBJECT
    public:
        CModbusTcp();
        ~CModbusTcp();

    public slots:
        void onStateChanged();
        bool connectModbus(QString p_iPAddressPort, int p_serverAddress);

        //写数据
        bool writeModbusTcpCoils(QString p_writeCoilsData, int p_starAddress, int p_valuesNumber);
        bool writeModbusTcpHoldingRegisters(QString p_writeRegistersData, int p_starAddress, int p_valuesNumber);
        //读数据
        bool readModbusTcpHoldingRegisters(int p_starAddress, quint16 p_valuesNumber, int serverAddress);
        void readReadyHoldingRegisters();

    signals:
        void statechangeOn();
        void statechangeOff();
        void readRegistersFinished(vector<uint8_t>& p_readData);

    protected:
        QModbusTcpClient *m_tcpClient;
    };
};