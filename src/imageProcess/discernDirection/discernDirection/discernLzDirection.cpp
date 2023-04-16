#include "discernLzDirection.h"
#include "system/tool/halconTool.h"

namespace TIGER_ProcessTool
{
    using namespace HalconCpp;
    bool CLzDiscern::processUnionRegion(const HalconCpp::HObject &p_image, const HalconCpp::HObject &p_roiRegion, QSize p_imageSize)
    {
        m_error.clear();
        try
        {
            //*****************得到最靠近接近传感器的螺柱***************
            ReduceDomain(p_image, p_roiRegion, &m_hObject);
            Threshold(m_hObject, &m_hObject, 200, 255);
            Connection(m_hObject, &m_hObject);

            //获得最近的工件
            HTuple areaAll, rowAll, columnAll, numberAll, i, j;
            SelectShape(m_hObject, &m_hObject, "area", "and", 3000, 9999999);
            AreaCenter(m_hObject, &areaAll, &rowAll, &columnAll);
            CountObj(m_hObject, &numberAll);
            for (int i=0; i<numberAll; i++)
            {
                for (int j=0; j<((numberAll-i)-1); j++)
                {
                    if (rowAll[j]>rowAll[j + 1])
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
            HObject selectedRegionsOriginal;
            SelectShape(m_hObject, &selectedRegionsOriginal, "row", "and", HTuple(rowAll[numberAll-1]), p_imageSize.height());

            //得到半边区域
            HObject tempRegion;
            HTuple row1, column1, row2, column2;
            HTuple areaRoi, rowRoi, columnRoi;
            SmallestRectangle1(selectedRegionsOriginal, &row1, &column1, &row2, &column2);
            AreaCenter(p_roiRegion, &areaRoi, &rowRoi, &columnRoi);
            GenRectangle1(&tempRegion, 0, column1, p_imageSize.height(), columnRoi);
            Intersection(m_hObject, tempRegion, &m_hObject);

            //对区域进行处理
            OpeningCircle(m_hObject, &m_hObject, 2);
            Connection(m_hObject, &m_hObject);
            AreaCenter(m_hObject, &areaAll, &rowAll, &columnAll);
            SelectShape(m_hObject, &m_hObject, "area", "and", areaAll.TupleMax(), areaAll.TupleMax());

            FillUp(m_hObject, &m_hObject);
            ShapeTrans(m_hObject, &m_hObject, "convex");

            //*****************判断位姿***************
            //判断selectedRegionsOriginal是否在中间
            SelectShape(selectedRegionsOriginal, &tempRegion, "column", "and", columnRoi - 30, columnRoi + 30);
            CountObj(tempRegion, &numberAll);
            if(numberAll.I()==0)
            {
                m_error = cnStr("姿势错误");
                return false;
            }

            //根据矩形度来判断
            HTuple rectangularity, circularity;
            Rectangularity(m_hObject, &rectangularity);
            Circularity(m_hObject, &circularity);
            if ((rectangularity.D()<0.81) || (circularity>0.63))
            {
                m_error = cnStr("姿势错误");
                return false;
            }

            //*****************判断方向***************
            //得到小面积区域
            SmallestRectangle1(m_hObject, &row1, &column1, &row2, &column2);
            GenRectangle1(&tempRegion, row1+(row2-row1)/2, column1, row2, column2);
            Intersection(m_hObject, tempRegion, &m_hObject);

            //得到小面积区域两端区域的长度比值
            SmallestRectangle1(m_hObject, &row1, &column1, &row2, &column2);

            HObject RectangleBehind;
            HTuple RowBehind1, ColumnBehind1, RowBehind2, ColumnBehind2, LengthBehind;
            GenRectangle1(&RectangleBehind, row1, column1, row1+3, column2);
            Intersection(m_hObject, RectangleBehind, &RectangleBehind);
            SmallestRectangle1(RectangleBehind, &RowBehind1, &ColumnBehind1, &RowBehind2, &ColumnBehind2);
            LengthBehind = ColumnBehind2 - ColumnBehind1;

            HObject RectangleFront;
            HTuple RowFront1, ColumnFront1, RowFront2, ColumnFront2, LengthFront;
            GenRectangle1(&RectangleFront, row2-3, column1, row2, column2);
            Intersection(m_hObject, RectangleFront, &RectangleFront);
            SmallestRectangle1(RectangleFront, &RowFront1, &ColumnFront1, &RowFront2, &ColumnFront2);
            LengthFront = ColumnFront2 - ColumnFront1;

            HTuple lengthRadio1, lengthRadio2, lengthRadi;
            lengthRadio1 = LengthFront / (LengthBehind.TupleReal());
            lengthRadio2 = LengthBehind / (LengthFront.TupleReal());
            lengthRadi = lengthRadio1;
            if (lengthRadio2 < lengthRadio1)
            {
                lengthRadi = lengthRadio2;
            }

            //根据长度比值判断方向
            if (lengthRadi.D()>0.8)
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