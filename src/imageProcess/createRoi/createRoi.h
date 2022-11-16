#pragma once
#include "../basic.h"
#include <QLabel>
namespace TIGER_ProcessTool
{
    class CCreateRoi : public IProcessToolFun
    {
    public:
        bool createRoi(HalconCpp::HTuple &p_handle, bool p_processYcg);
    };
};