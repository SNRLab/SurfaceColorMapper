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

#ifndef __qSlicerSurfaceColorMapperModuleWidget_h
#define __qSlicerSurfaceColorMapperModuleWidget_h

// SlicerQt includes
#include "qSlicerAbstractModuleWidget.h"

#include "qSlicerSurfaceColorMapperModuleExport.h"

class qSlicerSurfaceColorMapperModuleWidgetPrivate;
class vtkMRMLNode;

/// \ingroup Slicer_QtModules_ExtensionTemplate
class Q_SLICER_QTMODULES_SURFACECOLORMAPPER_EXPORT qSlicerSurfaceColorMapperModuleWidget :
  public qSlicerAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qSlicerAbstractModuleWidget Superclass;
  qSlicerSurfaceColorMapperModuleWidget(QWidget *parent=0);
  virtual ~qSlicerSurfaceColorMapperModuleWidget();

public slots:
  virtual void setMRMLScene(vtkMRMLScene *newScene);

  void enableMapping(int s);
  void setInputVolumeNode(vtkMRMLNode*);
  void setSurfaceModelNode(vtkMRMLNode*);
  void setColorTableNode(vtkMRMLNode*);
  void setColorRange(double min, double max);
  void setColorValues(double min, double max);
  void enableScalarBar(int s);
  void updateUnit(const QString & text);

protected:
  QScopedPointer<qSlicerSurfaceColorMapperModuleWidgetPrivate> d_ptr;
  
  virtual void setup();
  void forceRender();
  

private:
  Q_DECLARE_PRIVATE(qSlicerSurfaceColorMapperModuleWidget);
  Q_DISABLE_COPY(qSlicerSurfaceColorMapperModuleWidget);
};

#endif
