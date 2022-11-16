#include "createRoi.h"
#include "system/tool/halconTool.h"
#include "hal/camera/baslerCamera.h"
#include "hal/camera/baslerCameraLz.h"
using namespace TIGER_HalconTool;
using namespace HalconCpp;

namespace TIGER_ProcessTool
{
    bool CCreateRoi::createRoi(HalconCpp::HTuple &p_handle, bool p_processYcg)
    {
        m_error.clear();
        try
        {
            HImage hImage;
            if(p_processYcg == true)
            {
                if (!TIGER_HalconTool::qimgToHimg(baslerCamera()->getLastImage(), hImage))
                {
                    qInfo() << cnStr("延长杆的图像转换失败，无法识别处理");
                    return false;
                }
            }
            else
            {
                if (!TIGER_HalconTool::qimgToHimg(baslerCameraLz()->getLastImage(), hImage))
                {
                    qInfo() << cnStr("螺柱的图像转换失败，无法识别处理");
                    return false;
                }
            }
            HTuple x, y;
            GetImageSize(hImage, &x, &y);
            SetPart(p_handle, 0, 0, y, x);
            DispObj(hImage, p_handle);
            HTuple row, column, phi, length1, length2;
            DrawRectangle2(p_handle, &row, &column, &phi, &length1, &length2);
            GenRectangle2(&m_hObject, row, column, phi, length1, length2);
            if(p_processYcg == true)
            {
                WriteRegion(m_hObject, "ycgRoiRegion.hobj");
            }
            else
            {
                WriteRegion(m_hObject, "lzRoiRegion.hobj");
            }
            CloseWindow(p_handle);
            return true;
        }
        catch (HException &except)
        {
            m_error = except.ErrorMessage().Text();
        }
        return false;
    }
};