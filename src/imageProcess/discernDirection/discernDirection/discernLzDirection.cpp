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
            HObject roiRegion;
            ReadRegion(&roiRegion, "lzRoiRegion.hobj");
            ReduceDomain(p_image, roiRegion, &m_hObject);

            //*****************得到最靠近接近开关的延长杆***************
            Threshold(m_hObject, &m_hObject, 100, 255);
            Connection(m_hObject, &m_hObject);

            //得到中间的区域
            HTuple areaRoi, rowRoi, columnRoi;
            AreaCenter(roiRegion, &areaRoi, &rowRoi, &columnRoi);
            SelectShape(m_hObject, &m_hObject, "column", "and", columnRoi - 30, columnRoi + 30);

            //获得最近的工件
            HTuple areaAll, rowAll, columnAll, numberAll, i, j;
            SelectShape(m_hObject, &m_hObject, "area", "and", 3000, 9999999);
            AreaCenter(m_hObject, &areaAll, &rowAll, &columnAll);
            CountObj(m_hObject, &numberAll);
            {
                HTuple endI = numberAll - 1;
                HTuple stepI = 1;
                for (i = 0; i.Continue(endI, stepI); i += stepI)
                {
                    {
                        HTuple endJ = (numberAll - i) - 2;
                        HTuple stepJ = 1;
                        for (j = 0; j.Continue(endJ, stepJ); j += stepJ)
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
            HObject selectedRegionsOriginal;
            SelectShape(m_hObject, &selectedRegionsOriginal, "row", "and", HTuple(rowAll[numberAll - 1]), p_imageSize.height());

            //对区域进行处理
            HObject selectedRegionsNear;
            OpeningCircle(selectedRegionsOriginal, &selectedRegionsNear, 2);
            Connection(selectedRegionsNear, &selectedRegionsNear);
            AreaCenter(selectedRegionsNear, &areaAll, &rowAll, &columnAll);
            SelectShape(selectedRegionsNear, &selectedRegionsNear, "area", "and", areaAll.TupleMax(), areaAll.TupleMax());

            FillUp(selectedRegionsNear, &selectedRegionsNear);
            ShapeTrans(selectedRegionsNear, &selectedRegionsNear, "convex");

            //*****************判断位姿***************
            // //根据矩形度来判断
            // HTuple rectangularity;
            // Rectangularity(selectedRegionsNear, &rectangularity);
            // if (0 != (int(rectangularity < 0.80)))
            // {
            //     m_error = cnStr("姿势错误");
            //     return false;
            // }

            // //根据内接矩形来判断
            // HObject regionTrans, rectangle;
            // HTuple row1, column1, row2, column2, wide, height;
            // ShapeTrans(selectedRegionsNear, &regionTrans, "convex");
            // InnerRectangle1(regionTrans, &row1, &column1, &row2, &column2);
            // GenRectangle1(&rectangle, row1, column1, row2, column2);
            // wide = column2 - column1;
            // height = row2 - row1;
            // if (0 != (int(height <= wide)))
            // {
            //     Rectangularity(regionTrans, &rectangularity);
            //     if (0 != (int(rectangularity < 0.85)))
            //     {
            //         m_error = cnStr("姿势错误");
            //         return false;
            //     }
            // }

            // //根据外接矩形的角度来判断
            // HTuple row, column, phi, length1, length2, deg;
            // SmallestRectangle2(selectedRegionsNear, &row, &column, &phi, &length1, &length2);
            // GenRectangle2(&rectangle, row, column, phi, length1, length2);
            // TupleDeg(phi, &deg);
            // if (0 != (HTuple(HTuple(HTuple(HTuple(int(deg>10)).TupleAnd(int(deg<80))).TupleOr(HTuple(int(deg>100)).TupleAnd(int(deg<170)))).TupleOr(HTuple(int(deg<-10)).TupleAnd(int(deg>-80)))).TupleOr(HTuple(int(deg<-100)).TupleAnd(int(deg>-170)))))
            // {
            //     m_error = cnStr("姿势错误");
            //     return false;
            // }

            //*****************判断方向***************
            //得到左边区域
            HObject rectangleLeft;
            GenRectangle1(&rectangleLeft, 0, columnRoi - 40, p_imageSize.height(), columnRoi);
            Intersection(rectangleLeft, selectedRegionsNear, &rectangleLeft);

            //得到小面积区域
            HTuple areaMin, rowMin, columnMin;
            HTuple row1, column1, row2, column2;
            SmallestRectangle1(rectangleLeft, &row1, &column1, &row2, &column2);
            GenRectangle1(&rectangleLeft, row2 - 50, column1, row2 + 50, column2);
            Intersection(selectedRegionsNear, rectangleLeft, &rectangleLeft);

            //得到小面积区域两端区域的长度比值
            HTuple rowNear1, columnNear1, rowNear2, columnNear2;
            SmallestRectangle1(rectangleLeft, &rowNear1, &columnNear1, &rowNear2, &columnNear2);

            HObject RectangleBehind, RegionIntersectionBehind;
            HTuple AreaBehind, RowBehind, ColumnBehind, RowBehind1, ColumnBehind1, RowBehind2, ColumnBehind2, LengthBehind;
            GenRectangle1(&RectangleBehind, rowNear1, columnNear1, rowNear1 + 10, columnNear2);
            Intersection(rectangleLeft, RectangleBehind, &RegionIntersectionBehind);
            AreaCenter(RegionIntersectionBehind, &AreaBehind, &RowBehind, &ColumnBehind);
            SmallestRectangle1(RegionIntersectionBehind, &RowBehind1, &ColumnBehind1, &RowBehind2, &ColumnBehind2);
            LengthBehind = ColumnBehind2 - ColumnBehind1;

            HObject RectangleFront, RegionIntersectionFront;
            HTuple AreaFront, RowFront, ColumnFront, RowFront1, ColumnFront1, RowFront2, ColumnFront2, LengthFront;
            GenRectangle1(&RectangleFront, rowNear2 - 10, columnNear1, rowNear2, columnNear2);
            Intersection(rectangleLeft, RectangleFront, &RegionIntersectionFront);
            AreaCenter(RegionIntersectionFront, &AreaFront, &RowFront, &ColumnFront);
            SmallestRectangle1(RegionIntersectionFront, &RowFront1, &ColumnFront1, &RowFront2, &ColumnFront2);
            LengthFront = ColumnFront2 - ColumnFront1;

            HTuple lengthRadio1, lengthRadio2, lengthRadi;
            lengthRadio1 = LengthFront / (LengthBehind.TupleReal());
            lengthRadio2 = LengthBehind / (LengthFront.TupleReal());
            lengthRadi = lengthRadio1;
            if (0 != (int(lengthRadio2 < lengthRadio1)))
            {
                lengthRadi = lengthRadio2;
            }

            //根据长度比值判断方向
            if (0 != (int(lengthRadi > 0.9)))
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