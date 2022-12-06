#include "para/define/paraDef.h"
#include "para/define/paraNode.h"
#include "communicationDef.h"

using namespace TIGER_ParaDef;

namespace TIGER_PCDef
{
    static CPCPara g_pcParas;
    CPCPara *pcParas()
    {
        return &g_pcParas;
    };
    class CPcDefPara : public CParas
    {
    public:
        CPcDefPara() : CParas(cnStr("communicationParas"), cnStr("通讯参数"))
        {
            m_currentNode->appendNode({pntString, "serverIp", cnStr("通讯服务器ip"), true, cnStr("设置通讯服务器ip")}, &(pcParas()->serverIp));
            m_currentNode->appendNode({pntInt, "serverPort", cnStr("通讯服务器端口"), true, cnStr("设置通讯服务器端口")}, &(pcParas()->serverPort));
            m_currentNode->appendNode({pntInt, "serverAddr", cnStr("通讯服务器设备"), true, cnStr("设置通讯服务器设备")}, &(pcParas()->serverAddr));
        };
    };
    static CPcDefPara g_pcDefPara;
};