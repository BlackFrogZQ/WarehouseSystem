#include "vm.h"
#include "communication/modbusMaster.h"
#include "communication/serialPort/serialPort.h"
#include "action/iAction.h"

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
    const auto cIP = "192.168.0.32";
    const auto cPort = 502;
    const auto cAddr = 255;
    myInfo << cnStr("服务器地址:ip:%1,port:%2,设备号:%3").arg(cIP).arg(cPort).arg(cAddr);
    bool successful = m_pMaster->listen(cIP, cPort, cAddr);
    myInfo << (successful ? cnStr("服务器创建成功") : cnStr("服务器创建失败"));
    if (successful)
    {
        // reset();
    }

    const auto portName = "COM3";
    m_pSerialPort = new CSerialPort(this);
    m_pSerialPort->setPort(portName);
    myInfo << (m_pSerialPort->slotOpenPort() ? cnStr("扫描仪连接成功") : cnStr("扫描仪连接失败"));
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
    m_pSerialPort->slotOpenPort();
}

void CVM::stopWork()
{
    assert(m_state == vmAutoWork);
    m_pAutoWorkAction->stop();
    m_pSerialPort->slotOpenPort();
}

void CVM::changeState(CVMState nextState)
{
    m_state = nextState;
    myDebug << cnStr("当前vm状态：") << nextState;
    emit sigVMStateUpdate();
}
