#include "vm.h"
#include "communication/serialPort/pullStorage.h"
#include "communication/serialPort/pushStorage.h"
#include "para/define/serialPortDef.h"

static CVM *g_Vm = nullptr;
CVM *vm()
{
    return g_Vm;
}

CVM::CVM(QObject *p)
    : QObject(p)
{
    g_Vm = this;

    m_pPushStorage = new CPushStorage(this);
    const auto pushStorageportName = TIGER_SerialPortDef::serialPortParas()->pushStorageScanner;
    myInfo << (m_pPushStorage->slotOpenPort(pushStorageportName) ? cnStr("入库扫描仪连接成功") : cnStr("入库扫描仪连接失败"));
    connect(m_pPushStorage,&CPushStorage::pushStorageSignal,this,&CVM::sigPushStorageType);

    m_pPullStorage = new CPullStorage(this);
    const auto pullStorage = TIGER_SerialPortDef::serialPortParas()->pullStorageScanner;
    myInfo << (m_pPullStorage->slotOpenPort(pullStorage) ? cnStr("出库扫描仪连接成功") : cnStr("出库扫描仪连接失败"));
    connect(m_pPullStorage,&CPullStorage::pullStorageSignal,this,&CVM::sigPullStorageType);
}

CVM::~CVM()
{
}