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

// Qt includes
#include <QtPlugin>

// SurfaceColorMapper Logic includes
#include <vtkSlicerSurfaceColorMapperLogic.h>

// SurfaceColorMapper includes
#include "qSlicerSurfaceColorMapperModule.h"
#include "qSlicerSurfaceColorMapperModuleWidget.h"

//-----------------------------------------------------------------------------
Q_EXPORT_PLUGIN2(qSlicerSurfaceColorMapperModule, qSlicerSurfaceColorMapperModule);

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerSurfaceColorMapperModulePrivate
{
public:
  qSlicerSurfaceColorMapperModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerSurfaceColorMapperModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerSurfaceColorMapperModulePrivate
::qSlicerSurfaceColorMapperModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerSurfaceColorMapperModule methods

//-----------------------------------------------------------------------------
qSlicerSurfaceColorMapperModule
::qSlicerSurfaceColorMapperModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerSurfaceColorMapperModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerSurfaceColorMapperModule::~qSlicerSurfaceColorMapperModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerSurfaceColorMapperModule::helpText()const
{
  return "This is a loadable module bundled in an extension";
}

//-----------------------------------------------------------------------------
QString qSlicerSurfaceColorMapperModule::acknowledgementText()const
{
  return "This work is supported by NIH 5R01CA138586.";
}

//-----------------------------------------------------------------------------
QStringList qSlicerSurfaceColorMapperModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Junichi Tokuda (BWH)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerSurfaceColorMapperModule::icon()const
{
  return QIcon(":/Icons/SurfaceColorMapper.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerSurfaceColorMapperModule::categories() const
{
  return QStringList() << "Examples";
}

//-----------------------------------------------------------------------------
QStringList qSlicerSurfaceColorMapperModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qSlicerSurfaceColorMapperModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation * qSlicerSurfaceColorMapperModule
::createWidgetRepresentation()
{
  return new qSlicerSurfaceColorMapperModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerSurfaceColorMapperModule::createLogic()
{
  return vtkSlicerSurfaceColorMapperLogic::New();
}
