#pragma once
#include "system/basic.h"
#include "system/myMath.h"
#include <HalconCpp.h>
class QImage;
namespace TIGER_ProcessTool
{
    class IProcessToolFun
    {
    public:
        virtual ~IProcessToolFun(){};
        HalconCpp::HObject getProcessObject() const { return m_hObject; };
        QString getErrorMsg() const { return m_error; };

    protected:
        QString m_error;
        HalconCpp::HObject m_hObject;
    };
};