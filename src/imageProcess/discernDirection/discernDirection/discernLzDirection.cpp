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
            //****1、得到螺柱*****
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

            //****2、得到底端*****
            SmallestRectangle1(m_hObject, &row1, &column1, &row2, &column2);
            GenRectangle1(&tempRegion, row2-100, column1, row2, column2);
            Intersection(m_hObject, tempRegion, &m_hObject);

            //****3、剪裁图片*****
            ReduceDomain(p_image, m_hObject, &m_hObject);
            CropDomain(m_hObject, &m_hObject);

            return getDirection(catLz, m_hObject, m_direction);
        }
        catch (HException &except)
        {
            m_error = except.ErrorMessage().Text();
        }
        return false;
    }
};