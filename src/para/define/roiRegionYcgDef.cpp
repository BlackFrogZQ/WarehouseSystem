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
            m_currentNode->appendNode({pntInt, "x", cnStr("X轴坐标"), true}, &(ycgRoiParas()->x));
            m_currentNode->appendNode({pntInt, "y", cnStr("Y轴坐标"), true}, &(ycgRoiParas()->y));
            m_currentNode->appendNode({pntInt, "w", cnStr("区域宽"), true}, &(ycgRoiParas()->w));
            m_currentNode->appendNode({pntInt, "h", cnStr("区域高"), true}, &(ycgRoiParas()->h));
            m_currentNode->appendNode({pntBool, "inShow", cnStr("显示区域"), true}, &(ycgRoiParas()->isShow));
        };
    };
    static CYcgRoiDefParas g_ycgRoiDefParas;
};