#include "para/define/paraDef.h"
#include "para/define/paraNode.h"
#include "cameraLzDef.h"

using namespace TIGER_ParaDef;

namespace TIGER_CameraLzDef
{
    static CCameraParaLz g_cameraParaParas;
    CCameraParaLz* cameraParasLz()
    {
        return &g_cameraParaParas;
    };

    class CCameraDefParasLz : public CParas
    {
    public:
        CCameraDefParasLz(): CParas(cnStr("camera"), cnStr("螺柱相机参数"), true)
        {
            m_currentNode->appendNode({pntString, "ip", cnStr("ip"), true}, &(cameraParasLz()->ip));
            m_currentNode->appendNode({pntInt, "exposureTime", cnStr("初始曝光时间"), false}, &(cameraParasLz()->exposureTime));
            m_currentNode->appendNode({pntInt, "heartbeatTime", cnStr("心跳时间"), true}, &(cameraParasLz()->heartbeatTime));
        };
    };
    static CCameraDefParasLz g_cameraParas;
};