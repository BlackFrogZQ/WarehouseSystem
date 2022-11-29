#include "para/define/paraDef.h"
#include "para/define/paraNode.h"
#include "roiRegionLzDef.h"

using namespace TIGER_ParaDef;

namespace TIGER_LzRoiDef
{
    static CLzRoiPara g_lzRoiParas;
    CLzRoiPara* lzRoiParas()
    {
        return &g_lzRoiParas;
    };

    class CLzRoiDefParas : public CParas
    {
    public:
        CLzRoiDefParas() : CParas(cnStr("lzRoiPara"), cnStr("螺柱相机识别区域"), true)
        {
            m_currentNode->appendNode({pntInt, "x", cnStr("X轴坐标"), true}, &(lzRoiParas()->x));
            m_currentNode->appendNode({pntInt, "y", cnStr("Y轴坐标"), true}, &(lzRoiParas()->y));
            m_currentNode->appendNode({pntInt, "w", cnStr("区域宽"), true}, &(lzRoiParas()->w));
            m_currentNode->appendNode({pntInt, "h", cnStr("区域高"), true}, &(lzRoiParas()->h));
            m_currentNode->appendNode({pntBool, "inShow", cnStr("显示区域"), true}, &(lzRoiParas()->isShow));
        };
    };
    static CLzRoiDefParas g_lzRoiDefParas;
};