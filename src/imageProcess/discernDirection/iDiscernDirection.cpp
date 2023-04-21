#include "iDiscernDirection.h"
#include "discernDirection/discernLzDirection.h"
#include "discernDirection/discernYcgDirection.h"
#include "discernDirectionDef.h"
#include "HDevThread.h"

namespace TIGER_ProcessTool
{
    using namespace HalconCpp;
    bool IDiscernDirection::getDirection(const CAssembleType &p_AssembleType, const HalconCpp::HObject &p_region, bool &p_direction)
    {
        m_error.clear();
        try
        {
            //****5、判断方向*****
            //模型初始化
            HTuple preprocessParamFileName, retrainedModelFileName;
            switch (p_AssembleType)
            {
            case catYcg:
            {
                preprocessParamFileName = "./halconPara/preprocessParamYcg.hdict";
                retrainedModelFileName = "./halconPara/bestModelClassificationYcg.hdl";
                break;
            }
            case catLz:
            {
                preprocessParamFileName = "./halconPara/preprocessParamLz.hdict";
                retrainedModelFileName = "./halconPara/bestModelClassificationLz.hdl";
                break;
            }
            default:
                break;
            }

            HTuple batchSizeInference, dlModelHandle, classNames, classIDs, dlPreprocessParam;
            batchSizeInference = 1;
            ReadDlModel(retrainedModelFileName, &dlModelHandle);
            SetDlModelParam(dlModelHandle,"runtime","cpu");
            SetDlModelParam(dlModelHandle, "batch_size", batchSizeInference);
            GetDlModelParam(dlModelHandle, "class_names", &classNames);
            GetDlModelParam(dlModelHandle, "class_ids", &classIDs);
            ReadDict(preprocessParamFileName, HTuple(), HTuple(), &dlPreprocessParam);

            //检测缺陷
            HTuple dlSampleBatch, dlResultBatch;
            gen_dl_samples_from_images(p_region, &dlSampleBatch);
            preprocess_dl_samples(dlSampleBatch, dlPreprocessParam);
            ApplyDlModel(dlModelHandle, dlSampleBatch, HTuple(), &dlResultBatch);

            GetDictTuple(dlResultBatch, "classification_class_names", &classNames);
            classNames = ((const HTuple&)classNames)[0];
            if (classNames == HTuple("negative"))
            {
                p_direction = false;
            }
            else if(classNames == HTuple("positive"))
            {
                p_direction = true;
            }
            else
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
}

using namespace TIGER_ProcessTool;
TIGER_ProcessTool::IDiscernDirection *createDiscernPositioning(bool p_processYcg)
{
    IDiscernDirection *p = nullptr;
    if(p_processYcg)
    {
        p = new CYcgDiscern;
    }
    else
    {
        p = new CLzDiscern;
    }
    assert(p != nullptr);
    return p;
}