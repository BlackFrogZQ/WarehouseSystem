#include "discernYcgDirection.h"
#include "system/tool/halconTool.h"

namespace TIGER_ProcessTool
{
    using namespace HalconCpp;
    bool CYcgDiscern::processUnionRegion(const HalconCpp::HObject &p_image, const HalconCpp::HObject &p_roiRegion, QSize p_imageSize)
    {
        m_error.clear();
        try
        {
            //****1、得到延长杆*****
            ReduceDomain(p_image, p_roiRegion, &m_hObject);
            Threshold(m_hObject, &m_hObject, 230, 255);
            Connection(m_hObject, &m_hObject);

            //去除多余区域
            SelectShape(m_hObject, &m_hObject, "area", "and", 1000, 99999999);
            ClosingCircle(m_hObject, &m_hObject, 10);
            FillUp(m_hObject, &m_hObject);
            Connection(m_hObject, &m_hObject);

            //获得最近的工件
            HTuple areaAll, rowAll, columnAll, numberAll, i, j;
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
            SelectShape(m_hObject, &selectedRegionsOriginal, "row", "and", HTuple(rowAll[numberAll - 1]), p_imageSize.height());

            //去除直振导轨两边的磨损
            HTuple AreaRoi, RowRoi, ColumnRoi;
            HTuple Area, Row, Column;
            AreaCenter(p_roiRegion, &AreaRoi, &RowRoi, &ColumnRoi);
            GenRectangle1(&m_hObject, 0, ColumnRoi, p_imageSize.height(), ColumnRoi);
            Intersection(selectedRegionsOriginal, m_hObject, &m_hObject);
            Connection(m_hObject, &m_hObject);
            AreaCenter(m_hObject, &Area, &Row, &Column);
            SelectShape(m_hObject, &m_hObject, "area", "and", Area.TupleMax(), Area.TupleMax());

            HTuple rowOriginal1, columnOriginal1, rowOriginal2, columnOriginal2, rowNear1, columnNear1, rowNear2, columnNear2;
            HObject selectedRegionsNear;
            SmallestRectangle1(selectedRegionsOriginal, &rowNear1, &columnNear1, &rowNear2, &columnNear2);
            SmallestRectangle1(m_hObject, &rowOriginal1, &columnOriginal1, &rowOriginal2, &columnOriginal2);
            GenRectangle1(&m_hObject, rowOriginal1, columnNear1, rowOriginal2, columnNear2);
            Intersection(selectedRegionsOriginal, m_hObject, &selectedRegionsNear);

            //****2、得到底端*****
            SmallestRectangle1(selectedRegionsNear, &rowNear1, &columnNear1, &rowNear2, &columnNear2);
            GenRectangle1(&m_hObject, rowNear2-100, columnNear1, rowNear2, columnNear2);
            Intersection(selectedRegionsNear, m_hObject, &m_hObject);

            //****3、剪裁图片*****
            ReduceDomain(p_image, m_hObject, &m_hObject);
            CropDomain(m_hObject, &m_hObject);

            return getDirection(catYcg, m_hObject, m_direction);
        }
        catch (HException &except)
        {
            m_error = except.ErrorMessage().Text();
        }
        return false;
    }
};