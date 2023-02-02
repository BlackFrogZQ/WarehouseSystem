#include "vm.h"
#include "communication/serialPort/serialPort.h"
#include "communication/serialPort/pushStorage.h"
#include "para/define/serialPortDef.h"

static CVM *g_Vm = nullptr;
CVM *vm()
{
    return g_Vm;
}

CVM::CVM(QObject *p)
    : QObject(p),
      m_pMaster(nullptr)
{
    g_Vm = this;

    m_pPushStorage = new CPushStorage(this);
    const auto pushStorageportName = TIGER_SerialPortDef::serialPortParas()->pushStorageScanner;
    myInfo << (m_pPushStorage->slotOpenPort(pushStorageportName) ? cnStr("入库扫描仪连接成功") : cnStr("入库扫描仪连接失败"));
    connect(m_pPushStorage,&CPushStorage::pushStorageSignal,this,&CVM::sigPushStorageType);

    m_pSerialPort = new CSerialPort(this);
    const auto pullStorage = TIGER_SerialPortDef::serialPortParas()->pullStorageScanner;
    myInfo << (m_pSerialPort->slotOpenPort(pullStorage) ? cnStr("出库扫描仪连接成功") : cnStr("出库扫描仪连接失败"));
    connect(m_pSerialPort,&CSerialPort::sendReadSignal,this,&CVM::sigRunType);
}

CVM::~CVM()
{
}


bool CVM::sendDisColis(int p_addr, bool p_value)
{
    return true;
}

bool CVM::sendHold(int p_addr, quint16 p_value)
{
    return true;
}

void CVM::reset()
{
    // changeState(vmReset);
    // m_pResetAction->start();
}

void CVM::stoReset()
{
    // assert(m_state == vmReset);
    // m_pResetAction->stop();
}

void CVM::autoWork()
{
    // changeState(vmAutoWork);
    // m_pAutoWorkAction->start();
}

void CVM::stopWork()
{
    // assert(m_state == vmAutoWork);
    // m_pAutoWorkAction->stop();
}
