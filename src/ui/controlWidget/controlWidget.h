#pragma once
#include "ui/mainWindowDef.h"
#include "ui/basic.h"
#include "system/basic.h"
#include "system/tool/halconTool.h"
#include "hal/communication/modbusTcp.h"
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QTableWidget>
#include <vector>
#include <QTimer>
using namespace std;
using namespace TIGER_Communication;

#define IPAddress "192.168.0.1:502"
#define MACAddress 64
#define TableColumnNum 1
#define TableRowNum 6
const QString cTableStyleSheet = "QHeaderView::section{background:transparent; color:black;}";
#define TypeLength 9

class ControlWidget : public QWidget
{
    Q_OBJECT

public:
    ControlWidget();
    virtual ~ControlWidget();

protected:
    void initLayout();
    bool eventFilter(QObject *obj, QEvent *event);

protected slots:
    void slotExposureTimePB(bool p_ycgExposureTime);

    void slotScanYcgQRCode(QString p_ycgType);
    void slotScanLzQRCode(QString p_LzType);
    bool slotConnectModbus();

    void slotInitialData();
    void slotWriteModbusRegisters();
    void slotReadModbusRegisters();
    void slotReadModbusData(vector<uint8_t>& p_readData);

    bool slotStartRun();
    void slotChangeWrite();

protected:
    QPushButton *m_pYcgExposureTimeButton;
    QPushButton *m_pLzExposureTimeButton;
    QPushButton *m_pVideoImageButton;
    double m_pYcgExposureTime;
    double m_pLzExposureTime;

    QLineEdit *m_ycgType;
    QLineEdit *m_lzType;
    QPushButton *m_connectModbus;
    QPushButton *m_write;
    QPushButton *m_read;
    QPushButton *m_writeRead;
    QPushButton *m_pStartRun;

    QTableWidget *m_pTableWrite;
    QTableWidget *m_pTableRead;

    CModbusTcp *m_modbusTcp;
    QTimer *m_timer;
    QString m_writeContent;
    QString m_readData;
    int m_pData;

    bool m_ycgDirection;
    bool m_ycgPosture;
    bool m_ycgIsTrigger;
    bool m_lzDirection;
    bool m_lzPosture;
    bool m_lzIsTrigger;
};