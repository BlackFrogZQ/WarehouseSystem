#pragma once
#include "src/system/sigleClass.h"
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QQueue>

class CLED:public QObject
{
    Q_OBJECT
    SINGLETON(CLED)
    public:
    enum COrderState
    {
        clsON,
        clsOFF,
        clsFlashed,
        clsMax
    };
    void setGreen(COrderState p_state);
    void setRed(COrderState p_state);
    void setYellow(COrderState p_state);
    void setBuzzer(COrderState p_state);
    void setAll(COrderState p_green, COrderState p_yellow, COrderState p_red, COrderState p_buzzer);
    bool isSending() const;

protected slots:
    void slotReceive();

protected:
    enum COrderType
    {
        cltGreen,
        cltRed,
        cltYellow,
        cltBuzzer,
        cltMax
    };
    void sendOrder(QByteArray p_data);
    void sendOrders(QQueue<QByteArray> p_datas);
    QByteArray getOrder(COrderType type,COrderState p_state);
    void setOne(COrderType type,COrderState p_state);

protected:
    QSerialPort* m_pSerialPort;
    bool m_isSending;
    QByteArray m_sendBuffer;
    QQueue<QByteArray> m_delaySendBuffer;
};

CLED *led();