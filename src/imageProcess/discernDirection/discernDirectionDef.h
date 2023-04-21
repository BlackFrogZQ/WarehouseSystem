#include "system/tool/halconTool.h"
using namespace HalconCpp;

namespace TIGER_ProcessTool
{
    void check_dl_preprocess_param (HTuple hv_DLPreprocessParam);
    void convert_rect2_5to8param (HTuple hv_Row, HTuple hv_Col, HTuple hv_Length1, HTuple hv_Length2, HTuple hv_Phi, HTuple *hv_Row1, HTuple *hv_Col1, HTuple *hv_Row2, HTuple *hv_Col2, HTuple *hv_Row3, HTuple *hv_Col3, HTuple *hv_Row4, HTuple *hv_Col4);
    void convert_rect2_8to5param (HTuple hv_Row1, HTuple hv_Col1, HTuple hv_Row2, HTuple hv_Col2, HTuple hv_Row3, HTuple hv_Col3, HTuple hv_Row4, HTuple hv_Col4, HTuple hv_ForceL1LargerL2, HTuple *hv_Row, HTuple *hv_Col, HTuple *hv_Length1, HTuple *hv_Length2, HTuple *hv_Phi);
    void disp_continue_message (HTuple hv_WindowHandle, HTuple hv_Color, HTuple hv_Box);
    void gen_dl_samples_from_images (HObject ho_Images, HTuple *hv_DLSampleBatch);
    void preprocess_dl_model_anomaly (HObject ho_AnomalyImages, HObject *ho_AnomalyImagesPreprocessed, HTuple hv_DLPreprocessParam);
    void preprocess_dl_model_bbox_rect1 (HObject ho_ImageRaw, HTuple hv_DLSample, HTuple hv_DLPreprocessParam);
    void preprocess_dl_model_bbox_rect2 (HObject ho_ImageRaw, HTuple hv_DLSample, HTuple hv_DLPreprocessParam);
    void preprocess_dl_model_images (HObject ho_Images, HObject *ho_ImagesPreprocessed, HTuple hv_DLPreprocessParam);
    void preprocess_dl_model_segmentations (HObject ho_ImagesRaw, HObject ho_Segmentations, HObject *ho_SegmentationsPreprocessed, HTuple hv_DLPreprocessParam);
    void preprocess_dl_samples (HTuple hv_DLSampleBatch, HTuple hv_DLPreprocessParam);
    void reassign_pixel_values (HObject ho_Image, HObject *ho_ImageOut, HTuple hv_ValuesToChange, HTuple hv_NewValue);
    void replace_legacy_preprocessing_parameters (HTuple hv_DLPreprocessParam);
    void set_display_font (HTuple hv_WindowHandle, HTuple hv_Size, HTuple hv_Font, HTuple hv_Bold, HTuple hv_Slant);

    void check_dl_preprocess_param (HTuple hv_DLPreprocessParam)
    {
      HTuple  hv_CheckParams, hv_KeyExists, hv_DLModelType;
      HTuple  hv_Exception, hv_SupportedModelTypes, hv_Index;
      HTuple  hv_ParamNamesGeneral, hv_ParamNamesSegmentation;
      HTuple  hv_ParamNamesDetectionOptional, hv_ParamNamesPreprocessingOptional;
      HTuple  hv_ParamNamesAll, hv_ParamNames, hv_KeysExists;
      HTuple  hv_I, hv_Exists, hv_InputKeys, hv_Key, hv_Value;
      HTuple  hv_Indices, hv_ValidValues, hv_ValidTypes, hv_V;
      HTuple  hv_T, hv_IsInt, hv_ValidTypesListing, hv_ValidValueListing;
      HTuple  hv_EmptyStrings, hv_ImageRangeMinExists, hv_ImageRangeMaxExists;
      HTuple  hv_ImageRangeMin, hv_ImageRangeMax, hv_IndexParam;
      HTuple  hv_SetBackgroundID, hv_ClassIDsBackground, hv_Intersection;
      HTuple  hv_IgnoreClassIDs, hv_KnownClasses, hv_IgnoreClassID;
      HTuple  hv_OptionalKeysExist, hv_InstanceType, hv_IgnoreDirection;
      HTuple  hv_ClassIDsNoOrientation, hv_SemTypes;

      hv_CheckParams = 1;

      GetDictParam(hv_DLPreprocessParam, "key_exists", "check_params", &hv_KeyExists);
      if (0 != hv_KeyExists)
      {
          GetDictTuple(hv_DLPreprocessParam, "check_params", &hv_CheckParams);
          if (0 != (hv_CheckParams.TupleNot()))
          {
              return;
          }
      }
      try
      {
        GetDictTuple(hv_DLPreprocessParam, "model_type", &hv_DLModelType);
      }
      catch (HException &HDevExpDefaultException)
      {
        HDevExpDefaultException.ToHTuple(&hv_Exception);
        throw HException(HTuple(HTuple("DLPreprocessParam needs the parameter: '")+"model_type")+"'");
      }
      hv_SupportedModelTypes.Clear();
      hv_SupportedModelTypes[0] = "anomaly_detection";
      hv_SupportedModelTypes[1] = "classification";
      hv_SupportedModelTypes[2] = "detection";
      hv_SupportedModelTypes[3] = "segmentation";
      TupleFind(hv_SupportedModelTypes, hv_DLModelType, &hv_Index);
      if (0 != (HTuple(int(hv_Index==-1)).TupleOr(int(hv_Index==HTuple()))))
      {
          throw HException(HTuple("Only models of type 'anomaly_detection', 'classification', 'detection', or 'segmentation' are supported"));
          return;
      }
      hv_ParamNamesGeneral.Clear();
      hv_ParamNamesGeneral[0] = "model_type";
      hv_ParamNamesGeneral[1] = "image_width";
      hv_ParamNamesGeneral[2] = "image_height";
      hv_ParamNamesGeneral[3] = "image_num_channels";
      hv_ParamNamesGeneral[4] = "image_range_min";
      hv_ParamNamesGeneral[5] = "image_range_max";
      hv_ParamNamesGeneral[6] = "normalization_type";
      hv_ParamNamesGeneral[7] = "domain_handling";
      hv_ParamNamesSegmentation.Clear();
      hv_ParamNamesSegmentation[0] = "ignore_class_ids";
      hv_ParamNamesSegmentation[1] = "set_background_id";
      hv_ParamNamesSegmentation[2] = "class_ids_background";
      hv_ParamNamesDetectionOptional.Clear();
      hv_ParamNamesDetectionOptional[0] = "instance_type";
      hv_ParamNamesDetectionOptional[1] = "ignore_direction";
      hv_ParamNamesDetectionOptional[2] = "class_ids_no_orientation";
      hv_ParamNamesPreprocessingOptional.Clear();
      hv_ParamNamesPreprocessingOptional[0] = "mean_values_normalization";
      hv_ParamNamesPreprocessingOptional[1] = "deviation_values_normalization";
      hv_ParamNamesAll.Clear();
      hv_ParamNamesAll.Append(hv_ParamNamesGeneral);
      hv_ParamNamesAll.Append(hv_ParamNamesSegmentation);
      hv_ParamNamesAll.Append(hv_ParamNamesDetectionOptional);
      hv_ParamNamesAll.Append(hv_ParamNamesPreprocessingOptional);
      hv_ParamNames = hv_ParamNamesGeneral;
      if (0 != (int(hv_DLModelType==HTuple("segmentation"))))
      {
          hv_ParamNames = hv_ParamNames.TupleConcat(hv_ParamNamesSegmentation);
      }
      replace_legacy_preprocessing_parameters(hv_DLPreprocessParam);
      GetDictParam(hv_DLPreprocessParam, "key_exists", hv_ParamNames, &hv_KeysExists);
      if (0 != (int(((hv_KeysExists.TupleEqualElem(0)).TupleSum())>0)))
      {
        {
          HTuple end_val52 = hv_KeysExists.TupleLength();
          HTuple step_val52 = 1;
          for (hv_I=0; hv_I.Continue(end_val52, step_val52); hv_I += step_val52)
          {
              hv_Exists = HTuple(hv_KeysExists[hv_I]);
              if (0 != (hv_Exists.TupleNot()))
              {
                  throw HException(("DLPreprocessParam needs the parameter: '"+HTuple(hv_ParamNames[hv_I]))+"'");
              }
          }
        }
      }
      GetDictParam(hv_DLPreprocessParam, "keys", HTuple(), &hv_InputKeys);
      {
          HTuple end_val62 = (hv_InputKeys.TupleLength())-1;
          HTuple step_val62 = 1;
          for (hv_I=0; hv_I.Continue(end_val62, step_val62); hv_I += step_val62)
          {
            hv_Key = HTuple(hv_InputKeys[hv_I]);
            GetDictTuple(hv_DLPreprocessParam, hv_Key, &hv_Value);
            TupleFind(hv_ParamNamesAll, hv_Key, &hv_Indices);
            if (0 != (int(hv_Indices==-1)))
            {
              throw HException(("Unknown key for DLPreprocessParam: '"+HTuple(hv_InputKeys[hv_I]))+"'");
              return;
            }
            hv_ValidValues = HTuple();
            hv_ValidTypes = HTuple();
            if (0 != (int(hv_Key==HTuple("normalization_type"))))
            {
              hv_ValidValues.Clear();
              hv_ValidValues[0] = "all_channels";
              hv_ValidValues[1] = "first_channel";
              hv_ValidValues[2] = "constant_values";
              hv_ValidValues[3] = "none";
            }
            else if (0 != (int(hv_Key==HTuple("domain_handling"))))
            {
              if (0 != (int(hv_DLModelType==HTuple("anomaly_detection"))))
              {
                hv_ValidValues.Clear();
                hv_ValidValues[0] = "full_domain";
                hv_ValidValues[1] = "crop_domain";
                hv_ValidValues[2] = "keep_domain";
              }
              else
              {
                hv_ValidValues.Clear();
                hv_ValidValues[0] = "full_domain";
                hv_ValidValues[1] = "crop_domain";
              }
            }
            else if (0 != (int(hv_Key==HTuple("model_type"))))
            {
              hv_ValidValues.Clear();
              hv_ValidValues[0] = "anomaly_detection";
              hv_ValidValues[1] = "classification";
              hv_ValidValues[2] = "detection";
              hv_ValidValues[3] = "segmentation";
            }
            else if (0 != (int(hv_Key==HTuple("set_background_id"))))
            {
              hv_ValidTypes = "int";
            }
            else if (0 != (int(hv_Key==HTuple("class_ids_background"))))
            {
              hv_ValidTypes = "int";
            }

            if (0 != (int((hv_ValidTypes.TupleLength())>0)))
            {
              {
              HTuple end_val91 = (hv_ValidTypes.TupleLength())-1;
              HTuple step_val91 = 1;
              for (hv_V=0; hv_V.Continue(end_val91, step_val91); hv_V += step_val91)
              {
                hv_T = HTuple(hv_ValidTypes[hv_V]);
                if (0 != (int(hv_T==HTuple("int"))))
                {
                  TupleIsInt(hv_Value, &hv_IsInt);
                  if (0 != (hv_IsInt.TupleNot()))
                  {
                    hv_ValidTypes = ("'"+hv_ValidTypes)+"'";
                    if (0 != (int((hv_ValidTypes.TupleLength())<2)))
                    {
                      hv_ValidTypesListing = hv_ValidTypes;
                    }
                    else
                    {
                      hv_ValidTypesListing = (((hv_ValidTypes.TupleSelectRange(0,HTuple(0).TupleMax2((hv_ValidTypes.TupleLength())-2)))+HTuple(", "))+HTuple(hv_ValidTypes[(hv_ValidTypes.TupleLength())-1])).TupleSum();
                    }
                    throw HException(((((("The value given in the key '"+hv_Key)+"' of DLPreprocessParam is invalid. Valid types are: ")+hv_ValidTypesListing)+". The given value was '")+hv_Value)+"'.");
                    return;
                  }
                }
                else
                {
                  throw HException("Internal error. Unknown valid type.");
                }
              }
              }
            }

            if (0 != (int((hv_ValidValues.TupleLength())>0)))
            {
              TupleFindFirst(hv_ValidValues, hv_Value, &hv_Index);
              if (0 != (int(hv_Index==-1)))
              {
                hv_ValidValues = ("'"+hv_ValidValues)+"'";
                if (0 != (int((hv_ValidValues.TupleLength())<2)))
                {
                  hv_ValidValueListing = hv_ValidValues;
                }
                else
                {
                  hv_EmptyStrings = HTuple((hv_ValidValues.TupleLength())-2,"");
                  hv_ValidValueListing = (((hv_ValidValues.TupleSelectRange(0,HTuple(0).TupleMax2((hv_ValidValues.TupleLength())-2)))+HTuple(", "))+(hv_EmptyStrings.TupleConcat(HTuple(hv_ValidValues[(hv_ValidValues.TupleLength())-1])))).TupleSum();
                }
                throw HException(((((("The value given in the key '"+hv_Key)+"' of DLPreprocessParam is invalid. Valid values are: ")+hv_ValidValueListing)+". The given value was '")+hv_Value)+"'.");
              }
            }
          }
      }

      if (0 != (HTuple(int(hv_DLModelType==HTuple("classification"))).TupleOr(int(hv_DLModelType==HTuple("detection")))))
      {
        GetDictParam(hv_DLPreprocessParam, "key_exists", "image_range_min", &hv_ImageRangeMinExists);
        GetDictParam(hv_DLPreprocessParam, "key_exists", "image_range_max", &hv_ImageRangeMaxExists);
        if (0 != hv_ImageRangeMinExists)
        {
          GetDictTuple(hv_DLPreprocessParam, "image_range_min", &hv_ImageRangeMin);
          if (0 != (int(hv_ImageRangeMin!=-127)))
          {
            throw HException(("For model type "+hv_DLModelType)+" ImageRangeMin has to be -127.");
          }
        }
        if (0 != hv_ImageRangeMaxExists)
        {
          GetDictTuple(hv_DLPreprocessParam, "image_range_max", &hv_ImageRangeMax);
          if (0 != (int(hv_ImageRangeMax!=128)))
          {
            throw HException(("For model type "+hv_DLModelType)+" ImageRangeMax has to be 128.");
          }
        }
      }

      if (0 != (int(hv_DLModelType==HTuple("segmentation"))))
      {
        GetDictParam(hv_DLPreprocessParam, "key_exists", hv_ParamNamesDetectionOptional, &hv_KeysExists);
        {
          HTuple end_val151 = (hv_ParamNamesDetectionOptional.TupleLength())-1;
          HTuple step_val151 = 1;
          for (hv_IndexParam=0; hv_IndexParam.Continue(end_val151, step_val151); hv_IndexParam += step_val151)
          {
            if (0 != (HTuple(hv_KeysExists[hv_IndexParam])))
            {
              GetDictTuple(hv_DLPreprocessParam, HTuple(hv_ParamNamesDetectionOptional[hv_IndexParam]), 
                  &hv_Value);
              if (0 != (int(hv_Value!=HTuple())))
              {
                throw HException(((("The preprocessing parameter '"+HTuple(hv_ParamNamesDetectionOptional[hv_IndexParam]))+"' was set to ")+hv_Value)+HTuple(" but for segmentation it should be set to [], as it is not used for this method."));
              }
            }
          }
        }

        GetDictTuple(hv_DLPreprocessParam, "set_background_id", &hv_SetBackgroundID);
        if (0 != (int((hv_SetBackgroundID.TupleLength())>1)))
        {
          throw HException("Only one class_id as 'set_background_id' allowed.");
        }

        GetDictTuple(hv_DLPreprocessParam, "class_ids_background", &hv_ClassIDsBackground);
        if (0 != (HTuple(HTuple(int((hv_SetBackgroundID.TupleLength())>0)).TupleAnd(HTuple(int((hv_ClassIDsBackground.TupleLength())>0)).TupleNot())).TupleOr(HTuple(int((hv_ClassIDsBackground.TupleLength())>0)).TupleAnd(HTuple(int((hv_SetBackgroundID.TupleLength())>0)).TupleNot()))))
        {
          throw HException("Both keys 'set_background_id' and 'class_ids_background' are required.");
        }
        if (0 != (int((hv_SetBackgroundID.TupleLength())>0)))
        {
          TupleIntersection(hv_SetBackgroundID, hv_ClassIDsBackground, &hv_Intersection);
          if (0 != (hv_Intersection.TupleLength()))
          {
            throw HException("Class IDs in 'set_background_id' and 'class_ids_background' need to be disjoint.");
          }
        }
        GetDictTuple(hv_DLPreprocessParam, "ignore_class_ids", &hv_IgnoreClassIDs);
        hv_KnownClasses.Clear();
        hv_KnownClasses.Append(hv_SetBackgroundID);
        hv_KnownClasses.Append(hv_ClassIDsBackground);
        {
        HTuple end_val179 = (hv_IgnoreClassIDs.TupleLength())-1;
        HTuple step_val179 = 1;
        for (hv_I=0; hv_I.Continue(end_val179, step_val179); hv_I += step_val179)
        {
          hv_IgnoreClassID = HTuple(hv_IgnoreClassIDs[hv_I]);
          TupleFindFirst(hv_KnownClasses, hv_IgnoreClassID, &hv_Index);
          if (0 != (HTuple(int((hv_Index.TupleLength())>0)).TupleAnd(int(hv_Index!=-1))))
          {
            throw HException("The given 'ignore_class_ids' must not be included in the 'class_ids_background' or 'set_background_id'.");
          }
        }
        }
      }
      else if (0 != (int(hv_DLModelType==HTuple("detection"))))
      {
        GetDictParam(hv_DLPreprocessParam, "key_exists", hv_ParamNamesSegmentation, &hv_KeysExists);
        {
        HTuple end_val190 = (hv_ParamNamesSegmentation.TupleLength())-1;
        HTuple step_val190 = 1;
        for (hv_IndexParam=0; hv_IndexParam.Continue(end_val190, step_val190); hv_IndexParam += step_val190)
        {
          if (0 != (HTuple(hv_KeysExists[hv_IndexParam])))
          {
            GetDictTuple(hv_DLPreprocessParam, HTuple(hv_ParamNamesSegmentation[hv_IndexParam]), 
                &hv_Value);
            if (0 != (int(hv_Value!=HTuple())))
            {
              throw HException(((("The preprocessing parameter '"+HTuple(hv_ParamNamesSegmentation[hv_IndexParam]))+"' was set to ")+hv_Value)+HTuple(" but for detection it should be set to [], as it is not used for this method."));
            }
          }
        }
        }
        GetDictParam(hv_DLPreprocessParam, "key_exists", hv_ParamNamesDetectionOptional, 
            &hv_OptionalKeysExist);
        if (0 != (HTuple(hv_OptionalKeysExist[0])))
        {
          GetDictTuple(hv_DLPreprocessParam, HTuple(hv_ParamNamesDetectionOptional[0]), 
              &hv_InstanceType);
          if (0 != (int(((HTuple("rectangle1").Append("rectangle2")).TupleFind(hv_InstanceType))==-1)))
          {
            throw HException(("Invalid generic parameter for 'instance_type': "+hv_InstanceType)+HTuple(", only 'rectangle1' and 'rectangle2' are allowed"));
          }
        }
        GetDictParam(hv_DLPreprocessParam, "key_exists", hv_ParamNamesDetectionOptional, 
            &hv_OptionalKeysExist);
        if (0 != (HTuple(hv_OptionalKeysExist[1])))
        {
          GetDictTuple(hv_DLPreprocessParam, HTuple(hv_ParamNamesDetectionOptional[1]), &hv_IgnoreDirection);
          if (0 != (int(((HTuple(1).Append(0)).TupleFind(hv_IgnoreDirection))==-1)))
          {
            throw HException(("Invalid generic parameter for 'ignore_direction': "+hv_IgnoreDirection)+HTuple(", only true and false are allowed"));
          }
        }
        if (0 != (HTuple(hv_OptionalKeysExist[2])))
        {
          GetDictTuple(hv_DLPreprocessParam, HTuple(hv_ParamNamesDetectionOptional[2]), &hv_ClassIDsNoOrientation);
          TupleSemTypeElem(hv_ClassIDsNoOrientation, &hv_SemTypes);
          if (0 != (HTuple(int(hv_ClassIDsNoOrientation!=HTuple())).TupleAnd(int(((hv_SemTypes.TupleEqualElem("integer")).TupleSum())!=(hv_ClassIDsNoOrientation.TupleLength())))))
          {
            throw HException(("Invalid generic parameter for 'class_ids_no_orientation': "+hv_ClassIDsNoOrientation)+HTuple(", only integers are allowed"));
          }
          else
          {
            if (0 != (HTuple(int(hv_ClassIDsNoOrientation!=HTuple())).TupleAnd(int(((hv_ClassIDsNoOrientation.TupleGreaterEqualElem(0)).TupleSum())!=(hv_ClassIDsNoOrientation.TupleLength())))))
            {
              throw HException(("Invalid generic parameter for 'class_ids_no_orientation': "+hv_ClassIDsNoOrientation)+HTuple(", only non-negative integers are allowed"));
            }
          }
        }
      }
      return;
    }

    void convert_rect2_5to8param (HTuple hv_Row, HTuple hv_Col, HTuple hv_Length1, HTuple hv_Length2, HTuple hv_Phi, HTuple *hv_Row1, HTuple *hv_Col1, HTuple *hv_Row2, HTuple *hv_Col2, HTuple *hv_Row3, HTuple *hv_Col3, HTuple *hv_Row4, HTuple *hv_Col4)
    {
      HTuple  hv_Co1, hv_Co2, hv_Si1, hv_Si2;
      hv_Co1 = (hv_Phi.TupleCos())*hv_Length1;
      hv_Co2 = (hv_Phi.TupleCos())*hv_Length2;
      hv_Si1 = (hv_Phi.TupleSin())*hv_Length1;
      hv_Si2 = (hv_Phi.TupleSin())*hv_Length2;

      (*hv_Col1) = (hv_Co1-hv_Si2)+hv_Col;
      (*hv_Row1) = ((-hv_Si1)-hv_Co2)+hv_Row;
      (*hv_Col2) = ((-hv_Co1)-hv_Si2)+hv_Col;
      (*hv_Row2) = (hv_Si1-hv_Co2)+hv_Row;
      (*hv_Col3) = ((-hv_Co1)+hv_Si2)+hv_Col;
      (*hv_Row3) = (hv_Si1+hv_Co2)+hv_Row;
      (*hv_Col4) = (hv_Co1+hv_Si2)+hv_Col;
      (*hv_Row4) = ((-hv_Si1)+hv_Co2)+hv_Row;

      return;
    }

    void convert_rect2_8to5param (HTuple hv_Row1, HTuple hv_Col1, HTuple hv_Row2, HTuple hv_Col2, HTuple hv_Row3, HTuple hv_Col3, HTuple hv_Row4, HTuple hv_Col4, HTuple hv_ForceL1LargerL2, HTuple *hv_Row, HTuple *hv_Col, HTuple *hv_Length1, HTuple *hv_Length2, HTuple *hv_Phi)
    {
      HTuple  hv_Hor, hv_Vert, hv_IdxSwap, hv_Tmp;
      (*hv_Row) = (((hv_Row1+hv_Row2)+hv_Row3)+hv_Row4)/4.0;
      (*hv_Col) = (((hv_Col1+hv_Col2)+hv_Col3)+hv_Col4)/4.0;
      (*hv_Length1) = ((((hv_Row1-hv_Row2)*(hv_Row1-hv_Row2))+((hv_Col1-hv_Col2)*(hv_Col1-hv_Col2))).TupleSqrt())/2.0;
      (*hv_Length2) = ((((hv_Row2-hv_Row3)*(hv_Row2-hv_Row3))+((hv_Col2-hv_Col3)*(hv_Col2-hv_Col3))).TupleSqrt())/2.0;
      hv_Hor = hv_Col1-hv_Col2;
      hv_Vert = hv_Row2-hv_Row1;
      if (0 != hv_ForceL1LargerL2)
      {
        hv_IdxSwap = (((*hv_Length2)-(*hv_Length1)).TupleGreaterElem(1e-9)).TupleFind(1);
        if (0 != (int(hv_IdxSwap!=-1)))
        {
          hv_Tmp = HTuple((*hv_Length1)[hv_IdxSwap]);
          (*hv_Length1)[hv_IdxSwap] = HTuple((*hv_Length2)[hv_IdxSwap]);
          (*hv_Length2)[hv_IdxSwap] = hv_Tmp;
          hv_Hor[hv_IdxSwap] = HTuple(hv_Col2[hv_IdxSwap])-HTuple(hv_Col3[hv_IdxSwap]);
          hv_Vert[hv_IdxSwap] = HTuple(hv_Row3[hv_IdxSwap])-HTuple(hv_Row2[hv_IdxSwap]);
        }
      }
      (*hv_Phi) = hv_Vert.TupleAtan2(hv_Hor);
      return;
    }

    void disp_continue_message (HTuple hv_WindowHandle, HTuple hv_Color, HTuple hv_Box)
    {
      HTuple  hv_GenParamName, hv_GenParamValue, hv_ContinueMessage;
      hv_GenParamName = HTuple();
      hv_GenParamValue = HTuple();
      if (0 != (int((hv_Box.TupleLength())>0)))
      {
        if (0 != (int(HTuple(hv_Box[0])==HTuple("false"))))
        {
          hv_GenParamName = hv_GenParamName.TupleConcat("box");
          hv_GenParamValue = hv_GenParamValue.TupleConcat("false");
        }
        else if (0 != (int(HTuple(hv_Box[0])!=HTuple("true"))))
        {
          hv_GenParamName = hv_GenParamName.TupleConcat("box_color");
          hv_GenParamValue = hv_GenParamValue.TupleConcat(HTuple(hv_Box[0]));
        }
      }
      if (0 != (int((hv_Box.TupleLength())>1)))
      {
        if (0 != (int(HTuple(hv_Box[1])==HTuple("false"))))
        {
          hv_GenParamName = hv_GenParamName.TupleConcat("shadow");
          hv_GenParamValue = hv_GenParamValue.TupleConcat("false");
        }
        else if (0 != (int(HTuple(hv_Box[1])!=HTuple("true"))))
        {
          hv_GenParamName = hv_GenParamName.TupleConcat("shadow_color");
          hv_GenParamValue = hv_GenParamValue.TupleConcat(HTuple(hv_Box[1]));
        }
      }
      //
      if (0 != (int(hv_Color==HTuple(""))))
      {
        hv_Color = HTuple();
      }
      hv_ContinueMessage = "Press Run (F5) to continue";
      DispText(hv_WindowHandle, hv_ContinueMessage, "window", "bottom", "right", hv_Color, 
          hv_GenParamName, hv_GenParamValue);
      return;
    }

    void gen_dl_samples_from_images (HObject ho_Images, HTuple *hv_DLSampleBatch)
    {
      HObject  ho_Image;
      HTuple  hv_NumImages, hv_ImageIndex, hv_DLSample;
      CountObj(ho_Images, &hv_NumImages);
      (*hv_DLSampleBatch) = HTuple(hv_NumImages,-1);
      {
        HTuple end_val10 = hv_NumImages-1;
        HTuple step_val10 = 1;
        for (hv_ImageIndex=0; hv_ImageIndex.Continue(end_val10, step_val10); hv_ImageIndex += step_val10)
        {
            SelectObj(ho_Images, &ho_Image, hv_ImageIndex+1);
            CreateDict(&hv_DLSample);
            SetDictObject(ho_Image, hv_DLSample, "image");
            (*hv_DLSampleBatch)[hv_ImageIndex] = hv_DLSample;
        }
      }
      return;
    }
  
    void preprocess_dl_model_anomaly (HObject ho_AnomalyImages, HObject *ho_AnomalyImagesPreprocessed, HTuple hv_DLPreprocessParam)
    {
      HTuple  hv_ImageWidth, hv_ImageHeight, hv_ImageRangeMin;
      HTuple  hv_ImageRangeMax, hv_DomainHandling, hv_ModelType;
      HTuple  hv_ImageNumChannels, hv_Min, hv_Max, hv_Range, hv_ImageWidthInput;
      HTuple  hv_ImageHeightInput, hv_EqualWidth, hv_EqualHeight;
      HTuple  hv_Type, hv_NumMatches, hv_NumImages, hv_EqualByte;
      HTuple  hv_NumChannelsAllImages, hv_ImageNumChannelsTuple;
      HTuple  hv_IndicesWrongChannels;

      check_dl_preprocess_param(hv_DLPreprocessParam);
      GetDictTuple(hv_DLPreprocessParam, "image_width", &hv_ImageWidth);
      GetDictTuple(hv_DLPreprocessParam, "image_height", &hv_ImageHeight);
      GetDictTuple(hv_DLPreprocessParam, "image_range_min", &hv_ImageRangeMin);
      GetDictTuple(hv_DLPreprocessParam, "image_range_max", &hv_ImageRangeMax);
      GetDictTuple(hv_DLPreprocessParam, "domain_handling", &hv_DomainHandling);
      GetDictTuple(hv_DLPreprocessParam, "model_type", &hv_ModelType);
      hv_ImageNumChannels = 1;
      if (0 != (int(hv_DomainHandling==HTuple("full_domain"))))
      {
        FullDomain(ho_AnomalyImages, &ho_AnomalyImages);
      }
      else if (0 != (int(hv_DomainHandling==HTuple("crop_domain"))))
      {
        CropDomain(ho_AnomalyImages, &ho_AnomalyImages);
      }
      else if (0 != (HTuple(int(hv_DomainHandling==HTuple("keep_domain"))).TupleAnd(int(hv_ModelType==HTuple("anomaly_detection")))))
      {
        //Anomaly detection models accept the additional option 'keep_domain'.
      }
      else
      {
        throw HException("Unsupported parameter value for 'domain_handling'");
      }
      //
      MinMaxGray(ho_AnomalyImages, ho_AnomalyImages, 0, &hv_Min, &hv_Max, &hv_Range);
      if (0 != (int(hv_Min<0.0)))
      {
        throw HException("Values of anomaly image must not be smaller than 0.0.");
      }
      //
      //Zoom images only if they have a different size than the specified size.
      GetImageSize(ho_AnomalyImages, &hv_ImageWidthInput, &hv_ImageHeightInput);
      hv_EqualWidth = hv_ImageWidth.TupleEqualElem(hv_ImageWidthInput);
      hv_EqualHeight = hv_ImageHeight.TupleEqualElem(hv_ImageHeightInput);
      if (0 != (HTuple(int((hv_EqualWidth.TupleMin())==0)).TupleOr(int((hv_EqualHeight.TupleMin())==0))))
      {
        ZoomImageSize(ho_AnomalyImages, &ho_AnomalyImages, hv_ImageWidth, hv_ImageHeight, 
            "nearest_neighbor");
      }
      //
      //Check the type of the input images.
      GetImageType(ho_AnomalyImages, &hv_Type);
      TupleRegexpTest(hv_Type, "byte|real", &hv_NumMatches);
      CountObj(ho_AnomalyImages, &hv_NumImages);
      if (0 != (int(hv_NumMatches!=hv_NumImages)))
      {
        throw HException("Please provide only images of type 'byte' or 'real'.");
      }
      hv_EqualByte = hv_Type.TupleEqualElem("byte");
      if (0 != (int((hv_EqualByte.TupleMax())==1)))
      {
        if (0 != (int((hv_EqualByte.TupleMin())==0)))
        {
          throw HException("Passing mixed type images is not supported.");
        }
        ConvertImageType(ho_AnomalyImages, &ho_AnomalyImages, "real");
      }
      CountObj(ho_AnomalyImages, &hv_NumImages);
      CountChannels(ho_AnomalyImages, &hv_NumChannelsAllImages);
      TupleGenConst(hv_NumImages, hv_ImageNumChannels, &hv_ImageNumChannelsTuple);
      TupleFind(hv_NumChannelsAllImages.TupleNotEqualElem(hv_ImageNumChannelsTuple), 1, &hv_IndicesWrongChannels);
      if (0 != (int(hv_IndicesWrongChannels!=-1)))
      {
        throw HException("Number of channels in anomaly image is not supported. Please check for anomaly images with a number of channels different from 1.");
      }
      (*ho_AnomalyImagesPreprocessed) = ho_AnomalyImages;
      return;
    }

    void preprocess_dl_model_bbox_rect1 (HObject ho_ImageRaw, HTuple hv_DLSample, HTuple hv_DLPreprocessParam)
    {

      // Local iconic variables
      HObject  ho_DomainRaw;

      // Local control variables
      HTuple  hv_ImageWidth, hv_ImageHeight, hv_DomainHandling;
      HTuple  hv_BBoxCol1, hv_BBoxCol2, hv_BBoxRow1, hv_BBoxRow2;
      HTuple  hv_BBoxLabel, hv_Exception, hv_ImageId, hv_ExceptionMessage;
      HTuple  hv_BoxesInvalid, hv_RowDomain1, hv_ColumnDomain1;
      HTuple  hv_RowDomain2, hv_ColumnDomain2, hv_WidthRaw, hv_HeightRaw;
      HTuple  hv_Row1, hv_Col1, hv_Row2, hv_Col2, hv_MaskDelete;
      HTuple  hv_MaskNewBbox, hv_BBoxCol1New, hv_BBoxCol2New;
      HTuple  hv_BBoxRow1New, hv_BBoxRow2New, hv_BBoxLabelNew;
      HTuple  hv_FactorResampleWidth, hv_FactorResampleHeight;

      //
      //This procedure preprocesses the bounding boxes of type 'rectangle1' for a given sample.
      //
      //Check the validity of the preprocessing parameters.
      check_dl_preprocess_param(hv_DLPreprocessParam);
      //
      //Get the preprocessing parameters.
      GetDictTuple(hv_DLPreprocessParam, "image_width", &hv_ImageWidth);
      GetDictTuple(hv_DLPreprocessParam, "image_height", &hv_ImageHeight);
      GetDictTuple(hv_DLPreprocessParam, "domain_handling", &hv_DomainHandling);
      //
      //Get bounding box coordinates and labels.
      try
      {
        GetDictTuple(hv_DLSample, "bbox_col1", &hv_BBoxCol1);
        GetDictTuple(hv_DLSample, "bbox_col2", &hv_BBoxCol2);
        GetDictTuple(hv_DLSample, "bbox_row1", &hv_BBoxRow1);
        GetDictTuple(hv_DLSample, "bbox_row2", &hv_BBoxRow2);
        GetDictTuple(hv_DLSample, "bbox_label_id", &hv_BBoxLabel);
      }
      // catch (Exception) 
      catch (HException &HDevExpDefaultException)
      {
        HDevExpDefaultException.ToHTuple(&hv_Exception);
        GetDictTuple(hv_DLSample, "image_id", &hv_ImageId);
        if (0 != (int(HTuple(hv_Exception[0])==1302)))
        {
          hv_ExceptionMessage = "A bounding box coordinate key is missing.";
        }
        else
        {
          hv_ExceptionMessage = ((const HTuple&)hv_Exception)[2];
        }
        throw HException((("An error has occurred during preprocessing image_id "+hv_ImageId)+" when getting bounding box coordinates : ")+hv_ExceptionMessage);
      }
      //
      //Check that there are no invalid boxes.
      if (0 != (int((hv_BBoxRow1.TupleLength())>0)))
      {
        hv_BoxesInvalid = (hv_BBoxRow1.TupleGreaterEqualElem(hv_BBoxRow2)).TupleOr(hv_BBoxCol1.TupleGreaterEqualElem(hv_BBoxCol2));
        if (0 != (int((hv_BoxesInvalid.TupleSum())>0)))
        {
          GetDictTuple(hv_DLSample, "image_id", &hv_ImageId);
          throw HException(("An error has occurred during preprocessing image_id "+hv_ImageId)+HTuple(": Sample contains at least one box with zero-area, i.e. bbox_col1 >= bbox_col2 or bbox_row1 >= bbox_row2."));
        }
      }
      else
      {
        //There are no bounding boxes, hence nothing to do.
        return;
      }
      //
      //If the domain is cropped, crop bounding boxes.
      if (0 != (int(hv_DomainHandling==HTuple("crop_domain"))))
      {
        //
        //Get domain.
        GetDomain(ho_ImageRaw, &ho_DomainRaw);
        //
        //Set the size of the raw image to the domain extensions.
        SmallestRectangle1(ho_DomainRaw, &hv_RowDomain1, &hv_ColumnDomain1, &hv_RowDomain2, 
            &hv_ColumnDomain2);
        //The domain is always given as a pixel-precise region.
        hv_WidthRaw = (hv_ColumnDomain2-hv_ColumnDomain1)+1.0;
        hv_HeightRaw = (hv_RowDomain2-hv_RowDomain1)+1.0;
        //
        //Crop the bounding boxes.
        hv_Row1 = hv_BBoxRow1.TupleMax2(hv_RowDomain1-.5);
        hv_Col1 = hv_BBoxCol1.TupleMax2(hv_ColumnDomain1-.5);
        hv_Row2 = hv_BBoxRow2.TupleMin2(hv_RowDomain2+.5);
        hv_Col2 = hv_BBoxCol2.TupleMin2(hv_ColumnDomain2+.5);
        hv_MaskDelete = (hv_Row1.TupleGreaterEqualElem(hv_Row2)).TupleOr(hv_Col1.TupleGreaterEqualElem(hv_Col2));
        hv_MaskNewBbox = 1-hv_MaskDelete;
        //Store the preprocessed bounding box entries.
        hv_BBoxCol1New = (hv_Col1.TupleSelectMask(hv_MaskNewBbox))-hv_ColumnDomain1;
        hv_BBoxCol2New = (hv_Col2.TupleSelectMask(hv_MaskNewBbox))-hv_ColumnDomain1;
        hv_BBoxRow1New = (hv_Row1.TupleSelectMask(hv_MaskNewBbox))-hv_RowDomain1;
        hv_BBoxRow2New = (hv_Row2.TupleSelectMask(hv_MaskNewBbox))-hv_RowDomain1;
        hv_BBoxLabelNew = hv_BBoxLabel.TupleSelectMask(hv_MaskNewBbox);
        //
      }
      else if (0 != (int(hv_DomainHandling==HTuple("full_domain"))))
      {
        //If the entire image is used, set the variables accordingly.
        //Get the original size.
        GetImageSize(ho_ImageRaw, &hv_WidthRaw, &hv_HeightRaw);
        //Set new coordinates to input coordinates.
        hv_BBoxCol1New = hv_BBoxCol1;
        hv_BBoxCol2New = hv_BBoxCol2;
        hv_BBoxRow1New = hv_BBoxRow1;
        hv_BBoxRow2New = hv_BBoxRow2;
        hv_BBoxLabelNew = hv_BBoxLabel;
      }
      else
      {
        throw HException("Unsupported parameter value for 'domain_handling'");
      }
      //
      //Rescale the bounding boxes.
      //
      //Get required images width and height.
      //
      //Only rescale bounding boxes if the required image dimensions are not the raw dimensions.
      if (0 != (HTuple(int(hv_ImageHeight!=hv_HeightRaw)).TupleOr(int(hv_ImageWidth!=hv_WidthRaw))))
      {
        //Calculate rescaling factor.
        hv_FactorResampleWidth = (hv_ImageWidth.TupleReal())/hv_WidthRaw;
        hv_FactorResampleHeight = (hv_ImageHeight.TupleReal())/hv_HeightRaw;
        //Rescale the bounding box coordinates.
        //As we use XLD-coordinates we temporarily move the boxes by (.5,.5) for rescaling.
        //Doing so, the center of the XLD-coordinate system (-0.5,-0.5) is used
        //for scaling, hence the scaling is performed w.r.t. the pixel coordinate system.
        hv_BBoxCol1New = ((hv_BBoxCol1New+.5)*hv_FactorResampleWidth)-.5;
        hv_BBoxCol2New = ((hv_BBoxCol2New+.5)*hv_FactorResampleWidth)-.5;
        hv_BBoxRow1New = ((hv_BBoxRow1New+.5)*hv_FactorResampleHeight)-.5;
        hv_BBoxRow2New = ((hv_BBoxRow2New+.5)*hv_FactorResampleHeight)-.5;
        //
      }
      //
      //Make a final check and remove bounding boxes that have zero area.
      if (0 != (int((hv_BBoxRow1New.TupleLength())>0)))
      {
        hv_MaskDelete = (hv_BBoxRow1New.TupleGreaterEqualElem(hv_BBoxRow2New)).TupleOr(hv_BBoxCol1New.TupleGreaterEqualElem(hv_BBoxCol2New));
        hv_BBoxCol1New = hv_BBoxCol1New.TupleSelectMask(1-hv_MaskDelete);
        hv_BBoxCol2New = hv_BBoxCol2New.TupleSelectMask(1-hv_MaskDelete);
        hv_BBoxRow1New = hv_BBoxRow1New.TupleSelectMask(1-hv_MaskDelete);
        hv_BBoxRow2New = hv_BBoxRow2New.TupleSelectMask(1-hv_MaskDelete);
        hv_BBoxLabelNew = hv_BBoxLabelNew.TupleSelectMask(1-hv_MaskDelete);
      }
      //
      //Set new bounding box coordinates in the dictionary.
      SetDictTuple(hv_DLSample, "bbox_col1", hv_BBoxCol1New);
      SetDictTuple(hv_DLSample, "bbox_col2", hv_BBoxCol2New);
      SetDictTuple(hv_DLSample, "bbox_row1", hv_BBoxRow1New);
      SetDictTuple(hv_DLSample, "bbox_row2", hv_BBoxRow2New);
      SetDictTuple(hv_DLSample, "bbox_label_id", hv_BBoxLabelNew);
      //
      return;
    }

    // Chapter: Deep Learning / Object Detection
    // Short Description: This procedure preprocesses the bounding boxes of type 'rectangle2' for a given sample. 
    void preprocess_dl_model_bbox_rect2 (HObject ho_ImageRaw, HTuple hv_DLSample, HTuple hv_DLPreprocessParam)
    {

      // Local iconic variables
      HObject  ho_DomainRaw, ho_Rectangle2XLD, ho_Rectangle2XLDSheared;

      // Local control variables
      HTuple  hv_ImageWidth, hv_ImageHeight, hv_DomainHandling;
      HTuple  hv_IgnoreDirection, hv_ClassIDsNoOrientation, hv_KeyExists;
      HTuple  hv_BBoxRow, hv_BBoxCol, hv_BBoxLength1, hv_BBoxLength2;
      HTuple  hv_BBoxPhi, hv_BBoxLabel, hv_Exception, hv_ImageId;
      HTuple  hv_ExceptionMessage, hv_BoxesInvalid, hv_RowDomain1;
      HTuple  hv_ColumnDomain1, hv_RowDomain2, hv_ColumnDomain2;
      HTuple  hv_WidthRaw, hv_HeightRaw, hv_MaskDelete, hv_MaskNewBbox;
      HTuple  hv_BBoxRowNew, hv_BBoxColNew, hv_BBoxLength1New;
      HTuple  hv_BBoxLength2New, hv_BBoxPhiNew, hv_BBoxLabelNew;
      HTuple  hv_ClassIDsNoOrientationIndices, hv_Index, hv_ClassIDsNoOrientationIndicesTmp;
      HTuple  hv_DirectionLength1Row, hv_DirectionLength1Col;
      HTuple  hv_DirectionLength2Row, hv_DirectionLength2Col;
      HTuple  hv_Corner1Row, hv_Corner1Col, hv_Corner2Row, hv_Corner2Col;
      HTuple  hv_FactorResampleWidth, hv_FactorResampleHeight;
      HTuple  hv_BBoxCol1, hv_BBoxCol1New, hv_BBoxCol2, hv_BBoxCol2New;
      HTuple  hv_BBoxCol3, hv_BBoxCol3New, hv_BBoxCol4, hv_BBoxCol4New;
      HTuple  hv_BBoxRow1, hv_BBoxRow1New, hv_BBoxRow2, hv_BBoxRow2New;
      HTuple  hv_BBoxRow3, hv_BBoxRow3New, hv_BBoxRow4, hv_BBoxRow4New;
      HTuple  hv_HomMat2DIdentity, hv_HomMat2DScale, hv_BBoxPhiTmp;
      HTuple  hv_PhiDelta, hv_PhiDeltaNegativeIndices, hv_IndicesRot90;
      HTuple  hv_IndicesRot180, hv_IndicesRot270, hv_SwapIndices;
      HTuple  hv_Tmp, hv_BBoxPhiNewIndices, hv_PhiThreshold, hv_PhiToCorrect;
      HTuple  hv_NumCorrections, hv__;

      //This procedure preprocesses the bounding boxes of type 'rectangle2' for a given sample.
      //
      check_dl_preprocess_param(hv_DLPreprocessParam);
      //
      //Get preprocess parameters.
      GetDictTuple(hv_DLPreprocessParam, "image_width", &hv_ImageWidth);
      GetDictTuple(hv_DLPreprocessParam, "image_height", &hv_ImageHeight);
      GetDictTuple(hv_DLPreprocessParam, "domain_handling", &hv_DomainHandling);
      //The keys 'ignore_direction' and 'class_ids_no_orientation' are optional.
      hv_IgnoreDirection = 0;
      hv_ClassIDsNoOrientation = HTuple();
      GetDictParam(hv_DLPreprocessParam, "key_exists", (HTuple("ignore_direction").Append("class_ids_no_orientation")), 
          &hv_KeyExists);
      if (0 != (HTuple(hv_KeyExists[0])))
      {
        GetDictTuple(hv_DLPreprocessParam, "ignore_direction", &hv_IgnoreDirection);
        if (0 != (int(hv_IgnoreDirection==HTuple("true"))))
        {
          hv_IgnoreDirection = 1;
        }
        else if (0 != (int(hv_IgnoreDirection==HTuple("false"))))
        {
          hv_IgnoreDirection = 0;
        }
      }
      if (0 != (HTuple(hv_KeyExists[1])))
      {
        GetDictTuple(hv_DLPreprocessParam, "class_ids_no_orientation", &hv_ClassIDsNoOrientation);
      }
      //
      //Get bounding box coordinates and labels.
      try
      {
        GetDictTuple(hv_DLSample, "bbox_row", &hv_BBoxRow);
        GetDictTuple(hv_DLSample, "bbox_col", &hv_BBoxCol);
        GetDictTuple(hv_DLSample, "bbox_length1", &hv_BBoxLength1);
        GetDictTuple(hv_DLSample, "bbox_length2", &hv_BBoxLength2);
        GetDictTuple(hv_DLSample, "bbox_phi", &hv_BBoxPhi);
        GetDictTuple(hv_DLSample, "bbox_label_id", &hv_BBoxLabel);
      }
      // catch (Exception) 
      catch (HException &HDevExpDefaultException)
      {
        HDevExpDefaultException.ToHTuple(&hv_Exception);
        GetDictTuple(hv_DLSample, "image_id", &hv_ImageId);
        if (0 != (int(HTuple(hv_Exception[0])==1302)))
        {
          hv_ExceptionMessage = "A bounding box coordinate key is missing.";
        }
        else
        {
          hv_ExceptionMessage = ((const HTuple&)hv_Exception)[2];
        }
        throw HException((("An error has occurred during preprocessing image_id "+hv_ImageId)+" when getting bounding box coordinates : ")+hv_ExceptionMessage);
      }
      //
      //Check that there are no invalid boxes.
      if (0 != (int((hv_BBoxRow.TupleLength())>0)))
      {
        hv_BoxesInvalid = ((hv_BBoxLength1.TupleEqualElem(0)).TupleSum())+((hv_BBoxLength2.TupleEqualElem(0)).TupleSum());
        if (0 != (int(hv_BoxesInvalid>0)))
        {
          GetDictTuple(hv_DLSample, "image_id", &hv_ImageId);
          throw HException(("An error has occurred during preprocessing image_id "+hv_ImageId)+HTuple(": Sample contains at least one bounding box with zero-area, i.e. bbox_length1 == 0 or bbox_length2 == 0!"));
        }
      }
      else
      {
        //There are no bounding boxes, hence nothing to do.
        return;
      }
      //
      //If the domain is cropped, crop bounding boxes.
      if (0 != (int(hv_DomainHandling==HTuple("crop_domain"))))
      {
        //
        //Get domain.
        GetDomain(ho_ImageRaw, &ho_DomainRaw);
        //
        //Set the size of the raw image to the domain extensions.
        SmallestRectangle1(ho_DomainRaw, &hv_RowDomain1, &hv_ColumnDomain1, &hv_RowDomain2, 
            &hv_ColumnDomain2);
        hv_WidthRaw = (hv_ColumnDomain2-hv_ColumnDomain1)+1;
        hv_HeightRaw = (hv_RowDomain2-hv_RowDomain1)+1;
        //
        //Crop the bounding boxes.
        //Remove the boxes with center outside of the domain.
        hv_MaskDelete = HTuple(HTuple((hv_BBoxRow.TupleLessElem(hv_RowDomain1)).TupleOr(hv_BBoxCol.TupleLessElem(hv_ColumnDomain1))).TupleOr(hv_BBoxRow.TupleGreaterElem(hv_RowDomain2))).TupleOr(hv_BBoxCol.TupleGreaterElem(hv_ColumnDomain2));
        hv_MaskNewBbox = 1-hv_MaskDelete;
        //Store the preprocessed bounding box entries.
        hv_BBoxRowNew = (hv_BBoxRow.TupleSelectMask(hv_MaskNewBbox))-hv_RowDomain1;
        hv_BBoxColNew = (hv_BBoxCol.TupleSelectMask(hv_MaskNewBbox))-hv_ColumnDomain1;
        hv_BBoxLength1New = hv_BBoxLength1.TupleSelectMask(hv_MaskNewBbox);
        hv_BBoxLength2New = hv_BBoxLength2.TupleSelectMask(hv_MaskNewBbox);
        hv_BBoxPhiNew = hv_BBoxPhi.TupleSelectMask(hv_MaskNewBbox);
        hv_BBoxLabelNew = hv_BBoxLabel.TupleSelectMask(hv_MaskNewBbox);
        //
      }
      else if (0 != (int(hv_DomainHandling==HTuple("full_domain"))))
      {
        //If the entire image is used, set the variables accordingly.
        //Get the original size.
        GetImageSize(ho_ImageRaw, &hv_WidthRaw, &hv_HeightRaw);
        //Set new coordinates to input coordinates.
        hv_BBoxRowNew = hv_BBoxRow;
        hv_BBoxColNew = hv_BBoxCol;
        hv_BBoxLength1New = hv_BBoxLength1;
        hv_BBoxLength2New = hv_BBoxLength2;
        hv_BBoxPhiNew = hv_BBoxPhi;
        hv_BBoxLabelNew = hv_BBoxLabel;
      }
      else
      {
        throw HException("Unsupported parameter value for 'domain_handling'");
      }
      //
      //Generate smallest enclosing axis-aligned bounding box for classes in ClassIDsNoOrientation.
      hv_ClassIDsNoOrientationIndices = HTuple();
      {
      HTuple end_val94 = (hv_ClassIDsNoOrientation.TupleLength())-1;
      HTuple step_val94 = 1;
      for (hv_Index=0; hv_Index.Continue(end_val94, step_val94); hv_Index += step_val94)
      {
        hv_ClassIDsNoOrientationIndicesTmp = (hv_BBoxLabelNew.TupleEqualElem(HTuple(hv_ClassIDsNoOrientation[hv_Index]))).TupleFind(1);
        if (0 != (int(hv_ClassIDsNoOrientationIndicesTmp!=-1)))
        {
          hv_ClassIDsNoOrientationIndices = hv_ClassIDsNoOrientationIndices.TupleConcat(hv_ClassIDsNoOrientationIndicesTmp);
        }
      }
      }
      if (0 != (int((hv_ClassIDsNoOrientationIndices.TupleLength())>0)))
      {
        //Calculate length1 and length2 using position of corners.
        hv_DirectionLength1Row = -(HTuple(hv_BBoxPhiNew[hv_ClassIDsNoOrientationIndices]).TupleSin());
        hv_DirectionLength1Col = HTuple(hv_BBoxPhiNew[hv_ClassIDsNoOrientationIndices]).TupleCos();
        hv_DirectionLength2Row = -hv_DirectionLength1Col;
        hv_DirectionLength2Col = hv_DirectionLength1Row;
        hv_Corner1Row = (HTuple(hv_BBoxLength1New[hv_ClassIDsNoOrientationIndices])*hv_DirectionLength1Row)+(HTuple(hv_BBoxLength2New[hv_ClassIDsNoOrientationIndices])*hv_DirectionLength2Row);
        hv_Corner1Col = (HTuple(hv_BBoxLength1New[hv_ClassIDsNoOrientationIndices])*hv_DirectionLength1Col)+(HTuple(hv_BBoxLength2New[hv_ClassIDsNoOrientationIndices])*hv_DirectionLength2Col);
        hv_Corner2Row = (HTuple(hv_BBoxLength1New[hv_ClassIDsNoOrientationIndices])*hv_DirectionLength1Row)-(HTuple(hv_BBoxLength2New[hv_ClassIDsNoOrientationIndices])*hv_DirectionLength2Row);
        hv_Corner2Col = (HTuple(hv_BBoxLength1New[hv_ClassIDsNoOrientationIndices])*hv_DirectionLength1Col)-(HTuple(hv_BBoxLength2New[hv_ClassIDsNoOrientationIndices])*hv_DirectionLength2Col);
        //
        hv_BBoxPhiNew[hv_ClassIDsNoOrientationIndices] = 0.0;
        hv_BBoxLength1New[hv_ClassIDsNoOrientationIndices] = (hv_Corner1Col.TupleAbs()).TupleMax2(hv_Corner2Col.TupleAbs());
        hv_BBoxLength2New[hv_ClassIDsNoOrientationIndices] = (hv_Corner1Row.TupleAbs()).TupleMax2(hv_Corner2Row.TupleAbs());
      }
      //
      //Rescale bounding boxes.
      //
      //Get required images width and height.
      //
      //Only rescale bounding boxes if the required image dimensions are not the raw dimensions.
      if (0 != (HTuple(int(hv_ImageHeight!=hv_HeightRaw)).TupleOr(int(hv_ImageWidth!=hv_WidthRaw))))
      {
        //Calculate rescaling factor.
        hv_FactorResampleWidth = (hv_ImageWidth.TupleReal())/hv_WidthRaw;
        hv_FactorResampleHeight = (hv_ImageHeight.TupleReal())/hv_HeightRaw;
        if (0 != (int(hv_FactorResampleHeight!=hv_FactorResampleWidth)))
        {
          //In order to preserve the correct orientation we have to transform the points individually.
          //Get the coordinates of the four corner points.
          convert_rect2_5to8param(hv_BBoxRowNew, hv_BBoxColNew, hv_BBoxLength1New, hv_BBoxLength2New, 
              hv_BBoxPhiNew, &hv_BBoxRow1, &hv_BBoxCol1, &hv_BBoxRow2, &hv_BBoxCol2, 
              &hv_BBoxRow3, &hv_BBoxCol3, &hv_BBoxRow4, &hv_BBoxCol4);
          //
          //Rescale the coordinates.
          hv_BBoxCol1New = hv_BBoxCol1*hv_FactorResampleWidth;
          hv_BBoxCol2New = hv_BBoxCol2*hv_FactorResampleWidth;
          hv_BBoxCol3New = hv_BBoxCol3*hv_FactorResampleWidth;
          hv_BBoxCol4New = hv_BBoxCol4*hv_FactorResampleWidth;
          hv_BBoxRow1New = hv_BBoxRow1*hv_FactorResampleHeight;
          hv_BBoxRow2New = hv_BBoxRow2*hv_FactorResampleHeight;
          hv_BBoxRow3New = hv_BBoxRow3*hv_FactorResampleHeight;
          hv_BBoxRow4New = hv_BBoxRow4*hv_FactorResampleHeight;
          //
          //The rectangles will get sheared, that is why new rectangles have to be found.
          //Generate homography to scale rectangles.
          HomMat2dIdentity(&hv_HomMat2DIdentity);
          HomMat2dScale(hv_HomMat2DIdentity, hv_FactorResampleHeight, hv_FactorResampleWidth, 
              0, 0, &hv_HomMat2DScale);
          //Generate XLD contours for the rectangles.
          GenRectangle2ContourXld(&ho_Rectangle2XLD, hv_BBoxRowNew, hv_BBoxColNew, hv_BBoxPhiNew, 
              hv_BBoxLength1New, hv_BBoxLength2New);
          //Scale the XLD contours --> results in sheared regions.
          AffineTransContourXld(ho_Rectangle2XLD, &ho_Rectangle2XLDSheared, hv_HomMat2DScale);
          SmallestRectangle2Xld(ho_Rectangle2XLDSheared, &hv_BBoxRowNew, &hv_BBoxColNew, 
              &hv_BBoxPhiNew, &hv_BBoxLength1New, &hv_BBoxLength2New);
          //
          //smallest_rectangle2_xld might change the orientation of the bounding box.
          //Hence, take the orientation that is closest to the one obtained out of the 4 corner points.
          convert_rect2_8to5param(hv_BBoxRow1New, hv_BBoxCol1New, hv_BBoxRow2New, hv_BBoxCol2New, 
              hv_BBoxRow3New, hv_BBoxCol3New, hv_BBoxRow4New, hv_BBoxCol4New, hv_IgnoreDirection, 
              &hv__, &hv__, &hv__, &hv__, &hv_BBoxPhiTmp);
          hv_PhiDelta = (hv_BBoxPhiTmp-hv_BBoxPhiNew).TupleFmod(HTuple(360).TupleRad());
          //Guarantee that angles are positive.
          hv_PhiDeltaNegativeIndices = (hv_PhiDelta.TupleLessElem(0.0)).TupleFind(1);
          if (0 != (int(hv_PhiDeltaNegativeIndices!=-1)))
          {
            hv_PhiDelta[hv_PhiDeltaNegativeIndices] = HTuple(hv_PhiDelta[hv_PhiDeltaNegativeIndices])+(HTuple(360).TupleRad());
          }
          hv_IndicesRot90 = HTuple((hv_PhiDelta.TupleGreaterElem(HTuple(45).TupleRad())).TupleAnd(hv_PhiDelta.TupleLessEqualElem(HTuple(135).TupleRad()))).TupleFind(1);
          hv_IndicesRot180 = HTuple((hv_PhiDelta.TupleGreaterElem(HTuple(135).TupleRad())).TupleAnd(hv_PhiDelta.TupleLessEqualElem(HTuple(225).TupleRad()))).TupleFind(1);
          hv_IndicesRot270 = HTuple((hv_PhiDelta.TupleGreaterElem(HTuple(225).TupleRad())).TupleAnd(hv_PhiDelta.TupleLessEqualElem(HTuple(315).TupleRad()))).TupleFind(1);
          hv_SwapIndices = HTuple();
          if (0 != (int(hv_IndicesRot90!=-1)))
          {
            hv_BBoxPhiNew[hv_IndicesRot90] = HTuple(hv_BBoxPhiNew[hv_IndicesRot90])+(HTuple(90).TupleRad());
            hv_SwapIndices = hv_SwapIndices.TupleConcat(hv_IndicesRot90);
          }
          if (0 != (int(hv_IndicesRot180!=-1)))
          {
            hv_BBoxPhiNew[hv_IndicesRot180] = HTuple(hv_BBoxPhiNew[hv_IndicesRot180])+(HTuple(180).TupleRad());
          }
          if (0 != (int(hv_IndicesRot270!=-1)))
          {
            hv_BBoxPhiNew[hv_IndicesRot270] = HTuple(hv_BBoxPhiNew[hv_IndicesRot270])+(HTuple(270).TupleRad());
            hv_SwapIndices = hv_SwapIndices.TupleConcat(hv_IndicesRot270);
          }
          if (0 != (int(hv_SwapIndices!=HTuple())))
          {
            hv_Tmp = HTuple(hv_BBoxLength1New[hv_SwapIndices]);
            hv_BBoxLength1New[hv_SwapIndices] = HTuple(hv_BBoxLength2New[hv_SwapIndices]);
            hv_BBoxLength2New[hv_SwapIndices] = hv_Tmp;
          }
          //Change angles such that they lie in the range (-180°, 180°].
          hv_BBoxPhiNewIndices = (hv_BBoxPhiNew.TupleGreaterElem(HTuple(180).TupleRad())).TupleFind(1);
          if (0 != (int(hv_BBoxPhiNewIndices!=-1)))
          {
            hv_BBoxPhiNew[hv_BBoxPhiNewIndices] = HTuple(hv_BBoxPhiNew[hv_BBoxPhiNewIndices])-(HTuple(360).TupleRad());
          }
          //
        }
        else
        {
          hv_BBoxColNew = hv_BBoxColNew*hv_FactorResampleWidth;
          hv_BBoxRowNew = hv_BBoxRowNew*hv_FactorResampleWidth;
          hv_BBoxLength1New = hv_BBoxLength1New*hv_FactorResampleWidth;
          hv_BBoxLength2New = hv_BBoxLength2New*hv_FactorResampleWidth;
          //Phi stays the same.
        }
        //
      }
      //
      //Adapt the bounding box angles such that they are within the correct range,
      //which is (-180°,180°] for 'ignore_direction'==false and (-90°,90°] else.
      hv_PhiThreshold = (HTuple(180).TupleRad())-(hv_IgnoreDirection*(HTuple(90).TupleRad()));
      hv_PhiDelta = 2*hv_PhiThreshold;
      //Correct angles that are too large.
      hv_PhiToCorrect = (hv_BBoxPhiNew.TupleGreaterElem(hv_PhiThreshold)).TupleFind(1);
      if (0 != (HTuple(int(hv_PhiToCorrect!=-1)).TupleAnd(int(hv_PhiToCorrect!=HTuple()))))
      {
        hv_NumCorrections = (((HTuple(hv_BBoxPhiNew[hv_PhiToCorrect])-hv_PhiThreshold)/hv_PhiDelta).TupleInt())+1;
        hv_BBoxPhiNew[hv_PhiToCorrect] = HTuple(hv_BBoxPhiNew[hv_PhiToCorrect])-(hv_NumCorrections*hv_PhiDelta);
      }
      //Correct angles that are too small.
      hv_PhiToCorrect = (hv_BBoxPhiNew.TupleLessEqualElem(-hv_PhiThreshold)).TupleFind(1);
      if (0 != (HTuple(int(hv_PhiToCorrect!=-1)).TupleAnd(int(hv_PhiToCorrect!=HTuple()))))
      {
        hv_NumCorrections = ((((HTuple(hv_BBoxPhiNew[hv_PhiToCorrect])+hv_PhiThreshold).TupleAbs())/hv_PhiDelta).TupleInt())+1;
        hv_BBoxPhiNew[hv_PhiToCorrect] = HTuple(hv_BBoxPhiNew[hv_PhiToCorrect])+(hv_NumCorrections*hv_PhiDelta);
      }
      //
      //Check that there are no invalid boxes.
      if (0 != (int((hv_BBoxRowNew.TupleLength())>0)))
      {
        hv_BoxesInvalid = ((hv_BBoxLength1New.TupleEqualElem(0)).TupleSum())+((hv_BBoxLength2New.TupleEqualElem(0)).TupleSum());
        if (0 != (int(hv_BoxesInvalid>0)))
        {
          GetDictTuple(hv_DLSample, "image_id", &hv_ImageId);
          throw HException(("An error has occurred during preprocessing image_id "+hv_ImageId)+HTuple(": Sample contains at least one box with zero-area, i.e. bbox_length1 == 0 or bbox_length2 == 0!"));
        }
      }
      SetDictTuple(hv_DLSample, "bbox_row", hv_BBoxRowNew);
      SetDictTuple(hv_DLSample, "bbox_col", hv_BBoxColNew);
      SetDictTuple(hv_DLSample, "bbox_length1", hv_BBoxLength1New);
      SetDictTuple(hv_DLSample, "bbox_length2", hv_BBoxLength2New);
      SetDictTuple(hv_DLSample, "bbox_phi", hv_BBoxPhiNew);
      SetDictTuple(hv_DLSample, "bbox_label_id", hv_BBoxLabelNew);
      //
      return;

    }

    // Chapter: Deep Learning / Model
    // Short Description: Preprocess images for deep-learning-based training and inference. 
    void preprocess_dl_model_images (HObject ho_Images, HObject *ho_ImagesPreprocessed, 
        HTuple hv_DLPreprocessParam)
    {

      // Local iconic variables
      HObject  ho_ImagesScaled, ho_ImageSelected, ho_ImageScaled;
      HObject  ho_Channel, ho_ChannelScaled, ho_ThreeChannelImage;
      HObject  ho_SingleChannelImage;

      // Local control variables
      HTuple  hv_ImageWidth, hv_ImageHeight, hv_ImageNumChannels;
      HTuple  hv_ImageRangeMin, hv_ImageRangeMax, hv_DomainHandling;
      HTuple  hv_NormalizationType, hv_ModelType, hv_NumImages;
      HTuple  hv_Type, hv_NumMatches, hv_InputNumChannels, hv_OutputNumChannels;
      HTuple  hv_NumChannels1, hv_NumChannels3, hv_AreInputNumChannels1;
      HTuple  hv_AreInputNumChannels3, hv_AreInputNumChannels1Or3;
      HTuple  hv_ValidNumChannels, hv_ValidNumChannelsText, hv_ImageIndex;
      HTuple  hv_NumChannels, hv_ChannelIndex, hv_Min, hv_Max;
      HTuple  hv_Range, hv_Scale, hv_Shift, hv_MeanValues, hv_DeviationValues;
      HTuple  hv_UseDefaultNormalizationValues, hv_Exception;
      HTuple  hv_Indices, hv_RescaleRange, hv_CurrentNumChannels;
      HTuple  hv_DiffNumChannelsIndices, hv_Index, hv_DiffNumChannelsIndex;

      //
      //This procedure preprocesses the provided Images according to the parameters in
      //the dictionary DLPreprocessParam. Note that depending on the images, additional
      //preprocessing steps might be beneficial.
      //
      //Validate the preprocessing parameters.
      check_dl_preprocess_param(hv_DLPreprocessParam);
      //
      //Get the preprocessing parameters.
      GetDictTuple(hv_DLPreprocessParam, "image_width", &hv_ImageWidth);
      GetDictTuple(hv_DLPreprocessParam, "image_height", &hv_ImageHeight);
      GetDictTuple(hv_DLPreprocessParam, "image_num_channels", &hv_ImageNumChannels);
      GetDictTuple(hv_DLPreprocessParam, "image_range_min", &hv_ImageRangeMin);
      GetDictTuple(hv_DLPreprocessParam, "image_range_max", &hv_ImageRangeMax);
      GetDictTuple(hv_DLPreprocessParam, "domain_handling", &hv_DomainHandling);
      GetDictTuple(hv_DLPreprocessParam, "normalization_type", &hv_NormalizationType);
      GetDictTuple(hv_DLPreprocessParam, "model_type", &hv_ModelType);
      //
      //Validate the type of the input images.
      CountObj(ho_Images, &hv_NumImages);
      if (0 != (int(hv_NumImages==0)))
      {
        throw HException("Please provide some images to preprocess.");
      }
      GetImageType(ho_Images, &hv_Type);
      TupleRegexpTest(hv_Type, "byte|int|real", &hv_NumMatches);
      if (0 != (int(hv_NumMatches!=hv_NumImages)))
      {
        throw HException(HTuple("Please provide only images of type 'byte', 'int1', 'int2', 'uint2', 'int4', or 'real'."));
      }
      //
      //Validate the number channels of the input images.
      CountChannels(ho_Images, &hv_InputNumChannels);
      hv_OutputNumChannels = HTuple(hv_NumImages,hv_ImageNumChannels);
      //Only for 'image_num_channels' 1 and 3 combinations of 1- and 3-channel images are allowed.
      if (0 != (HTuple(int(hv_ImageNumChannels==1)).TupleOr(int(hv_ImageNumChannels==3))))
      {
        hv_NumChannels1 = HTuple(hv_NumImages,1);
        hv_NumChannels3 = HTuple(hv_NumImages,3);
        hv_AreInputNumChannels1 = hv_InputNumChannels.TupleEqualElem(hv_NumChannels1);
        hv_AreInputNumChannels3 = hv_InputNumChannels.TupleEqualElem(hv_NumChannels3);
        hv_AreInputNumChannels1Or3 = hv_AreInputNumChannels1+hv_AreInputNumChannels3;
        hv_ValidNumChannels = int(hv_AreInputNumChannels1Or3==hv_NumChannels1);
        hv_ValidNumChannelsText = "Valid numbers of channels for the specified model are 1 or 3.";
      }
      else
      {
        hv_ValidNumChannels = int(hv_InputNumChannels==hv_OutputNumChannels);
        hv_ValidNumChannelsText = ("Valid number of channels for the specified model is "+hv_ImageNumChannels)+".";
      }
      if (0 != (hv_ValidNumChannels.TupleNot()))
      {
        throw HException("Please provide images with a valid number of channels. "+hv_ValidNumChannelsText);
      }
      //Preprocess the images.
      //
      //Apply the domain to the images.
      if (0 != (int(hv_DomainHandling==HTuple("full_domain"))))
      {
        FullDomain(ho_Images, &ho_Images);
      }
      else if (0 != (int(hv_DomainHandling==HTuple("crop_domain"))))
      {
        CropDomain(ho_Images, &ho_Images);
      }
      else if (0 != (HTuple(int(hv_DomainHandling==HTuple("keep_domain"))).TupleAnd(int(hv_ModelType==HTuple("anomaly_detection")))))
      {
        //Anomaly detection models accept the additional option 'keep_domain'.
      }
      else
      {
        throw HException("Unsupported parameter value for 'domain_handling'.");
      }
      //
      //Convert the images to real and zoom the images.
      ConvertImageType(ho_Images, &ho_Images, "real");
      ZoomImageSize(ho_Images, &ho_Images, hv_ImageWidth, hv_ImageHeight, "constant");
      //
      if (0 != (int(hv_NormalizationType==HTuple("all_channels"))))
      {
        //Scale for each image the gray values of all channels to ImageRangeMin-ImageRangeMax.
        GenEmptyObj(&ho_ImagesScaled);
        {
        HTuple end_val68 = hv_NumImages;
        HTuple step_val68 = 1;
        for (hv_ImageIndex=1; hv_ImageIndex.Continue(end_val68, step_val68); hv_ImageIndex += step_val68)
        {
          SelectObj(ho_Images, &ho_ImageSelected, hv_ImageIndex);
          CountChannels(ho_ImageSelected, &hv_NumChannels);
          GenEmptyObj(&ho_ImageScaled);
          {
          HTuple end_val72 = hv_NumChannels;
          HTuple step_val72 = 1;
          for (hv_ChannelIndex=1; hv_ChannelIndex.Continue(end_val72, step_val72); hv_ChannelIndex += step_val72)
          {
            AccessChannel(ho_ImageSelected, &ho_Channel, hv_ChannelIndex);
            MinMaxGray(ho_Channel, ho_Channel, 0, &hv_Min, &hv_Max, &hv_Range);
            if (0 != (int((hv_Max-hv_Min)==0)))
            {
              hv_Scale = 1;
            }
            else
            {
              hv_Scale = (hv_ImageRangeMax-hv_ImageRangeMin)/(hv_Max-hv_Min);
            }
            hv_Shift = ((-hv_Scale)*hv_Min)+hv_ImageRangeMin;
            ScaleImage(ho_Channel, &ho_ChannelScaled, hv_Scale, hv_Shift);
            AppendChannel(ho_ImageScaled, ho_ChannelScaled, &ho_ImageScaled);
          }
          }
          ConcatObj(ho_ImagesScaled, ho_ImageScaled, &ho_ImagesScaled);
        }
        }
        ho_Images = ho_ImagesScaled;
      }
      else if (0 != (int(hv_NormalizationType==HTuple("first_channel"))))
      {
        //Scale for each image the gray values of first channel to ImageRangeMin-ImageRangeMax.
        GenEmptyObj(&ho_ImagesScaled);
        {
        HTuple end_val90 = hv_NumImages;
        HTuple step_val90 = 1;
        for (hv_ImageIndex=1; hv_ImageIndex.Continue(end_val90, step_val90); hv_ImageIndex += step_val90)
        {
          SelectObj(ho_Images, &ho_ImageSelected, hv_ImageIndex);
          MinMaxGray(ho_ImageSelected, ho_ImageSelected, 0, &hv_Min, &hv_Max, &hv_Range);
          if (0 != (int((hv_Max-hv_Min)==0)))
          {
            hv_Scale = 1;
          }
          else
          {
            hv_Scale = (hv_ImageRangeMax-hv_ImageRangeMin)/(hv_Max-hv_Min);
          }
          hv_Shift = ((-hv_Scale)*hv_Min)+hv_ImageRangeMin;
          ScaleImage(ho_ImageSelected, &ho_ImageSelected, hv_Scale, hv_Shift);
          ConcatObj(ho_ImagesScaled, ho_ImageSelected, &ho_ImagesScaled);
        }
        }
        ho_Images = ho_ImagesScaled;
      }
      else if (0 != (int(hv_NormalizationType==HTuple("constant_values"))))
      {
        //Scale for each image the gray values of all channels to the corresponding channel DeviationValues[].
        try
        {
          GetDictTuple(hv_DLPreprocessParam, "mean_values_normalization", &hv_MeanValues);
          GetDictTuple(hv_DLPreprocessParam, "deviation_values_normalization", &hv_DeviationValues);
          hv_UseDefaultNormalizationValues = 0;
        }
        // catch (Exception) 
        catch (HException &HDevExpDefaultException)
        {
          HDevExpDefaultException.ToHTuple(&hv_Exception);
          hv_MeanValues.Clear();
          hv_MeanValues[0] = 123.675;
          hv_MeanValues[1] = 116.28;
          hv_MeanValues[2] = 103.53;
          hv_DeviationValues.Clear();
          hv_DeviationValues[0] = 58.395;
          hv_DeviationValues[1] = 57.12;
          hv_DeviationValues[2] = 57.375;
          hv_UseDefaultNormalizationValues = 1;
        }
        GenEmptyObj(&ho_ImagesScaled);
        {
        HTuple end_val115 = hv_NumImages;
        HTuple step_val115 = 1;
        for (hv_ImageIndex=1; hv_ImageIndex.Continue(end_val115, step_val115); hv_ImageIndex += step_val115)
        {
          SelectObj(ho_Images, &ho_ImageSelected, hv_ImageIndex);
          CountChannels(ho_ImageSelected, &hv_NumChannels);
          //Ensure that the number of channels is equal |DeviationValues| and |MeanValues|
          if (0 != hv_UseDefaultNormalizationValues)
          {
            if (0 != (int(hv_NumChannels==1)))
            {
              Compose3(ho_ImageSelected, ho_ImageSelected, ho_ImageSelected, &ho_ImageSelected
                  );
              CountChannels(ho_ImageSelected, &hv_NumChannels);
            }
            else if (0 != (int(hv_NumChannels!=3)))
            {
              throw HException("Using default values for normalization type 'constant_values' is allowed only for 1- and 3-channel images.");
            }
          }
          if (0 != (HTuple(int((hv_MeanValues.TupleLength())!=hv_NumChannels)).TupleOr(int((hv_DeviationValues.TupleLength())!=hv_NumChannels))))
          {
            throw HException("The length of mean and deviation values for normalization type 'constant_values' have to be the same size as the number of channels of the image.");
          }
          GenEmptyObj(&ho_ImageScaled);
          {
          HTuple end_val131 = hv_NumChannels;
          HTuple step_val131 = 1;
          for (hv_ChannelIndex=1; hv_ChannelIndex.Continue(end_val131, step_val131); hv_ChannelIndex += step_val131)
          {
            AccessChannel(ho_ImageSelected, &ho_Channel, hv_ChannelIndex);
            hv_Scale = 1.0/HTuple(hv_DeviationValues[hv_ChannelIndex-1]);
            hv_Shift = (-hv_Scale)*HTuple(hv_MeanValues[hv_ChannelIndex-1]);
            ScaleImage(ho_Channel, &ho_ChannelScaled, hv_Scale, hv_Shift);
            AppendChannel(ho_ImageScaled, ho_ChannelScaled, &ho_ImageScaled);
          }
          }
          ConcatObj(ho_ImagesScaled, ho_ImageScaled, &ho_ImagesScaled);
        }
        }
        ho_Images = ho_ImagesScaled;
      }
      else if (0 != (int(hv_NormalizationType==HTuple("none"))))
      {
        TupleFind(hv_Type, "byte", &hv_Indices);
        if (0 != (int(hv_Indices!=-1)))
        {
          //Shift the gray values from [0-255] to the expected range for byte images.
          hv_RescaleRange = (hv_ImageRangeMax-hv_ImageRangeMin)/255.0;
          SelectObj(ho_Images, &ho_ImageSelected, hv_Indices+1);
          ScaleImage(ho_ImageSelected, &ho_ImageSelected, hv_RescaleRange, hv_ImageRangeMin);
          ReplaceObj(ho_Images, ho_ImageSelected, &ho_Images, hv_Indices+1);
        }
      }
      else if (0 != (int(hv_NormalizationType!=HTuple("none"))))
      {
        throw HException("Unsupported parameter value for 'normalization_type'");
      }
      //
      //Ensure that the number of channels of the resulting images is consistent with the
      //number of channels of the given model. The only exceptions that are adapted below
      //are combinations of 1- and 3-channel images if ImageNumChannels is either 1 or 3.
      if (0 != (HTuple(int(hv_ImageNumChannels==1)).TupleOr(int(hv_ImageNumChannels==3))))
      {
        CountChannels(ho_Images, &hv_CurrentNumChannels);
        TupleFind(hv_CurrentNumChannels.TupleNotEqualElem(hv_OutputNumChannels), 1, &hv_DiffNumChannelsIndices);
        if (0 != (int(hv_DiffNumChannelsIndices!=-1)))
        {
          {
          HTuple end_val161 = (hv_DiffNumChannelsIndices.TupleLength())-1;
          HTuple step_val161 = 1;
          for (hv_Index=0; hv_Index.Continue(end_val161, step_val161); hv_Index += step_val161)
          {
            hv_DiffNumChannelsIndex = HTuple(hv_DiffNumChannelsIndices[hv_Index]);
            hv_ImageIndex = hv_DiffNumChannelsIndex+1;
            hv_NumChannels = HTuple(hv_CurrentNumChannels[hv_ImageIndex-1]);
            SelectObj(ho_Images, &ho_ImageSelected, hv_ImageIndex);
            if (0 != (HTuple(int(hv_NumChannels==1)).TupleAnd(int(hv_ImageNumChannels==3))))
            {
              //Conversion from 1- to 3-channel image required
              Compose3(ho_ImageSelected, ho_ImageSelected, ho_ImageSelected, &ho_ThreeChannelImage
                  );
              ReplaceObj(ho_Images, ho_ThreeChannelImage, &ho_Images, hv_ImageIndex);
            }
            else if (0 != (HTuple(int(hv_NumChannels==3)).TupleAnd(int(hv_ImageNumChannels==1))))
            {
              //Conversion from 3- to 1-channel image required
              Rgb1ToGray(ho_ImageSelected, &ho_SingleChannelImage);
              ReplaceObj(ho_Images, ho_SingleChannelImage, &ho_Images, hv_ImageIndex);
            }
            else
            {
              throw HException(((("Unexpected error adapting the number of channels. The number of channels of the resulting image is "+hv_NumChannels)+HTuple(", but the number of channels of the model is "))+hv_ImageNumChannels)+".");
            }
          }
          }
        }
      }
      //
      //Write preprocessed images to output variable.
      (*ho_ImagesPreprocessed) = ho_Images;
      //
      return;
    }

    // Chapter: Deep Learning / Semantic Segmentation
    // Short Description: Preprocess segmentation and weight images for deep-learning-based segmentation training and inference. 
    void preprocess_dl_model_segmentations (HObject ho_ImagesRaw, HObject ho_Segmentations, 
        HObject *ho_SegmentationsPreprocessed, HTuple hv_DLPreprocessParam)
    {

      // Local iconic variables
      HObject  ho_Domain, ho_SelectedSeg, ho_SelectedDomain;

      // Local control variables
      HTuple  hv_NumberImages, hv_NumberSegmentations;
      HTuple  hv_Width, hv_Height, hv_WidthSeg, hv_HeightSeg;
      HTuple  hv_DLModelType, hv_ImageWidth, hv_ImageHeight, hv_ImageNumChannels;
      HTuple  hv_ImageRangeMin, hv_ImageRangeMax, hv_DomainHandling;
      HTuple  hv_SetBackgroundID, hv_ClassesToBackground, hv_IgnoreClassIDs;
      HTuple  hv_IsInt, hv_IndexImage, hv_ImageWidthRaw, hv_ImageHeightRaw;
      HTuple  hv_EqualWidth, hv_EqualHeight, hv_Type, hv_EqualReal;

      //
      //This procedure preprocesses the segmentation or weight images
      //given by Segmentations so that they can be handled by
      //train_dl_model_batch and apply_dl_model.
      //
      //Check input data.
      //Examine number of images.
      CountObj(ho_ImagesRaw, &hv_NumberImages);
      CountObj(ho_Segmentations, &hv_NumberSegmentations);
      if (0 != (int(hv_NumberImages!=hv_NumberSegmentations)))
      {
        throw HException("Equal number of images given in ImagesRaw and Segmentations required");
      }
      //Size of images.
      GetImageSize(ho_ImagesRaw, &hv_Width, &hv_Height);
      GetImageSize(ho_Segmentations, &hv_WidthSeg, &hv_HeightSeg);
      if (0 != (HTuple(int(hv_Width!=hv_WidthSeg)).TupleOr(int(hv_Height!=hv_HeightSeg))))
      {
        throw HException("Equal size of the images given in ImagesRaw and Segmentations required.");
      }
      //Check the validity of the preprocessing parameters.
      check_dl_preprocess_param(hv_DLPreprocessParam);
      //
      //Get the relevant preprocessing parameters.
      GetDictTuple(hv_DLPreprocessParam, "model_type", &hv_DLModelType);
      GetDictTuple(hv_DLPreprocessParam, "image_width", &hv_ImageWidth);
      GetDictTuple(hv_DLPreprocessParam, "image_height", &hv_ImageHeight);
      GetDictTuple(hv_DLPreprocessParam, "image_num_channels", &hv_ImageNumChannels);
      GetDictTuple(hv_DLPreprocessParam, "image_range_min", &hv_ImageRangeMin);
      GetDictTuple(hv_DLPreprocessParam, "image_range_max", &hv_ImageRangeMax);
      GetDictTuple(hv_DLPreprocessParam, "domain_handling", &hv_DomainHandling);
      //Segmentation specific parameters.
      GetDictTuple(hv_DLPreprocessParam, "set_background_id", &hv_SetBackgroundID);
      GetDictTuple(hv_DLPreprocessParam, "class_ids_background", &hv_ClassesToBackground);
      GetDictTuple(hv_DLPreprocessParam, "ignore_class_ids", &hv_IgnoreClassIDs);
      //
      //Check the input parameter for setting the background ID.
      if (0 != (int(hv_SetBackgroundID!=HTuple())))
      {
        //Check that the model is a segmentation model.
        if (0 != (int(hv_DLModelType!=HTuple("segmentation"))))
        {
          throw HException("Setting class IDs to background is only implemented for segmentation.");
        }
        //Check the background ID.
        TupleIsIntElem(hv_SetBackgroundID, &hv_IsInt);
        if (0 != (int((hv_SetBackgroundID.TupleLength())!=1)))
        {
          throw HException("Only one class_id as 'set_background_id' allowed.");
        }
        else if (0 != (hv_IsInt.TupleNot()))
        {
          //Given class_id has to be of type int.
          throw HException("The class_id given as 'set_background_id' has to be of type int.");
        }
        //Check the values of ClassesToBackground.
        if (0 != (int((hv_ClassesToBackground.TupleLength())==0)))
        {
          //Check that the given classes are of length > 0.
          throw HException(HTuple("If 'set_background_id' is given, 'class_ids_background' must at least contain this class ID."));
        }
        else if (0 != (int((hv_ClassesToBackground.TupleIntersection(hv_IgnoreClassIDs))!=HTuple())))
        {
          //Check that class_ids_background is not included in the ignore_class_ids of the DLModel.
          throw HException("The given 'class_ids_background' must not be included in the 'ignore_class_ids' of the model.");
        }
      }
      //
      //Domain handling of the image to be preprocessed.
      //
      if (0 != (int(hv_DomainHandling==HTuple("full_domain"))))
      {
        FullDomain(ho_Segmentations, &ho_Segmentations);
      }
      else if (0 != (int(hv_DomainHandling==HTuple("crop_domain"))))
      {
        //If the domain should be cropped the domain has to be transferred
        //from the raw image to the segmentation image.
        GetDomain(ho_ImagesRaw, &ho_Domain);
        {
        HTuple end_val66 = hv_NumberImages;
        HTuple step_val66 = 1;
        for (hv_IndexImage=1; hv_IndexImage.Continue(end_val66, step_val66); hv_IndexImage += step_val66)
        {
          SelectObj(ho_Segmentations, &ho_SelectedSeg, hv_IndexImage);
          SelectObj(ho_Domain, &ho_SelectedDomain, hv_IndexImage);
          ChangeDomain(ho_SelectedSeg, ho_SelectedDomain, &ho_SelectedSeg);
          ReplaceObj(ho_Segmentations, ho_SelectedSeg, &ho_Segmentations, hv_IndexImage);
        }
        }
        CropDomain(ho_Segmentations, &ho_Segmentations);
      }
      else
      {
        throw HException("Unsupported parameter value for 'domain_handling'");
      }
      //
      //Preprocess the segmentation images.
      //
      //Set all background classes to the given background class ID.
      if (0 != (int(hv_SetBackgroundID!=HTuple())))
      {
        reassign_pixel_values(ho_Segmentations, &ho_Segmentations, hv_ClassesToBackground, 
            hv_SetBackgroundID);
      }
      //
      //Zoom images only if they have a different size than the specified size.
      GetImageSize(ho_Segmentations, &hv_ImageWidthRaw, &hv_ImageHeightRaw);
      hv_EqualWidth = hv_ImageWidth.TupleEqualElem(hv_ImageWidthRaw);
      hv_EqualHeight = hv_ImageHeight.TupleEqualElem(hv_ImageHeightRaw);
      if (0 != (HTuple(int((hv_EqualWidth.TupleMin())==0)).TupleOr(int((hv_EqualHeight.TupleMin())==0))))
      {
        ZoomImageSize(ho_Segmentations, &ho_Segmentations, hv_ImageWidth, hv_ImageHeight, 
            "nearest_neighbor");
      }
      //
      //Check the type of the input images
      //and convert if necessary.
      GetImageType(ho_Segmentations, &hv_Type);
      hv_EqualReal = hv_Type.TupleEqualElem("real");
      //
      if (0 != (int((hv_EqualReal.TupleMin())==0)))
      {
        //Convert the image type to 'real',
        //because the model expects 'real' images.
        ConvertImageType(ho_Segmentations, &ho_Segmentations, "real");
      }
      //
      //Write preprocessed Segmentations to output variable.
      (*ho_SegmentationsPreprocessed) = ho_Segmentations;
      return;
    }

    // Chapter: Deep Learning / Model
    // Short Description: Preprocess given DLSamples according to the preprocessing parameters given in DLPreprocessParam. 
    void preprocess_dl_samples (HTuple hv_DLSampleBatch, HTuple hv_DLPreprocessParam)
    {

      // Local iconic variables
      HObject  ho_ImageRaw, ho_ImagePreprocessed, ho_AnomalyImageRaw;
      HObject  ho_AnomalyImagePreprocessed, ho_SegmentationRaw;
      HObject  ho_SegmentationPreprocessed;

      // Local control variables
      HTuple  hv_ModelType, hv_SampleIndex, hv_ImageExists;
      HTuple  hv_KeysExists, hv_AnomalyParamExist, hv_Rectangle1ParamExist;
      HTuple  hv_Rectangle2ParamExist, hv_SegmentationParamExist;

      //
      //This procedure preprocesses all images of the sample dictionaries in the tuple DLSampleBatch.
      //The images are preprocessed according to the parameters provided in DLPreprocessParam.
      //
      //Check the validity of the preprocessing parameters.
      check_dl_preprocess_param(hv_DLPreprocessParam);
      GetDictTuple(hv_DLPreprocessParam, "model_type", &hv_ModelType);
      //
      //Preprocess the sample entries.
      //
      {
      HTuple end_val10 = (hv_DLSampleBatch.TupleLength())-1;
      HTuple step_val10 = 1;
      for (hv_SampleIndex=0; hv_SampleIndex.Continue(end_val10, step_val10); hv_SampleIndex += step_val10)
      {
        //
        //Check the existence of the sample keys.
        GetDictParam(HTuple(hv_DLSampleBatch[hv_SampleIndex]), "key_exists", "image", 
            &hv_ImageExists);
        //
        //Preprocess the images.
        if (0 != hv_ImageExists)
        {
          //
          //Get the image.
          GetDictObject(&ho_ImageRaw, HTuple(hv_DLSampleBatch[hv_SampleIndex]), "image");
          //
          //Preprocess the image.
          preprocess_dl_model_images(ho_ImageRaw, &ho_ImagePreprocessed, hv_DLPreprocessParam);
          //
          //Replace the image in the dictionary.
          SetDictObject(ho_ImagePreprocessed, HTuple(hv_DLSampleBatch[hv_SampleIndex]), 
              "image");
          //
          //Check existence of model specific sample keys:
          //- bbox_row1 for 'rectangle1'
          //- bbox_phi for 'rectangle2'
          //- segmentation_image for 'semantic segmentation'
          GetDictParam(HTuple(hv_DLSampleBatch[hv_SampleIndex]), "key_exists", (((HTuple("anomaly_ground_truth").Append("bbox_row1")).Append("bbox_phi")).Append("segmentation_image")), 
              &hv_KeysExists);
          hv_AnomalyParamExist = ((const HTuple&)hv_KeysExists)[0];
          hv_Rectangle1ParamExist = ((const HTuple&)hv_KeysExists)[1];
          hv_Rectangle2ParamExist = ((const HTuple&)hv_KeysExists)[2];
          hv_SegmentationParamExist = ((const HTuple&)hv_KeysExists)[3];
          //
          //Preprocess the anomaly ground truth if present.
          if (0 != hv_AnomalyParamExist)
          {
            //
            //Get the anomaly image.
            GetDictObject(&ho_AnomalyImageRaw, HTuple(hv_DLSampleBatch[hv_SampleIndex]), 
                "anomaly_ground_truth");
            //
            //Preprocess the anomaly image.
            preprocess_dl_model_anomaly(ho_AnomalyImageRaw, &ho_AnomalyImagePreprocessed, 
                hv_DLPreprocessParam);
            //
            //Set preprocessed anomaly image.
            SetDictObject(ho_AnomalyImagePreprocessed, HTuple(hv_DLSampleBatch[hv_SampleIndex]), 
                "anomaly_ground_truth");
          }
          //
          //Preprocess depending on the model type.
          //If bounding boxes are given, rescale them as well.
          if (0 != hv_Rectangle1ParamExist)
          {
            //
            //Preprocess the bounding boxes of type 'rectangle1'.
            preprocess_dl_model_bbox_rect1(ho_ImageRaw, HTuple(hv_DLSampleBatch[hv_SampleIndex]), 
                hv_DLPreprocessParam);
          }
          else if (0 != hv_Rectangle2ParamExist)
          {
            //
            //Preprocess the bounding boxes of type 'rectangle2'.
            preprocess_dl_model_bbox_rect2(ho_ImageRaw, HTuple(hv_DLSampleBatch[hv_SampleIndex]), 
                hv_DLPreprocessParam);
          }
          //
          //Preprocess the segmentation image if present.
          if (0 != hv_SegmentationParamExist)
          {
            //
            //Get the segmentation image.
            GetDictObject(&ho_SegmentationRaw, HTuple(hv_DLSampleBatch[hv_SampleIndex]), 
                "segmentation_image");
            //
            //Preprocess the segmentation image.
            preprocess_dl_model_segmentations(ho_ImageRaw, ho_SegmentationRaw, &ho_SegmentationPreprocessed, 
                hv_DLPreprocessParam);
            //
            //Set preprocessed segmentation image.
            SetDictObject(ho_SegmentationPreprocessed, HTuple(hv_DLSampleBatch[hv_SampleIndex]), 
                "segmentation_image");
          }
        }
        else
        {
          throw HException((HTuple("All samples processed need to include an image, but the sample with index ")+hv_SampleIndex)+" does not.");
        }
      }
      }
      //
      return;
    }

    // Chapter: Image / Manipulation
    // Short Description: Changes a value of ValuesToChange in Image to NewValue. 
    void reassign_pixel_values (HObject ho_Image, HObject *ho_ImageOut, HTuple hv_ValuesToChange, 
        HTuple hv_NewValue)
    {

      // Local iconic variables
      HObject  ho_RegionToChange, ho_RegionClass;

      // Local control variables
      HTuple  hv_IndexReset;

      //
      //This procedure sets all pixels of Image
      //with the values given in ValuesToChange to the given value NewValue.
      //
      GenEmptyRegion(&ho_RegionToChange);
      {
      HTuple end_val5 = (hv_ValuesToChange.TupleLength())-1;
      HTuple step_val5 = 1;
      for (hv_IndexReset=0; hv_IndexReset.Continue(end_val5, step_val5); hv_IndexReset += step_val5)
      {
        Threshold(ho_Image, &ho_RegionClass, HTuple(hv_ValuesToChange[hv_IndexReset]), 
            HTuple(hv_ValuesToChange[hv_IndexReset]));
        Union2(ho_RegionToChange, ho_RegionClass, &ho_RegionToChange);
      }
      }
      OverpaintRegion(ho_Image, ho_RegionToChange, hv_NewValue, "fill");
      (*ho_ImageOut) = ho_Image;
      return;
    }

    // Chapter: Deep Learning / Model
    // Short Description: This procedure replaces legacy preprocessing parameters. 
    void replace_legacy_preprocessing_parameters (HTuple hv_DLPreprocessParam)
    {

      // Local iconic variables

      // Local control variables
      HTuple  hv_Exception, hv_NormalizationTypeExists;
      HTuple  hv_NormalizationType, hv_LegacyNormalizationKeyExists;
      HTuple  hv_ContrastNormalization;

      //
      //This procedure adapts the dictionary DLPreprocessParam
      //if a legacy preprocessing parameter is set.
      //
      //Map legacy value set to new parameter.
      hv_Exception = 0;
      try
      {
        GetDictParam(hv_DLPreprocessParam, "key_exists", "normalization_type", &hv_NormalizationTypeExists);
        //
        if (0 != hv_NormalizationTypeExists)
        {
          GetDictTuple(hv_DLPreprocessParam, "normalization_type", &hv_NormalizationType);
          if (0 != (int(hv_NormalizationType==HTuple("true"))))
          {
            hv_NormalizationType = "first_channel";
          }
          else if (0 != (int(hv_NormalizationType==HTuple("false"))))
          {
            hv_NormalizationType = "none";
          }
          SetDictTuple(hv_DLPreprocessParam, "normalization_type", hv_NormalizationType);
        }
      }
      // catch (Exception) 
      catch (HException &HDevExpDefaultException)
      {
        HDevExpDefaultException.ToHTuple(&hv_Exception);
      }
      //
      //Map legacy parameter to new parameter and corresponding value.
      hv_Exception = 0;
      try
      {
        GetDictParam(hv_DLPreprocessParam, "key_exists", "contrast_normalization", &hv_LegacyNormalizationKeyExists);
        if (0 != hv_LegacyNormalizationKeyExists)
        {
          GetDictTuple(hv_DLPreprocessParam, "contrast_normalization", &hv_ContrastNormalization);
          //Replace 'contrast_normalization' by 'normalization_type'.
          if (0 != (int(hv_ContrastNormalization==HTuple("false"))))
          {
            SetDictTuple(hv_DLPreprocessParam, "normalization_type", "none");
          }
          else if (0 != (int(hv_ContrastNormalization==HTuple("true"))))
          {
            SetDictTuple(hv_DLPreprocessParam, "normalization_type", "first_channel");
          }
          RemoveDictKey(hv_DLPreprocessParam, "contrast_normalization");
        }
      }
      // catch (Exception) 
      catch (HException &HDevExpDefaultException)
      {
        HDevExpDefaultException.ToHTuple(&hv_Exception);
      }
      return;
    }

    // Chapter: Graphics / Text
    // Short Description: Set font independent of OS 
    void set_display_font (HTuple hv_WindowHandle, HTuple hv_Size, HTuple hv_Font, HTuple hv_Bold, 
        HTuple hv_Slant)
    {

      // Local iconic variables

      // Local control variables
      HTuple  hv_OS, hv_Fonts, hv_Style, hv_Exception;
      HTuple  hv_AvailableFonts, hv_Fdx, hv_Indices;

      //This procedure sets the text font of the current window with
      //the specified attributes.
      //
      //Input parameters:
      //WindowHandle: The graphics window for which the font will be set
      //Size: The font size. If Size=-1, the default of 16 is used.
      //Bold: If set to 'true', a bold font is used
      //Slant: If set to 'true', a slanted font is used
      //
      GetSystem("operating_system", &hv_OS);
      if (0 != (HTuple(int(hv_Size==HTuple())).TupleOr(int(hv_Size==-1))))
      {
        hv_Size = 16;
      }
      if (0 != (int((hv_OS.TupleSubstr(0,2))==HTuple("Win"))))
      {
        //Restore previous behaviour
        hv_Size = (1.13677*hv_Size).TupleInt();
      }
      else
      {
        hv_Size = hv_Size.TupleInt();
      }
      if (0 != (int(hv_Font==HTuple("Courier"))))
      {
        hv_Fonts.Clear();
        hv_Fonts[0] = "Courier";
        hv_Fonts[1] = "Courier 10 Pitch";
        hv_Fonts[2] = "Courier New";
        hv_Fonts[3] = "CourierNew";
        hv_Fonts[4] = "Liberation Mono";
      }
      else if (0 != (int(hv_Font==HTuple("mono"))))
      {
        hv_Fonts.Clear();
        hv_Fonts[0] = "Consolas";
        hv_Fonts[1] = "Menlo";
        hv_Fonts[2] = "Courier";
        hv_Fonts[3] = "Courier 10 Pitch";
        hv_Fonts[4] = "FreeMono";
        hv_Fonts[5] = "Liberation Mono";
      }
      else if (0 != (int(hv_Font==HTuple("sans"))))
      {
        hv_Fonts.Clear();
        hv_Fonts[0] = "Luxi Sans";
        hv_Fonts[1] = "DejaVu Sans";
        hv_Fonts[2] = "FreeSans";
        hv_Fonts[3] = "Arial";
        hv_Fonts[4] = "Liberation Sans";
      }
      else if (0 != (int(hv_Font==HTuple("serif"))))
      {
        hv_Fonts.Clear();
        hv_Fonts[0] = "Times New Roman";
        hv_Fonts[1] = "Luxi Serif";
        hv_Fonts[2] = "DejaVu Serif";
        hv_Fonts[3] = "FreeSerif";
        hv_Fonts[4] = "Utopia";
        hv_Fonts[5] = "Liberation Serif";
      }
      else
      {
        hv_Fonts = hv_Font;
      }
      hv_Style = "";
      if (0 != (int(hv_Bold==HTuple("true"))))
      {
        hv_Style += HTuple("Bold");
      }
      else if (0 != (int(hv_Bold!=HTuple("false"))))
      {
        hv_Exception = "Wrong value of control parameter Bold";
        throw HException(hv_Exception);
      }
      if (0 != (int(hv_Slant==HTuple("true"))))
      {
        hv_Style += HTuple("Italic");
      }
      else if (0 != (int(hv_Slant!=HTuple("false"))))
      {
        hv_Exception = "Wrong value of control parameter Slant";
        throw HException(hv_Exception);
      }
      if (0 != (int(hv_Style==HTuple(""))))
      {
        hv_Style = "Normal";
      }
      QueryFont(hv_WindowHandle, &hv_AvailableFonts);
      hv_Font = "";
      {
      HTuple end_val48 = (hv_Fonts.TupleLength())-1;
      HTuple step_val48 = 1;
      for (hv_Fdx=0; hv_Fdx.Continue(end_val48, step_val48); hv_Fdx += step_val48)
      {
        hv_Indices = hv_AvailableFonts.TupleFind(HTuple(hv_Fonts[hv_Fdx]));
        if (0 != (int((hv_Indices.TupleLength())>0)))
        {
          if (0 != (int(HTuple(hv_Indices[0])>=0)))
          {
            hv_Font = HTuple(hv_Fonts[hv_Fdx]);
            break;
          }
        }
      }
      }
      if (0 != (int(hv_Font==HTuple(""))))
      {
        throw HException("Wrong value of control parameter Font");
      }
      hv_Font = (((hv_Font+"-")+hv_Style)+"-")+hv_Size;
      SetFont(hv_WindowHandle, hv_Font);
      return;
    }
}