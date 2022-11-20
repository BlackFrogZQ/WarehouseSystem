#pragma once
#include "src/system/sigleClass.h"
#include "src/system/basic.h"
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QString>

class CSerialPort : public QObject
{
    Q_OBJECT

public:
    CSerialPort(QObject* p = nullptr);
    ~CSerialPort();

public slots:
    void setPort(QString p_portName);
    bool slotOpenPort();
    void slotReceiveInfo();

signals:
    void sendReadSignal(QByteArray p_readData);

private:
    QSerialPort* m_serialPort;
    QString m_portName;
};