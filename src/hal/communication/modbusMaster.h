#pragma once
#include <QObject>
#include <QModbusDataUnit>
class QModbusTcpServer;

class CModbusMaster:public QObject
{
    Q_OBJECT
public:
    CModbusMaster(QObject* p = nullptr);
    ~CModbusMaster();
    bool sendDisColis(int p_addr,bool p_value);
    bool sendHold(int p_addr,quint16 p_value);
    bool readData(QModbusDataUnit::RegisterType p_type,int p_startAddr,QList<quint16>& p_readDatas);
    bool listen(QString p_ip = "127.0.0.1",quint16 p_port = 502,quint8 p_masterAddr = 0xff);

signals:
    void sigDataUpdate();

protected slots:
    void dataChanged(QModbusDataUnit::RegisterType table, int address, int size);

protected:
    bool writeData(QModbusDataUnit::RegisterType p_type,int p_startAddr,QList<quint16> p_readDatas);

protected:
    QModbusTcpServer *m_pServer;
    bool& m_isConnected;
};

