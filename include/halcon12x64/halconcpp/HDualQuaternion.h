/***********************************************************
 * File generated by the HALCON-Compiler hcomp version 20.11
 * Usage: Interface to C++
 *
 * Software by: MVTec Software GmbH, www.mvtec.com
 ***********************************************************/


#ifndef HCPP_HDUALQUATERNION
#define HCPP_HDUALQUATERNION

namespace HalconCpp
{

// Represents a dual quaternion.
class LIntExport HDualQuaternion : public HDataBase
{

public:

  // Create an uninitialized instance
  HDualQuaternion():HDataBase() {}

  // Copy constructor
  HDualQuaternion(const HDualQuaternion& source) : HDataBase(source) {}

  // Constructor from HTuple
  explicit HDualQuaternion(const HTuple& tuple) : HDataBase(tuple) {}

  // Deep copy of all data represented by this object instance
  HDualQuaternion Clone() const;



/*****************************************************************************
 * Operator-based class constructors
 *****************************************************************************/

  // pose_to_dual_quat: Convert a 3D pose to a unit dual quaternion.
  explicit HDualQuaternion(const HPose& Pose);

  // screw_to_dual_quat: Convert a screw into a dual quaternion.
  explicit HDualQuaternion(const HString& ScrewFormat, const HTuple& AxisDirectionX, const HTuple& AxisDirectionY, const HTuple& AxisDirectionZ, const HTuple& AxisMomentOrPointX, const HTuple& AxisMomentOrPointY, const HTuple& AxisMomentOrPointZ, const HTuple& Rotation, const HTuple& Translation);

  // screw_to_dual_quat: Convert a screw into a dual quaternion.
  explicit HDualQuaternion(const HString& ScrewFormat, double AxisDirectionX, double AxisDirectionY, double AxisDirectionZ, double AxisMomentOrPointX, double AxisMomentOrPointY, double AxisMomentOrPointZ, double Rotation, double Translation);

  // screw_to_dual_quat: Convert a screw into a dual quaternion.
  explicit HDualQuaternion(const char* ScrewFormat, double AxisDirectionX, double AxisDirectionY, double AxisDirectionZ, double AxisMomentOrPointX, double AxisMomentOrPointY, double AxisMomentOrPointZ, double Rotation, double Translation);

#ifdef _WIN32
  // screw_to_dual_quat: Convert a screw into a dual quaternion.
  explicit HDualQuaternion(const wchar_t* ScrewFormat, double AxisDirectionX, double AxisDirectionY, double AxisDirectionZ, double AxisMomentOrPointX, double AxisMomentOrPointY, double AxisMomentOrPointZ, double Rotation, double Translation);
#endif


/*****************************************************************************
 * Operator overloads (non-member overloads reside in HOperatorOverloads.h)
 *****************************************************************************/

  // Convert to matrix
  operator HHomMat3D() const;

  // Convert to pose
  operator HPose() const;

  // Conjugate dual quaternion
  HDualQuaternion operator ~ () const;

  // Create a dual quaternion from eight double values, where 
  // (RealW, RealX, RealY, RealZ) are the elements of the real part and
  // (DualW, DualX, DualY, DualZ) are the elements of the dual part.
  explicit HDualQuaternion(double RealW, double RealX,
                           double RealY, double RealZ,
                           double DualW, double DualX,
                           double DualY, double DualZ);

  // Create a dual quaternion from two quaternions for the real and the 
  // dual part.
  explicit HDualQuaternion(HQuaternion QuatReal, HQuaternion QuatDual);


  /***************************************************************************
   * Operators                                                               *
   ***************************************************************************/

  // Deserialize a serialized dual quaternion.
  void DeserializeDualQuat(const HSerializedItem& SerializedItemHandle);

  // Multiply two dual quaternions.
  static HDualQuaternionArray DualQuatCompose(const HDualQuaternionArray& DualQuaternionLeft, const HDualQuaternionArray& DualQuaternionRight);

  // Multiply two dual quaternions.
  HDualQuaternion DualQuatCompose(const HDualQuaternion& DualQuaternionRight) const;

  // Conjugate a dual quaternion.
  static HDualQuaternionArray DualQuatConjugate(const HDualQuaternionArray& DualQuaternion);

  // Conjugate a dual quaternion.
  HDualQuaternion DualQuatConjugate() const;

  // Interpolate two dual quaternions.
  HDualQuaternionArray DualQuatInterpolate(const HDualQuaternion& DualQuaternionEnd, const HTuple& InterpPos) const;

  // Interpolate two dual quaternions.
  HDualQuaternion DualQuatInterpolate(const HDualQuaternion& DualQuaternionEnd, double InterpPos) const;

  // Normalize a dual quaternion.
  static HDualQuaternionArray DualQuatNormalize(const HDualQuaternionArray& DualQuaternion);

  // Normalize a dual quaternion.
  HDualQuaternion DualQuatNormalize() const;

  // Convert a unit dual quaternion into a homogeneous transformation matrix.
  HHomMat3D DualQuatToHomMat3d() const;

  // Convert a dual quaternion to a 3D pose.
  static HPoseArray DualQuatToPose(const HDualQuaternionArray& DualQuaternion);

  // Convert a dual quaternion to a 3D pose.
  HPose DualQuatToPose() const;

  // Convert a unit dual quaternion into a screw.
  void DualQuatToScrew(const HString& ScrewFormat, double* AxisDirectionX, double* AxisDirectionY, double* AxisDirectionZ, double* AxisMomentOrPointX, double* AxisMomentOrPointY, double* AxisMomentOrPointZ, double* Rotation, double* Translation) const;

  // Convert a unit dual quaternion into a screw.
  void DualQuatToScrew(const char* ScrewFormat, double* AxisDirectionX, double* AxisDirectionY, double* AxisDirectionZ, double* AxisMomentOrPointX, double* AxisMomentOrPointY, double* AxisMomentOrPointZ, double* Rotation, double* Translation) const;

#ifdef _WIN32
  // Convert a unit dual quaternion into a screw.
  void DualQuatToScrew(const wchar_t* ScrewFormat, double* AxisDirectionX, double* AxisDirectionY, double* AxisDirectionZ, double* AxisMomentOrPointX, double* AxisMomentOrPointY, double* AxisMomentOrPointZ, double* Rotation, double* Translation) const;
#endif

  // Transform a 3D line with a unit dual quaternion.
  void DualQuatTransLine3d(const HString& LineFormat, const HTuple& LineDirectionX, const HTuple& LineDirectionY, const HTuple& LineDirectionZ, const HTuple& LineMomentOrPointX, const HTuple& LineMomentOrPointY, const HTuple& LineMomentOrPointZ, HTuple* TransLineDirectionX, HTuple* TransLineDirectionY, HTuple* TransLineDirectionZ, HTuple* TransLineMomentOrPointX, HTuple* TransLineMomentOrPointY, HTuple* TransLineMomentOrPointZ) const;

  // Transform a 3D line with a unit dual quaternion.
  void DualQuatTransLine3d(const HString& LineFormat, double LineDirectionX, double LineDirectionY, double LineDirectionZ, double LineMomentOrPointX, double LineMomentOrPointY, double LineMomentOrPointZ, double* TransLineDirectionX, double* TransLineDirectionY, double* TransLineDirectionZ, double* TransLineMomentOrPointX, double* TransLineMomentOrPointY, double* TransLineMomentOrPointZ) const;

  // Transform a 3D line with a unit dual quaternion.
  void DualQuatTransLine3d(const char* LineFormat, double LineDirectionX, double LineDirectionY, double LineDirectionZ, double LineMomentOrPointX, double LineMomentOrPointY, double LineMomentOrPointZ, double* TransLineDirectionX, double* TransLineDirectionY, double* TransLineDirectionZ, double* TransLineMomentOrPointX, double* TransLineMomentOrPointY, double* TransLineMomentOrPointZ) const;

#ifdef _WIN32
  // Transform a 3D line with a unit dual quaternion.
  void DualQuatTransLine3d(const wchar_t* LineFormat, double LineDirectionX, double LineDirectionY, double LineDirectionZ, double LineMomentOrPointX, double LineMomentOrPointY, double LineMomentOrPointZ, double* TransLineDirectionX, double* TransLineDirectionY, double* TransLineDirectionZ, double* TransLineMomentOrPointX, double* TransLineMomentOrPointY, double* TransLineMomentOrPointZ) const;
#endif

  // Convert a 3D pose to a unit dual quaternion.
  static HDualQuaternionArray PoseToDualQuat(const HPoseArray& Pose);

  // Convert a 3D pose to a unit dual quaternion.
  void PoseToDualQuat(const HPose& Pose);

  // Convert a screw into a dual quaternion.
  void ScrewToDualQuat(const HString& ScrewFormat, const HTuple& AxisDirectionX, const HTuple& AxisDirectionY, const HTuple& AxisDirectionZ, const HTuple& AxisMomentOrPointX, const HTuple& AxisMomentOrPointY, const HTuple& AxisMomentOrPointZ, const HTuple& Rotation, const HTuple& Translation);

  // Convert a screw into a dual quaternion.
  void ScrewToDualQuat(const HString& ScrewFormat, double AxisDirectionX, double AxisDirectionY, double AxisDirectionZ, double AxisMomentOrPointX, double AxisMomentOrPointY, double AxisMomentOrPointZ, double Rotation, double Translation);

  // Convert a screw into a dual quaternion.
  void ScrewToDualQuat(const char* ScrewFormat, double AxisDirectionX, double AxisDirectionY, double AxisDirectionZ, double AxisMomentOrPointX, double AxisMomentOrPointY, double AxisMomentOrPointZ, double Rotation, double Translation);

#ifdef _WIN32
  // Convert a screw into a dual quaternion.
  void ScrewToDualQuat(const wchar_t* ScrewFormat, double AxisDirectionX, double AxisDirectionY, double AxisDirectionZ, double AxisMomentOrPointX, double AxisMomentOrPointY, double AxisMomentOrPointZ, double Rotation, double Translation);
#endif

  // Serialize a dual quaternion.
  HSerializedItem SerializeDualQuat() const;

};

class LIntExport HDualQuaternionArray : public HDataArray
{
public:
  HDualQuaternionArray();
  HDualQuaternionArray(HDualQuaternion* classes, Hlong length);
  /* Define a default constructor to prevent compilers that support
   * C++11 R-Value References v3.0 declaring an implicite move
   * assignment/constructor. */
  ~HDualQuaternionArray() {}
protected:
  virtual void CreateArray(Hlong length);
  virtual int GetFixedSize() const;
};

}

#endif
