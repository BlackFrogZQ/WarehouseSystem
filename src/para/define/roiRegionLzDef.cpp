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
			for (size_t i = 0; i < cytMax-1; i++)
			{
                m_currentNode->appendNode({pntBool, cAssembleTypeName[i], cAssembleTypeName[i], true}, &(lzRoiParas()->assemblyStates[i]));
			}
        };
    };
    static CLzRoiDefParas g_lzRoiDefParas;
};