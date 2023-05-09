#pragma once
#include "src/system/sigleClass.h"
#include "src/system/basic.h"
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QString>

class CPullStorage : public QObject
{
    Q_OBJECT

public:
    CPullStorage(QObject* p = nullptr);
    ~CPullStorage();

public slots:
    bool slotOpenPort(const QString &p_portName);
    void slotReceiveInfo();

signals:
    void pullStorageSignal(const int &p_readType);

private:
    QSerialPort* m_serialPort;
};