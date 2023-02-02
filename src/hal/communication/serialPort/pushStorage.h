#pragma once
#include "src/system/sigleClass.h"
#include "src/system/basic.h"
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QString>

class CPushStorage : public QObject
{
    Q_OBJECT

public:
    CPushStorage(QObject* p = nullptr);
    ~CPushStorage();

public slots:
    bool slotOpenPort(const QString &p_portName);
    void slotReceiveInfo();

signals:
    void pushStorageSignal(const int &p_readType);

private:
    QSerialPort* m_serialPort;
};