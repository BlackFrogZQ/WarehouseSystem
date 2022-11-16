/***********************************************************
 * File generated by the HALCON-Compiler hcomp version 20.11
 * Usage: Interface to C++
 *
 * Software by: MVTec Software GmbH, www.mvtec.com
 ***********************************************************/


#ifndef HCPP_HDATACODE2D
#define HCPP_HDATACODE2D

namespace HalconCpp
{

// Represents an instance of a 2D data code reader.
class LIntExport HDataCode2D : public HHandle
{

public:

  // Create an uninitialized instance
  HDataCode2D():HHandle() {}

  // Copy constructor
  HDataCode2D(const HDataCode2D& source) : HHandle(source) {}

  // Copy constructor
  HDataCode2D(const HHandle& handle);

  // Create HDataCode2D from handle, taking ownership
  explicit HDataCode2D(Hlong handle);

  bool operator==(const HHandle& obj) const
  {
    return HHandleBase::operator==(obj);
  }

  bool operator!=(const HHandle& obj) const
  {
    return HHandleBase::operator!=(obj);
  }

protected:

  // Verify matching semantic type ('datacode_2d')!
  virtual void AssertType(Hphandle handle) const;

public:

  // Deep copy of all data represented by this object instance
  HDataCode2D Clone() const;



/*****************************************************************************
 * Operator-based class constructors
 *****************************************************************************/

  // read_data_code_2d_model: Read a 2D data code model from a file and create a new model.
  explicit HDataCode2D(const HString& FileName);

  // read_data_code_2d_model: Read a 2D data code model from a file and create a new model.
  explicit HDataCode2D(const char* FileName);

#ifdef _WIN32
  // read_data_code_2d_model: Read a 2D data code model from a file and create a new model.
  explicit HDataCode2D(const wchar_t* FileName);
#endif

  // create_data_code_2d_model: Create a model of a 2D data code class.
  explicit HDataCode2D(const HString& SymbolType, const HTuple& GenParamName, const HTuple& GenParamValue);

  // create_data_code_2d_model: Create a model of a 2D data code class.
  explicit HDataCode2D(const HString& SymbolType, const HString& GenParamName, const HString& GenParamValue);

  // create_data_code_2d_model: Create a model of a 2D data code class.
  explicit HDataCode2D(const char* SymbolType, const char* GenParamName, const char* GenParamValue);

#ifdef _WIN32
  // create_data_code_2d_model: Create a model of a 2D data code class.
  explicit HDataCode2D(const wchar_t* SymbolType, const wchar_t* GenParamName, const wchar_t* GenParamValue);
#endif




  /***************************************************************************
   * Operators                                                               *
   ***************************************************************************/

  // Access iconic objects that were created during the search for 2D data code symbols.
  HObject GetDataCode2dObjects(const HTuple& CandidateHandle, const HString& ObjectName) const;

  // Access iconic objects that were created during the search for 2D data code symbols.
  HObject GetDataCode2dObjects(Hlong CandidateHandle, const HString& ObjectName) const;

  // Access iconic objects that were created during the search for 2D data code symbols.
  HObject GetDataCode2dObjects(Hlong CandidateHandle, const char* ObjectName) const;

#ifdef _WIN32
  // Access iconic objects that were created during the search for 2D data code symbols.
  HObject GetDataCode2dObjects(Hlong CandidateHandle, const wchar_t* ObjectName) const;
#endif

  // Get the alphanumerical results that were accumulated during the search for 2D data code symbols.
  HTuple GetDataCode2dResults(const HTuple& CandidateHandle, const HTuple& ResultNames) const;

  // Get the alphanumerical results that were accumulated during the search for 2D data code symbols.
  HTuple GetDataCode2dResults(const HString& CandidateHandle, const HString& ResultNames) const;

  // Get the alphanumerical results that were accumulated during the search for 2D data code symbols.
  HTuple GetDataCode2dResults(const char* CandidateHandle, const char* ResultNames) const;

#ifdef _WIN32
  // Get the alphanumerical results that were accumulated during the search for 2D data code symbols.
  HTuple GetDataCode2dResults(const wchar_t* CandidateHandle, const wchar_t* ResultNames) const;
#endif

  // Detect and read 2D data code symbols in an image or train the 2D data code model.
  HXLDCont FindDataCode2d(const HImage& Image, const HTuple& GenParamName, const HTuple& GenParamValue, HTuple* ResultHandles, HTuple* DecodedDataStrings) const;

  // Detect and read 2D data code symbols in an image or train the 2D data code model.
  HXLDCont FindDataCode2d(const HImage& Image, const HString& GenParamName, Hlong GenParamValue, Hlong* ResultHandles, HString* DecodedDataStrings) const;

  // Detect and read 2D data code symbols in an image or train the 2D data code model.
  HXLDCont FindDataCode2d(const HImage& Image, const char* GenParamName, Hlong GenParamValue, Hlong* ResultHandles, HString* DecodedDataStrings) const;

#ifdef _WIN32
  // Detect and read 2D data code symbols in an image or train the 2D data code model.
  HXLDCont FindDataCode2d(const HImage& Image, const wchar_t* GenParamName, Hlong GenParamValue, Hlong* ResultHandles, HString* DecodedDataStrings) const;
#endif

  // Set selected parameters of the 2D data code model.
  void SetDataCode2dParam(const HTuple& GenParamName, const HTuple& GenParamValue) const;

  // Set selected parameters of the 2D data code model.
  void SetDataCode2dParam(const HString& GenParamName, const HString& GenParamValue) const;

  // Set selected parameters of the 2D data code model.
  void SetDataCode2dParam(const char* GenParamName, const char* GenParamValue) const;

#ifdef _WIN32
  // Set selected parameters of the 2D data code model.
  void SetDataCode2dParam(const wchar_t* GenParamName, const wchar_t* GenParamValue) const;
#endif

  // Get one or several parameters that describe the 2D data code model.
  HTuple GetDataCode2dParam(const HTuple& GenParamName) const;

  // Get one or several parameters that describe the 2D data code model.
  HTuple GetDataCode2dParam(const HString& GenParamName) const;

  // Get one or several parameters that describe the 2D data code model.
  HTuple GetDataCode2dParam(const char* GenParamName) const;

#ifdef _WIN32
  // Get one or several parameters that describe the 2D data code model.
  HTuple GetDataCode2dParam(const wchar_t* GenParamName) const;
#endif

  // Get for a given 2D data code model the names of the generic parameters or objects that can be used in the other 2D data code operators.
  HTuple QueryDataCode2dParams(const HString& QueryName) const;

  // Get for a given 2D data code model the names of the generic parameters or objects that can be used in the other 2D data code operators.
  HTuple QueryDataCode2dParams(const char* QueryName) const;

#ifdef _WIN32
  // Get for a given 2D data code model the names of the generic parameters or objects that can be used in the other 2D data code operators.
  HTuple QueryDataCode2dParams(const wchar_t* QueryName) const;
#endif

  // Deserialize a serialized 2D data code model.
  void DeserializeDataCode2dModel(const HSerializedItem& SerializedItemHandle);

  // Serialize a 2D data code model.
  HSerializedItem SerializeDataCode2dModel() const;

  // Read a 2D data code model from a file and create a new model.
  void ReadDataCode2dModel(const HString& FileName);

  // Read a 2D data code model from a file and create a new model.
  void ReadDataCode2dModel(const char* FileName);

#ifdef _WIN32
  // Read a 2D data code model from a file and create a new model.
  void ReadDataCode2dModel(const wchar_t* FileName);
#endif

  // Writes a 2D data code model into a file.
  void WriteDataCode2dModel(const HString& FileName) const;

  // Writes a 2D data code model into a file.
  void WriteDataCode2dModel(const char* FileName) const;

#ifdef _WIN32
  // Writes a 2D data code model into a file.
  void WriteDataCode2dModel(const wchar_t* FileName) const;
#endif

  // Delete a 2D data code model and free the allocated memory.
  void ClearDataCode2dModel() const;

  // Create a model of a 2D data code class.
  void CreateDataCode2dModel(const HString& SymbolType, const HTuple& GenParamName, const HTuple& GenParamValue);

  // Create a model of a 2D data code class.
  void CreateDataCode2dModel(const HString& SymbolType, const HString& GenParamName, const HString& GenParamValue);

  // Create a model of a 2D data code class.
  void CreateDataCode2dModel(const char* SymbolType, const char* GenParamName, const char* GenParamValue);

#ifdef _WIN32
  // Create a model of a 2D data code class.
  void CreateDataCode2dModel(const wchar_t* SymbolType, const wchar_t* GenParamName, const wchar_t* GenParamValue);
#endif

};

// forward declarations and types for internal array implementation

template<class T> class HSmartPtr;
template<class T> class HHandleBaseArrayRef;

typedef HHandleBaseArrayRef<HDataCode2D> HDataCode2DArrayRef;
typedef HSmartPtr< HDataCode2DArrayRef > HDataCode2DArrayPtr;


// Represents multiple tool instances
class LIntExport HDataCode2DArray : public HHandleBaseArray
{

public:

  // Create empty array
  HDataCode2DArray();

  // Create array from native array of tool instances
  HDataCode2DArray(HDataCode2D* classes, Hlong length);

  // Copy constructor
  HDataCode2DArray(const HDataCode2DArray &tool_array);

  // Destructor
  virtual ~HDataCode2DArray();

  // Assignment operator
  HDataCode2DArray &operator=(const HDataCode2DArray &tool_array);

  // Clears array and all tool instances
  virtual void Clear();

  // Get array of native tool instances
  const HDataCode2D* Tools() const;

  // Get number of tools
  virtual Hlong Length() const;

  // Create tool array from tuple of handles
  virtual void SetFromTuple(const HTuple& handles);

  // Get tuple of handles for tool array
  virtual HTuple ConvertToTuple() const;

protected:

// Smart pointer to internal data container
   HDataCode2DArrayPtr *mArrayPtr;
};

}

#endif
