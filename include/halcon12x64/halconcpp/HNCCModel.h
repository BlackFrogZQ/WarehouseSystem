/***********************************************************
 * File generated by the HALCON-Compiler hcomp version 20.11
 * Usage: Interface to C++
 *
 * Software by: MVTec Software GmbH, www.mvtec.com
 ***********************************************************/


#ifndef HCPP_HNCCMODEL
#define HCPP_HNCCMODEL

namespace HalconCpp
{

// Represents an instance of an NCC model for matching.
class LIntExport HNCCModel : public HHandle
{

public:

  // Create an uninitialized instance
  HNCCModel():HHandle() {}

  // Copy constructor
  HNCCModel(const HNCCModel& source) : HHandle(source) {}

  // Copy constructor
  HNCCModel(const HHandle& handle);

  // Create HNCCModel from handle, taking ownership
  explicit HNCCModel(Hlong handle);

  bool operator==(const HHandle& obj) const
  {
    return HHandleBase::operator==(obj);
  }

  bool operator!=(const HHandle& obj) const
  {
    return HHandleBase::operator!=(obj);
  }

protected:

  // Verify matching semantic type ('ncc_model')!
  virtual void AssertType(Hphandle handle) const;

public:

  // Deep copy of all data represented by this object instance
  HNCCModel Clone() const;



/*****************************************************************************
 * Operator-based class constructors
 *****************************************************************************/

  // read_ncc_model: Read an NCC model from a file.
  explicit HNCCModel(const HString& FileName);

  // read_ncc_model: Read an NCC model from a file.
  explicit HNCCModel(const char* FileName);

#ifdef _WIN32
  // read_ncc_model: Read an NCC model from a file.
  explicit HNCCModel(const wchar_t* FileName);
#endif

  // create_ncc_model: Prepare an NCC model for matching.
  explicit HNCCModel(const HImage& Template, const HTuple& NumLevels, double AngleStart, double AngleExtent, const HTuple& AngleStep, const HString& Metric);

  // create_ncc_model: Prepare an NCC model for matching.
  explicit HNCCModel(const HImage& Template, Hlong NumLevels, double AngleStart, double AngleExtent, double AngleStep, const HString& Metric);

  // create_ncc_model: Prepare an NCC model for matching.
  explicit HNCCModel(const HImage& Template, Hlong NumLevels, double AngleStart, double AngleExtent, double AngleStep, const char* Metric);

#ifdef _WIN32
  // create_ncc_model: Prepare an NCC model for matching.
  explicit HNCCModel(const HImage& Template, Hlong NumLevels, double AngleStart, double AngleExtent, double AngleStep, const wchar_t* Metric);
#endif




  /***************************************************************************
   * Operators                                                               *
   ***************************************************************************/

  // Free the memory of an NCC model.
  void ClearNccModel() const;

  // Deserialize an NCC model.
  void DeserializeNccModel(const HSerializedItem& SerializedItemHandle);

  // Serialize an NCC model.
  HSerializedItem SerializeNccModel() const;

  // Read an NCC model from a file.
  void ReadNccModel(const HString& FileName);

  // Read an NCC model from a file.
  void ReadNccModel(const char* FileName);

#ifdef _WIN32
  // Read an NCC model from a file.
  void ReadNccModel(const wchar_t* FileName);
#endif

  // Write an NCC model to a file.
  void WriteNccModel(const HString& FileName) const;

  // Write an NCC model to a file.
  void WriteNccModel(const char* FileName) const;

#ifdef _WIN32
  // Write an NCC model to a file.
  void WriteNccModel(const wchar_t* FileName) const;
#endif

  // Determine the parameters of an NCC model.
  static HTuple DetermineNccModelParams(const HImage& Template, const HTuple& NumLevels, double AngleStart, double AngleExtent, const HString& Metric, const HTuple& Parameters, HTuple* ParameterValue);

  // Determine the parameters of an NCC model.
  static HTuple DetermineNccModelParams(const HImage& Template, Hlong NumLevels, double AngleStart, double AngleExtent, const HString& Metric, const HString& Parameters, HTuple* ParameterValue);

  // Determine the parameters of an NCC model.
  static HTuple DetermineNccModelParams(const HImage& Template, Hlong NumLevels, double AngleStart, double AngleExtent, const char* Metric, const char* Parameters, HTuple* ParameterValue);

#ifdef _WIN32
  // Determine the parameters of an NCC model.
  static HTuple DetermineNccModelParams(const HImage& Template, Hlong NumLevels, double AngleStart, double AngleExtent, const wchar_t* Metric, const wchar_t* Parameters, HTuple* ParameterValue);
#endif

  // Return the parameters of an NCC model.
  Hlong GetNccModelParams(double* AngleStart, double* AngleExtent, double* AngleStep, HString* Metric) const;

  // Return the origin (reference point) of an NCC model.
  void GetNccModelOrigin(double* Row, double* Column) const;

  // Set the origin (reference point) of an NCC model.
  void SetNccModelOrigin(double Row, double Column) const;

  // Find the best matches of an NCC model in an image.
  void FindNccModel(const HImage& Image, double AngleStart, double AngleExtent, double MinScore, Hlong NumMatches, double MaxOverlap, const HString& SubPixel, const HTuple& NumLevels, HTuple* Row, HTuple* Column, HTuple* Angle, HTuple* Score) const;

  // Find the best matches of an NCC model in an image.
  void FindNccModel(const HImage& Image, double AngleStart, double AngleExtent, double MinScore, Hlong NumMatches, double MaxOverlap, const HString& SubPixel, Hlong NumLevels, HTuple* Row, HTuple* Column, HTuple* Angle, HTuple* Score) const;

  // Find the best matches of an NCC model in an image.
  void FindNccModel(const HImage& Image, double AngleStart, double AngleExtent, double MinScore, Hlong NumMatches, double MaxOverlap, const char* SubPixel, Hlong NumLevels, HTuple* Row, HTuple* Column, HTuple* Angle, HTuple* Score) const;

#ifdef _WIN32
  // Find the best matches of an NCC model in an image.
  void FindNccModel(const HImage& Image, double AngleStart, double AngleExtent, double MinScore, Hlong NumMatches, double MaxOverlap, const wchar_t* SubPixel, Hlong NumLevels, HTuple* Row, HTuple* Column, HTuple* Angle, HTuple* Score) const;
#endif

  // Set selected parameters of the NCC model.
  void SetNccModelParam(const HTuple& GenParamName, const HTuple& GenParamValue) const;

  // Prepare an NCC model for matching.
  void CreateNccModel(const HImage& Template, const HTuple& NumLevels, double AngleStart, double AngleExtent, const HTuple& AngleStep, const HString& Metric);

  // Prepare an NCC model for matching.
  void CreateNccModel(const HImage& Template, Hlong NumLevels, double AngleStart, double AngleExtent, double AngleStep, const HString& Metric);

  // Prepare an NCC model for matching.
  void CreateNccModel(const HImage& Template, Hlong NumLevels, double AngleStart, double AngleExtent, double AngleStep, const char* Metric);

#ifdef _WIN32
  // Prepare an NCC model for matching.
  void CreateNccModel(const HImage& Template, Hlong NumLevels, double AngleStart, double AngleExtent, double AngleStep, const wchar_t* Metric);
#endif

  // Find the best matches of multiple NCC models.
  static void FindNccModels(const HImage& Image, const HNCCModelArray& ModelIDs, const HTuple& AngleStart, const HTuple& AngleExtent, const HTuple& MinScore, const HTuple& NumMatches, const HTuple& MaxOverlap, const HTuple& SubPixel, const HTuple& NumLevels, HTuple* Row, HTuple* Column, HTuple* Angle, HTuple* Score, HTuple* Model);

  // Find the best matches of multiple NCC models.
  void FindNccModels(const HImage& Image, double AngleStart, double AngleExtent, double MinScore, Hlong NumMatches, double MaxOverlap, const HString& SubPixel, Hlong NumLevels, HTuple* Row, HTuple* Column, HTuple* Angle, HTuple* Score, HTuple* Model) const;

  // Find the best matches of multiple NCC models.
  void FindNccModels(const HImage& Image, double AngleStart, double AngleExtent, double MinScore, Hlong NumMatches, double MaxOverlap, const char* SubPixel, Hlong NumLevels, HTuple* Row, HTuple* Column, HTuple* Angle, HTuple* Score, HTuple* Model) const;

#ifdef _WIN32
  // Find the best matches of multiple NCC models.
  void FindNccModels(const HImage& Image, double AngleStart, double AngleExtent, double MinScore, Hlong NumMatches, double MaxOverlap, const wchar_t* SubPixel, Hlong NumLevels, HTuple* Row, HTuple* Column, HTuple* Angle, HTuple* Score, HTuple* Model) const;
#endif

  // Return the region used to create an NCC model.
  HRegion GetNccModelRegion() const;

};

// forward declarations and types for internal array implementation

template<class T> class HSmartPtr;
template<class T> class HHandleBaseArrayRef;

typedef HHandleBaseArrayRef<HNCCModel> HNCCModelArrayRef;
typedef HSmartPtr< HNCCModelArrayRef > HNCCModelArrayPtr;


// Represents multiple tool instances
class LIntExport HNCCModelArray : public HHandleBaseArray
{

public:

  // Create empty array
  HNCCModelArray();

  // Create array from native array of tool instances
  HNCCModelArray(HNCCModel* classes, Hlong length);

  // Copy constructor
  HNCCModelArray(const HNCCModelArray &tool_array);

  // Destructor
  virtual ~HNCCModelArray();

  // Assignment operator
  HNCCModelArray &operator=(const HNCCModelArray &tool_array);

  // Clears array and all tool instances
  virtual void Clear();

  // Get array of native tool instances
  const HNCCModel* Tools() const;

  // Get number of tools
  virtual Hlong Length() const;

  // Create tool array from tuple of handles
  virtual void SetFromTuple(const HTuple& handles);

  // Get tuple of handles for tool array
  virtual HTuple ConvertToTuple() const;

protected:

// Smart pointer to internal data container
   HNCCModelArrayPtr *mArrayPtr;
};

}

#endif