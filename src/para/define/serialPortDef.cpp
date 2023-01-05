#include "para/define/paraDef.h"
#include "para/define/paraNode.h"
#include "serialPortDef.h"

using namespace TIGER_ParaDef;

namespace TIGER_SerialPortDef
{
    static CSerialPortPara g_serialPortParas;
    CSerialPortPara *serialPortParas()
    {
        return &g_serialPortParas;
    };
    class CSerialPortDefPara : public CParas
    {
    public:
        CSerialPortDefPara() : CParas(cnStr("serialPortParas"), cnStr("串口参数"))
        {
            m_currentNode->appendNode({pntString, "scannerPort", cnStr("扫描仪端口"), true, cnStr("设置扫描仪端口")}, &(serialPortParas()->scannerPort));
            m_currentNode->appendNode({pntString, "indicatorLightPort", cnStr("指示灯端口"), true, cnStr("设置指示灯端口")}, &(serialPortParas()->indicatorLightPort));
        };
    };
    static CSerialPortDefPara g_scannerDefPara;
};