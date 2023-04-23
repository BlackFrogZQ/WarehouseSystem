#include "discernYcgDirection.h"
#include "system/tool/halconTool.h"
#include "ui/controlWidget/controlWidget.h"
#include "hal/communication/serialPort/typeDef.h"

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
            Threshold(m_hObject, &m_hObject, 210, 255);
            Connection(m_hObject, &m_hObject);

            //去除多余区域
            OpeningCircle(m_hObject, &m_hObject, 2);
            ClosingCircle(m_hObject, &m_hObject, 10);
            FillUp(m_hObject, &m_hObject);
            Connection(m_hObject, &m_hObject);

            //获得最近的工件
            SelectShape(m_hObject, &m_hObject, "area", "and", 1000, 99999999);
            HTuple areaAll, rowAll, columnAll, numberAll;
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
            SelectShape(m_hObject, &m_hObject, "row", "and", Row.TupleMax(), Row.TupleMax());

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

            //****3、剪裁图片判断方向*****
            ReduceDomain(p_image, m_hObject, &m_hObject);
            CropDomain(m_hObject, &m_hObject);
            if(!(getDirection(catYcg, m_hObject, m_direction)))
            {
                return false;
            }

            //****4、根据长度判断姿势*****
            HTuple rowOpen1, columnOpen1, rowOpen2, columnOpen2;
            OpeningRectangle1(selectedRegionsNear, &m_hObject, columnNear2-columnNear1-5, 10);
            SmallestRectangle1(m_hObject, &rowOpen1, &columnOpen1, &rowOpen2, &columnOpen2);
            double pLong;
            if(m_direction)
            {
                HObject RectangleBehind;
                HTuple RowBehind1, ColumnBehind1, RowBehind2, ColumnBehind2;
                GenRectangle1(&RectangleBehind, rowNear1, columnNear1, rowNear1 + 10, columnNear2);
                Intersection(selectedRegionsNear, RectangleBehind, &RectangleBehind);
                SmallestRectangle1(RectangleBehind, &RowBehind1, &ColumnBehind1, &RowBehind2, &ColumnBehind2);
                pLong = (rowOpen2-RowBehind1).D();
            }
            else
            {
                HObject RectangleFront;
                HTuple RowFront1, ColumnFront1, RowFront2, ColumnFront2;
                GenRectangle1(&RectangleFront, rowNear2 - 10, columnNear1, rowNear2, columnNear2);
                Intersection(selectedRegionsNear, RectangleFront, &RectangleFront);
                SmallestRectangle1(RectangleFront, &RowFront1, &ColumnFront1, &RowFront2, &ColumnFront2);
                pLong = (RowFront2-rowOpen1).D();
            }

            if((pLong<cYcgLong[controlPara()->runType()-1][0])||(pLong>cYcgLong[controlPara()->runType()-1][1]))
            {
                return false;
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