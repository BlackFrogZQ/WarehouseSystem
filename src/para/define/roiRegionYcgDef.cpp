#include "para/define/paraDef.h"
#include "para/define/paraNode.h"
#include "roiRegionYcgDef.h"

using namespace TIGER_ParaDef;

namespace TIGER_YcgRoiDef
{
    static CYcgRoiPara g_ycgRoiParas;
    CYcgRoiPara* ycgRoiParas()
    {
        return &g_ycgRoiParas;
    };

    class CYcgRoiDefParas : public CParas
    {
    public:
        CYcgRoiDefParas() : CParas(cnStr("ycgRoiPara"), cnStr("延长杆相机识别区域"), true)
        {
			for (size_t i = 0; i < cytMax-1; i++)
			{
                m_currentNode->appendNode({pntBool, cYcgTypeName[i], cYcgTypeName[i], true}, &(ycgRoiParas()->ycgStates[i]));
			}
        };
    };
    static CYcgRoiDefParas g_ycgRoiDefParas;
};