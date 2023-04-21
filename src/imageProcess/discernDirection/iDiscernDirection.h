#pragma once
#include "../basic.h"
#include "ui/mainWindowDef.h"
#include <QSize>
enum CDiscernType : uint8_t;
namespace TIGER_ProcessTool
{
    class IDiscernDirection : public IProcessToolFun
    {
    public:
        virtual ~IDiscernDirection(){};
        virtual bool processUnionRegion(const HalconCpp::HObject &p_image, const HalconCpp::HObject &p_roiRegion, QSize p_imageSize) = 0;
        bool getDirection() const { return m_direction; };

    protected:
        enum CAssembleType
        {
            catYcg,
            catLz,
            catMax
        };
        bool getDirection(const CAssembleType &p_AssembleType, const HalconCpp::HObject &p_region, bool &p_direction);

    protected:
        bool m_direction;
    };
}

TIGER_ProcessTool::IDiscernDirection *createDiscernPositioning(bool p_processYcg);