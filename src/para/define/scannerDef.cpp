#include "para/define/paraDef.h"
#include "para/define/paraNode.h"
#include "scannerDef.h"

using namespace TIGER_ParaDef;

namespace TIGER_ScannerDef
{
    static CScannerPara g_scannerParas;
    CScannerPara *scannerParas()
    {
        return &g_scannerParas;
    };
    class CScannerDefPara : public CParas
    {
    public:
        CScannerDefPara() : CParas(cnStr("scannerParas"), cnStr("扫描仪参数"))
        {
            m_currentNode->appendNode({pntString, "scannerPort", cnStr("扫描仪端口"), true, cnStr("设置扫描仪端口")}, &(scannerParas()->scannerPort));
        };
    };
    static CScannerDefPara g_scannerDefPara;
};