#pragma once
#include "system/basic.h"
#include "system/myMath.h"
#include <HalconCpp.h>
#include <QImage>
class QImage;
namespace TIGER_ProcessTool
{
    class CStationMatch
    {
    public:
        CStationMatch();
        ~CStationMatch();
        bool processImage(bool p_processYcg, const QImage &p_qImage, const QPainterPath &p_path);
        bool getDirection() const { return m_direction; };
        QImage getMarkImage();
        QString getErrorMsg() const;

    protected:
        QImage m_markImage;
        QString m_error;
        bool m_isError;
        bool m_direction;
    };
};