﻿#include "para/define/paraDef.h"
#include "para/define/paraNode.h"
#include "cameraDef.h"

using namespace TIGER_ParaDef;

namespace TIGER_CameraDef
{
    static CCameraPara g_cameraParaParas;
    CCameraPara* cameraParas()
    {
        return &g_cameraParaParas;
    };

    class CCameraDefParas : public CParas
    {
    public:
        CCameraDefParas() : CParas(cnStr("cameraPara"), cnStr("延长杆相机参数"), true)
        {
            m_currentNode->appendNode({pntString, "ip", cnStr("ip"), true}, &(cameraParas()->ip));
            m_currentNode->appendNode({pntInt, "exposureTime", cnStr("初始曝光时间"), false}, &(cameraParas()->exposureTime));
            m_currentNode->appendNode({pntInt, "heartbeatTime", cnStr("心跳时间"), true}, &(cameraParas()->heartbeatTime));
        };
    };
    static CCameraDefParas g_cameraParas;
};