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

    bool CStationMatch::processImage(bool p_processYcg, const QImage &p_qImage, const QPainterPath &p_path)
    {
        m_isError = true;

        // Qt图像转换为Halcon图像
        m_markImage = p_qImage;
        HImage hImage;
        if (!TIGER_HalconTool::qimgToHimg(m_markImage, hImage))
        {
            m_error = cnStr("图像转换失败，无法识别处理");
            return false;
        }

        // Qt区域转换为Halcon区域
        QImage qRoiImage = p_qImage;
        if (qRoiImage.isNull())
        {
            myInfo << cnStr("传入图像为空！");
            return false;
        }
        qRoiImage.fill(Qt::white);
        QPainter painter(&qRoiImage);
        painter.setPen(Qt::black);
        painter.setBrush(Qt::black);
        painter.drawPath(p_path);
        HImage hRoiImage;
        if (!TIGER_HalconTool::qimgToHimg(qRoiImage, hRoiImage))
        {
            myInfo << cnStr("图像转换失败，无法识别处理");
            return false;
        }
        HObject hRoiRegion;
        HalconCpp::Threshold(hRoiImage, &hRoiRegion, 0, 1);

        //方向判断
        IDiscernDirection *pFeaturesPositioning = createDiscernPositioning(p_processYcg);
        if (!pFeaturesPositioning->processUnionRegion(hImage, hRoiRegion, m_markImage.size()))
        {
            m_error = cnStr("识别失败：%1").arg(pFeaturesPositioning->getErrorMsg());
            delPtr(pFeaturesPositioning);
            return false;
        }
        m_direction = pFeaturesPositioning->getDirection();
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