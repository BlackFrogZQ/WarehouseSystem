#pragma once
#include "system/basic.h"
#include <HalconCpp.h>
namespace TIGER_HalconTool
{
    const HalconCpp::HTuple cDefHandleValue = -1;

    void setWidgetHandle(HalconCpp::HTuple &p_handle, QWidget *p_widget);

    bool qimgToHimg(const QImage &p_qimg, HalconCpp::HImage &p_himg);

    bool himgToQimg(HalconCpp::HObject p_himg, QImage &p_qimg);
    HalconCpp::HTuple getHColor(uchar r, uchar g = 0, uchar b = 0);
    HalconCpp::HObject getSmallestRectangleRegion(const HalconCpp::HObject &p_region);
    HalconCpp::HObject getSmallestEllipseRegion(const HalconCpp::HObject &p_region);
    HalconCpp::HObject getCrossContours(const HalconCpp::HTuple& p_rows,const HalconCpp::HTuple& p_cols);
};
