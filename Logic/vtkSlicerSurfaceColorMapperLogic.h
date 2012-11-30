/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// .NAME vtkSlicerSurfaceColorMapperLogic - slicer logic class for volumes manipulation
// .SECTION Description
// This class manages the logic associated with reading, saving,
// and changing propertied of the volumes


#ifndef __vtkSlicerSurfaceColorMapperLogic_h
#define __vtkSlicerSurfaceColorMapperLogic_h

// Slicer includes
#include "vtkSlicerModuleLogic.h"

// MRML includes

// STD includes
#include <cstdlib>

#include "vtkSlicerSurfaceColorMapperModuleLogicExport.h"

class vtkPolyDataNormals;
class vtkPolyData;
class vtkPolyDataMapper;
class vtkActor;
class vtkRenderer;
class vtkMRMLScalarVolumeNode;


/// \ingroup Slicer_QtModules_ExtensionTemplate
class VTK_SLICER_SURFACECOLORMAPPER_MODULE_LOGIC_EXPORT vtkSlicerSurfaceColorMapperLogic :
  public vtkSlicerModuleLogic
{
public:

  static vtkSlicerSurfaceColorMapperLogic *New();
  vtkTypeMacro(vtkSlicerSurfaceColorMapperLogic, vtkSlicerModuleLogic);
  void PrintSelf(ostream& os, vtkIndent indent);

  void   ProcessWindowLevel(double min, double max);
  void   ProcessOuterLayer(double min, double max);

  void   SetInputVolumeID(const char* nodeID) { this->VolumeNodeID = nodeID; };
  bool   SetInputModelID(const char* nodeID);
  void   SetColorTableNodeID(const char* nodeID) { this->ColorTableNodeID = nodeID; };
  void   UpdateTexture();

  void   SetProjectionMode(bool s) { this->ProjectionMode = s; };
  void   SetEnabled(bool);

protected:
  vtkSlicerSurfaceColorMapperLogic();
  virtual ~vtkSlicerSurfaceColorMapperLogic();

  //BTX
  std::string VolumeNodeID;
  std::string ModelNodeID;
  std::string ColorTableNodeID;
  //ETX

  double TrilinearInterpolation(vtkMRMLScalarVolumeNode * vnode, double x[3]);

  virtual void SetMRMLSceneInternal(vtkMRMLScene* newScene);
  /// Register MRML Node classes to Scene. Gets called automatically when the MRMLScene is attached to this logic class.
  virtual void RegisterNodes();
  virtual void UpdateFromMRMLScene();
  virtual void OnMRMLSceneNodeAdded(vtkMRMLNode* node);
  virtual void OnMRMLSceneNodeRemoved(vtkMRMLNode* node);

  bool                Enabled;
  bool                ProjectionMode;
  
  vtkDoubleArray*     PointValue;
  vtkPolyDataNormals* PolyDataNormals;
  vtkPolyData*        PolyData;
  vtkPolyDataMapper*  Mapper;
  vtkActor*           Actor;
  vtkRenderer*        Renderer;

  int    NeedModelUpdate;
  double Lower;
  double Upper;
  double Step;
  double Window;
  double Level;

private:

  vtkSlicerSurfaceColorMapperLogic(const vtkSlicerSurfaceColorMapperLogic&); // Not implemented
  void operator=(const vtkSlicerSurfaceColorMapperLogic&);               // Not implemented
};

#endif
