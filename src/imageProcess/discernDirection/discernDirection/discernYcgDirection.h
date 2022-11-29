#pragma once
#include "../iDiscernDirection.h"
namespace TIGER_ProcessTool
{
    class CYcgDiscern : public IDiscernDirection
    {
    public:
        using IDiscernDirection::IDiscernDirection;
        virtual bool processUnionRegion(const HalconCpp::HObject &p_image, const HalconCpp::HObject &p_roiRegion, QSize p_imageSize);
    };
};