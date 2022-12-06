#include "vm.h"
#include "communication/modbusMaster.h"
#include "communication/serialPort/serialPort.h"
#include "action/iAction.h"
#include "para/define/scannerDef.h"
#include "para/define/communicationDef.h"

static CVM *g_Vm = nullptr;
CVM *vm()
{
    return g_Vm;
}

CVM::CVM(QObject *p)
    : QObject(p),
      m_pMaster(nullptr),
      m_pAutoWorkAction(nullptr),
      m_pResetAction(nullptr),
      m_state(vmMax)
{
    g_Vm = this;

    CActionCreater creater(this);
    m_pResetAction = creater.resetAction();
    m_pAutoWorkAction = creater.autoWorkAction();
    connect(m_pResetAction, &IAction::sigEnd, [this]{
        if(m_state == vmReset)
        {
            changeState(vmIdle);
        } });
    connect(m_pAutoWorkAction, &IAction::sigEnd, [this]{
        if(m_state == vmAutoWork)
        {
            changeState(vmIdle);
        } });

    m_pMaster = new CModbusMaster(this);
    connect(m_pMaster,&CModbusMaster::sigDataUpdate,this,&CVM::sigPlcSigUpdate);
    const auto cIP = TIGER_PCDef::pcParas()->serverIp;
    const auto cPort = TIGER_PCDef::pcParas()->serverPort;
    const auto cAddr = TIGER_PCDef::pcParas()->serverAddr;
    myInfo << cnStr("服务器地址:ip:%1,port:%2,设备号:%3").arg(cIP).arg(cPort).arg(cAddr);
    bool successful = m_pMaster->listen(cIP, cPort, cAddr);
    myInfo << (successful ? cnStr("服务器创建成功") : cnStr("服务器创建失败"));
    if (successful)
    {
        reset();
    }

    const auto portName = TIGER_ScannerDef::scannerParas()->scannerPort;
    m_pSerialPort = new CSerialPort(this);
    m_pSerialPort->setPort(portName);
    myInfo << (m_pSerialPort->slotOpenPort() ? cnStr("扫描仪连接成功") : cnStr("扫描仪连接失败"));
    connect(m_pSerialPort,&CSerialPort::sendReadSignal,this,&CVM::setRunType);
}

CVM::~CVM()
{
    delPtr(m_pResetAction);
    delPtr(m_pAutoWorkAction);
}
CVMState CVM::vmState() const
{
    return m_state;
}

bool CVM::sendDisColis(int p_addr, bool p_value)
{
    return m_pMaster->sendDisColis(p_addr, p_value);
}

bool CVM::sendHold(int p_addr, quint16 p_value)
{
    return m_pMaster->sendHold(p_addr, p_value);
}

void CVM::reset()
{
    changeState(vmReset);
    m_pResetAction->start();
}

void CVM::stoReset()
{
    assert(m_state == vmReset);
    m_pResetAction->stop();
}

void CVM::autoWork()
{
    changeState(vmAutoWork);
    m_pAutoWorkAction->start();
}

void CVM::stopWork()
{
    assert(m_state == vmAutoWork);
    m_pAutoWorkAction->stop();
}

void CVM::changeState(CVMState nextState)
{
    m_state = nextState;
    myDebug << cnStr("当前vm状态：") << nextState;
    emit sigVMStateUpdate();
}

void CVM::setRunType(QByteArray p_readType)
{
    emit sigRunType(p_readType);
}
