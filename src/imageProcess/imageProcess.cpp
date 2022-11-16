#include "imageProcess.h"
#include "hal/camera/baslerCamera.h"
#include "hal/camera/baslerCameraLz.h"
#include "system/tool/halconTool.h"
#include "imageProcess/discernDirection/iDiscernDirection.h"
#include <QFont>
#include <QPainter>
namespace TIGER_ProcessTool
{
    using namespace HalconCpp;

    CStationMatch::CStationMatch():m_direction(nullptr)
    {
    }

    CStationMatch::~CStationMatch()
    {
    }

    bool CStationMatch::processImage(bool p_processYcg)
    {
        m_isError = true;

        if(p_processYcg == true)
        {
            if (baslerCamera()->isAcquire())
            {
                qInfo() << cnStr("延长杆相机正在采集，正在尝试停止相机采集");
                if (baslerCamera()->acquireChange())
                {
                    qInfo() << cnStr("停止相机采集成功");
                }
                else
                {
                    m_error = cnStr("停止相机采集失败，无法拍照识别");
                    return false;
                }
            }
            if (!(baslerCamera()->connected() && baslerCamera()->singleFrame()))
            {
                m_error = cnStr("相机采集失败，无法拍照识别");
                return false;
            }
            m_markImage = baslerCamera()->getLastImage();
        }
        else
        {
            if (baslerCameraLz()->isAcquire())
            {
                qInfo() << cnStr("螺柱相机正在采集，正在尝试停止相机采集");
                if (baslerCameraLz()->acquireChange())
                {
                    qInfo() << cnStr("停止相机采集成功");
                }
                else
                {
                    m_error = cnStr("停止相机采集失败，无法拍照识别");
                    return false;
                }
            }
            if (!(baslerCameraLz()->connected() && baslerCameraLz()->singleFrame()))
            {
                m_error = cnStr("相机采集失败，无法拍照识别");
                return false;
            }
            m_markImage = baslerCameraLz()->getLastImage();
        }

        HImage hImage;
        if (!TIGER_HalconTool::qimgToHimg(m_markImage, hImage))
        {
            m_error = cnStr("图像转换失败，无法识别处理");
            return false;
        }

        IDiscernDirection *pFeaturesPositioning = createDiscernPositioning(p_processYcg);
        if (!pFeaturesPositioning->processUnionRegion(hImage, m_markImage.size()))
        {
            m_error = cnStr("识别失败：%1").arg(pFeaturesPositioning->getErrorMsg());
            delPtr(pFeaturesPositioning);
            return false;
        }
        m_direction = pFeaturesPositioning->getDirection();

        if (!TIGER_HalconTool::himgToQimg(hImage, m_markImage))
        {
            m_error = cnStr("Halcon图像转换为QImage失败，无法显示图像");
            delPtr(pFeaturesPositioning);
            return false;
        }
        delPtr(pFeaturesPositioning);
        m_isError = false;
        return true;
    }

    QImage CStationMatch::getMarkImage()
    {
        if (m_isError && (!m_markImage.isNull()))
        {
            QPainter painter(&m_markImage);
            QFont font;
            font.setPixelSize(100);
            QPen pen(QColor(255, 0, 0));
            painter.setPen(pen);
            painter.setFont(font);
            painter.drawText(m_markImage.rect(), "NG", Qt::AlignLeft | Qt::AlignTop);
        }
        return m_markImage;
    }

    QString CStationMatch::getErrorMsg() const
    {
        return m_error;
    }
};