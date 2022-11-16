#include "discernLzDirection.h"
#include "system/tool/halconTool.h"

namespace TIGER_ProcessTool
{
    using namespace HalconCpp;
    bool CLzDiscern::processUnionRegion(const HalconCpp::HObject &p_image, QSize p_imageSize)
    {
        m_error.clear();
        try
        {
            HObject roiRegion, imageReduced;
            HTuple areaRoi, rowRoi, columnRoi;
            ReadRegion(&roiRegion, "lzRoiRegion.hobj");
            ReduceDomain(p_image, roiRegion, &imageReduced);
            AreaCenter(roiRegion, &areaRoi, &rowRoi, &columnRoi);

            //*****************得到最靠近接近开关的延长杆***************
            HObject region, connectedRegions, selectedRegionsCentre;
            Threshold(imageReduced, &region, 200, 255);
            Connection(region, &connectedRegions);
            SelectShape(connectedRegions, &selectedRegionsCentre, "column", "and", columnRoi - 20, columnRoi + 20);

            //获得最近的工件
            HObject selectedRegionsNear;
            HTuple areaAll, rowAll, columnAll, numberAll, i, j;
            SelectShape(selectedRegionsCentre, &selectedRegionsNear, "area", "and", 3000, 9999999);
            AreaCenter(selectedRegionsNear, &areaAll, &rowAll, &columnAll);
            CountObj(selectedRegionsNear, &numberAll);
            {
                HTuple end_val50 = numberAll - 1;
                HTuple step_val50 = 1;
                for (i = 0; i.Continue(end_val50, step_val50); i += step_val50)
                {
                    {
                        HTuple end_val51 = (numberAll - i) - 2;
                        HTuple step_val51 = 1;
                        for (j = 0; j.Continue(end_val51, step_val51); j += step_val51)
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
            }
            SelectShape(selectedRegionsNear, &selectedRegionsNear, "row", "and", HTuple(rowAll[numberAll - 1]), p_imageSize.height());

            //*****************判断位姿***************
            //前期处理
            HObject regionOpening;
            OpeningCircle(selectedRegionsNear, &regionOpening, 3.5);

            //根据矩形度来判断
            HTuple rectangularity;
            Rectangularity(regionOpening, &rectangularity);
            if (0 != (int(rectangularity < 0.80)))
            {
                m_error = cnStr("姿势错误");
                return false;
            }

            //根据内接矩形来判断
            HObject regionTrans, rectangle;
            HTuple row1, column1, row2, column2, wide, height;
            ShapeTrans(regionOpening, &regionTrans, "convex");
            InnerRectangle1(regionTrans, &row1, &column1, &row2, &column2);
            GenRectangle1(&rectangle, row1, column1, row2, column2);
            wide = column2 - column1;
            height = row2 - row1;
            if (0 != (int(height <= wide)))
            {
                Rectangularity(regionTrans, &rectangularity);
                if (0 != (int(rectangularity < 0.85)))
                {
                    m_error = cnStr("姿势错误");
                    return false;
                }
            }

            //根据外接矩形的角度来判断
            HTuple row, column, phi, length1, length2, deg;
            SmallestRectangle2(regionTrans, &row, &column, &phi, &length1, &length2);
            GenRectangle2(&rectangle, row, column, phi, length1, length2);
            TupleDeg(phi, &deg);
            if (0 != (HTuple(HTuple(HTuple(HTuple(int(deg>10)).TupleAnd(int(deg<80))).TupleOr(HTuple(int(deg>100)).TupleAnd(int(deg<170)))).TupleOr(HTuple(int(deg<-10)).TupleAnd(int(deg>-80)))).TupleOr(HTuple(int(deg<-100)).TupleAnd(int(deg>-170)))))
            {
                m_error = cnStr("姿势错误");
                return false;
            }

            //*****************判断方向***************
            //大面积区域
            HTuple areaMax, rowMax, columnMax;
            AreaCenter(regionOpening, &areaMax, &rowMax, &columnMax);

            //得到小面积区域
            HTuple areaMin, rowMin, columnMin;
            SmallestRectangle1(regionOpening, &row1, &column1, &row2, &column2);
            GenRectangle1(&regionTrans, row2 - 50, column1, row2 + 50, column2);
            Intersection(regionOpening, regionTrans, &regionTrans);
            Connection(regionTrans, &regionTrans);
            SelectShape(regionTrans, &regionTrans, "area", "and", 50, 99999);
            Union1(regionTrans, &regionTrans);
            ShapeTrans(regionTrans, &regionTrans, "convex");
            AreaCenter(regionTrans, &areaMin, &rowMin, &columnMin);

            //得到方向
            Rectangularity(regionTrans, &rectangularity);
            qDebug()<< rectangularity.D();
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