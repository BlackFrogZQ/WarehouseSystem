#include "halconTool.h"
#include <QMessageBox>
namespace TIGER_HalconTool
{
    void setWidgetHandle(HalconCpp::HTuple &p_handle, QWidget *p_widget)
    {
        assert(p_widget != nullptr);
        try
        {
            if (p_handle == cDefHandleValue)
            {
                OpenWindow(0, 0, p_widget->width(), p_widget->height(), HalconCpp::HTuple(long(p_widget->winId())), "visible", "", &p_handle);
                return;
            }
            HalconCpp::HDevWindowStack::Push(p_handle);
            if (HalconCpp::HDevWindowStack::IsOpen())
            {
                CloseWindow(p_handle);
                p_handle = cDefHandleValue;
            }
            HalconCpp::HDevWindowStack::Pop();
            OpenWindow(0, 0, p_widget->width(), p_widget->height(), HalconCpp::HTuple(long(p_widget->winId())), "visible", "", &p_handle);
        }
        catch (HalconCpp::HException &except)
        {
            QMessageBox::warning(nullptr, "setWidgetHandle error", except.ErrorMessage().Text());
        }
    }

    bool qimgToHimg(const QImage &p_qimg, HalconCpp::HImage &p_himg)
    {
        try
        {
            if (p_qimg.isNull())
            {
                myDebug << cnStr("图像为空无法转换");
                return false;
            }

            QImage temp = p_qimg.format() == QImage::Format_RGB888 ? p_qimg : p_qimg.convertToFormat(QImage::Format_RGB888);

            int width = temp.width(), height = temp.height();
            uchar *ptR = new uchar[width * height];
            uchar *r = ptR;
            uchar *ptG = new uchar[width * height];
            uchar *g = ptG;
            uchar *ptB = new uchar[width * height];
            uchar *b = ptB;

            for (int i = 0; i < height; i++)
            {
                uchar *img = (uchar *)temp.constScanLine(i);
                for (int j = 0; j < width; j++)
                {
                    *r++ = *img++;
                    *g++ = *img++;
                    *b++ = *img++;
                }
            }
            p_himg.GenImage3("byte", width, height, ptR, ptG, ptB);
            delete[] ptR;
            delete[] ptG;
            delete[] ptB;
            return true;
        }
        catch (HalconCpp::HException &except)
        {
            myDebug << ("qimgToHimg error:" + QString(except.ErrorMessage().Text()));
        }
        return false;
    }

    bool himgToQimg(HalconCpp::HObject p_himg, QImage &p_qimg)
    {
        try
        {
            ConvertImageType(p_himg, &p_himg, "byte");
            HalconCpp::HTuple hChannels;
            CountChannels(p_himg, &hChannels);
            if (hChannels.I() == 1)
            {
                Decompose3(p_himg, &p_himg, &p_himg, &p_himg);
                CountChannels(p_himg, &hChannels);
            }
            assert(hChannels.I() == 3);
            HalconCpp::HTuple hv_Rptr, hv_Gptr, hv_Bptr, hv_type, hv_width, hv_height;
            GetImagePointer3(p_himg, &hv_Rptr, &hv_Gptr, &hv_Bptr, &hv_type, &hv_width, &hv_height);
            int width = hv_width.I();
            int height = hv_height.I();
            QImage imgBuff(width, height, QImage::Format_RGB888);
            uchar *rPtr = (uchar *)hv_Rptr.L();
            uchar *gPtr = (uchar *)hv_Gptr.L();
            uchar *bPtr = (uchar *)hv_Bptr.L();
            for (int i = 0; i < height; i++)
            {
                uchar *img = imgBuff.scanLine(i);
                for (int j = 0; j < width; j++)
                {
                    *img++ = *rPtr++;
                    *img++ = *gPtr++;
                    *img++ = *bPtr++;
                }
            }
            if (imgBuff.isNull())
            {
                myDebug << cnStr("图像为空无法转换");
                return false;
            }
            p_qimg = imgBuff;
            return true;
        }
        catch (HalconCpp::HException &except)
        {
            myDebug << ("himgToQimg error:" + QString(except.ErrorMessage().Text()));
        }
        return false;
    }

    HalconCpp::HTuple getHColor(uchar r, uchar g, uchar b)
    {
        HalconCpp::HTuple color;
        color[0] = r;
        color[1] = g;
        color[2] = b;
        return color;
    }

    HalconCpp::HObject getSmallestRectangleRegion(const HalconCpp::HObject &p_region)
    {
        HalconCpp::HObject rectangleRegion;
        if (p_region.IsInitialized())
        {
            HalconCpp::Union1(p_region, &rectangleRegion);
            HalconCpp::HTuple row, h, col, w, angle;
            HalconCpp::SmallestRectangle2(rectangleRegion, &row, &col, &angle, &h, &w);
            HalconCpp::GenRectangle2(&rectangleRegion, row, col, angle, h, w);
        }
        return rectangleRegion;
    }

    HalconCpp::HObject getSmallestEllipseRegion(const HalconCpp::HObject &p_region)
    {
        const HalconCpp::HTuple cMinR = 5;
        HalconCpp::HObject ellipseRegion;
        if (p_region.IsInitialized())
        {
            HalconCpp::Union1(p_region, &ellipseRegion);
            HalconCpp::HTuple row, h, col, w, angle;
            HalconCpp::SmallestRectangle2(ellipseRegion, &row, &col, &angle, &h, &w);
            HalconCpp::GenEllipse(&ellipseRegion, row, col, angle, qMax(h, cMinR) + cMinR, qMax(w, cMinR) + cMinR);
        }
        return ellipseRegion;
    }

    HalconCpp::HObject getCrossContours(const HalconCpp::HTuple &p_rows, const HalconCpp::HTuple &p_cols)
    {
        const HalconCpp::HTuple cSize = 60;
        const HalconCpp::HTuple cDirection = 0.785398;
        const HalconCpp::HTuple radius = 20;
        HalconCpp::HObject crossContours;
        HalconCpp::GenEmptyRegion(&crossContours);
        assert(p_rows.Length() > 0);
        for (int i = 0; i < p_rows.Length(); i++)
        {
            HalconCpp::HObject CircleDot, Circle;

            //得到十字叉
            GenCrossContourXld(&CircleDot, p_rows[i], p_cols[i], cSize, cDirection);
            GenRegionContourXld(CircleDot, &CircleDot, "margin");
            GenCircle(&Circle, p_rows[i], p_cols[i], radius);
            Intersection(Circle, CircleDot, &CircleDot);
            //联合区域
            Union2(crossContours, CircleDot, &crossContours);
        }
        return crossContours;
    }
};
