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
            m_currentNode->appendNode({pntString, "indicatorLightPort", cnStr("入库扫描仪"), true, cnStr("设置入库扫描仪端口")}, &(serialPortParas()->pushStorageScanner));
            m_currentNode->appendNode({pntString, "scannerPort", cnStr("出库扫描仪"), true, cnStr("设置出库扫描仪端口")}, &(serialPortParas()->pullStorageScanner));
        };
    };
    static CSerialPortDefPara g_scannerDefPara;
};