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
            //********得到最靠近接近开关的延长杆
            //得到感兴趣区域
            ReduceDomain(p_image, p_roiRegion, &m_hObject);
            Threshold(m_hObject, &m_hObject, 210, 255);
            Connection(m_hObject, &m_hObject);

            //得到面积最大区域
            HTuple Area, Row, Column;
            HObject selectedRegionsOriginal;
            AreaCenter(m_hObject, &Area, &Row, &Column);
            SelectShape(m_hObject, &m_hObject, "area", "and", Area.TupleMax(), Area.TupleMax());
            ClosingCircle(m_hObject, &selectedRegionsOriginal, 10);

            //去除直振导轨两边的磨损
            HTuple AreaRoi, RowRoi, ColumnRoi;
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

            //********判断方向
            //得到工件两端小面积区域
            SmallestRectangle1(selectedRegionsNear, &rowNear1, &columnNear1, &rowNear2, &columnNear2);

            HObject RectangleBehind, RegionIntersectionBehind;
            HTuple AreaBehind, RowBehind, ColumnBehind;
            GenRectangle1(&RectangleBehind, rowNear1, columnNear1, rowNear1 + 10, columnNear2);
            Intersection(selectedRegionsNear, RectangleBehind, &RegionIntersectionBehind);
            AreaCenter(RegionIntersectionBehind, &AreaBehind, &RowBehind, &ColumnBehind);

            HObject RectangleFront, RegionIntersectionFront;
            HTuple AreaFront, RowFront, ColumnFront;
            GenRectangle1(&RectangleFront, rowNear2 - 10, columnNear1, rowNear2, columnNear2);
            Intersection(selectedRegionsNear, RectangleFront, &RegionIntersectionFront);
            AreaCenter(RegionIntersectionFront, &AreaFront, &RowFront, &ColumnFront);

            //根据前后区域面积判断方向
            if (0 != (int(AreaFront > AreaBehind)))
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