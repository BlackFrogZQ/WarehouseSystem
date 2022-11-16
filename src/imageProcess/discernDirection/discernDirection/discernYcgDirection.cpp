#include "discernYcgDirection.h"
#include "system/tool/halconTool.h"

namespace TIGER_ProcessTool
{
    using namespace HalconCpp;
    bool CYcgDiscern::processUnionRegion(const HalconCpp::HObject &p_image, QSize p_imageSize)
    {
        m_error.clear();
        try
        {
            //********调整曝光及读取ROI
            HObject roiRegion, imageReduced;
            HTuple areaRoi, rowRoi, columnRoi;
            ReadRegion(&roiRegion, "ycgRoiRegion.hobj");
            ReduceDomain(p_image, roiRegion, &imageReduced);
            AreaCenter(roiRegion, &areaRoi, &rowRoi, &columnRoi);

            //********得到最靠近接近开关的延长杆
            HObject region, connectedRegions, selectedRegionsCentre, selectedRegions, regionClosing, rgionFillUp, connectedRegionsAll, selectedRegionsNear;
            HTuple areaAll, rowAll, columnAll, numberAll, i, j;
            Threshold(imageReduced, &region, 200, 255);
            Connection(region, &connectedRegions);
            //得到中间的区域
            SelectShape(connectedRegions, &selectedRegionsCentre, "column", "and", columnRoi - 20, columnRoi + 20);
            //去除多余区域
            SelectShape(selectedRegionsCentre, &selectedRegions, "area", "and", 10000, 99999999);
            ClosingCircle(selectedRegions, &regionClosing, 10);
            FillUp(regionClosing, &rgionFillUp);
            Connection(rgionFillUp, &connectedRegionsAll);
            //获得最近的工件
            AreaCenter(connectedRegionsAll, &areaAll, &rowAll, &columnAll);
            CountObj(connectedRegionsAll, &numberAll);
            {
                HTuple end_val43 = numberAll - 1;
                HTuple step_val43 = 1;
                for (i = 0; i.Continue(end_val43, step_val43); i += step_val43)
                {
                    HTuple end_val47 = (numberAll - i) - 2;
                    HTuple step_val47 = 1;
                    for (j = 0; j.Continue(end_val47, step_val47); j += step_val47)
                    {
                        if (0 != (int(HTuple(rowAll[j]) > HTuple(rowAll[j + 1]))))
                        {
                            HTuple ROW, COL;
                            ROW = HTuple(rowAll[j]);
                            rowAll[j] = HTuple(rowAll[j + 1]);
                            rowAll[j + 1] = ROW;

                            COL = HTuple(columnAll[j]);
                            columnAll[j] = HTuple(columnAll[j + 1]);
                            columnAll[j + 1] = COL;
                        }
                    }
                }
            }
            SelectShape(connectedRegionsAll, &selectedRegionsNear, "row", "and", HTuple(rowAll[numberAll - 1]), p_imageSize.height());
            //********判断是否螺纹套螺纹
            //根据外接矩形的长度判断
            HTuple rowNear1, columnNear1, rowNear2, columnNear2, length, ratio;
            HObject rectangle;
            SmallestRectangle1(selectedRegionsNear, &rowNear1, &columnNear1, &rowNear2, &columnNear2);
            GenRectangle1(&rectangle, rowNear1, columnNear1, rowNear2, columnNear2);

            //********判断方向
            HTuple areaMax, rowMax, columnMax;
            AreaCenter(selectedRegionsNear, &areaMax, &rowMax, &columnMax);
            //得到小面积区域
            HObject regionTrans;
            HTuple areaMin, rowMin, columnMin;
            GenRectangle1(&rectangle, rowNear2 - 100, columnNear1, rowNear2, columnNear2);
            ReduceDomain(selectedRegionsNear, rectangle, &imageReduced);
            ShapeTrans(imageReduced, &regionTrans, "convex");
            AreaCenter(regionTrans, &areaMin, &rowMin, &columnMin);
            //得到方向
            HTuple rectangularity;
            Rectangularity(regionTrans, &rectangularity);
            if (0 != (int(rectangularity > 0.93)))
            {
                m_direction = false;
            }
            else
            {
                m_direction = true;
            }
            return true;
        }
        catch (HException &except)
        {
            m_error = except.ErrorMessage().Text();
        }
        return false;
    }
};