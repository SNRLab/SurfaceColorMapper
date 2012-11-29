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
#include <QDebug>

// SlicerQt includes
#include "qSlicerSurfaceColorMapperModuleWidget.h"
#include "ui_qSlicerSurfaceColorMapperModule.h"

#include "vtkSlicerSurfaceColorMapperLogic.h"

#include "qSlicerApplication.h"
#include "vtkMRMLScene.h"

#include "vtkMRMLScalarVolumeNode.h"
#include "vtkMRMLModelNode.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerSurfaceColorMapperModuleWidgetPrivate: public Ui_qSlicerSurfaceColorMapperModule
{
public:
  qSlicerSurfaceColorMapperModuleWidgetPrivate();
};

//-----------------------------------------------------------------------------
// qSlicerSurfaceColorMapperModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerSurfaceColorMapperModuleWidgetPrivate::qSlicerSurfaceColorMapperModuleWidgetPrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerSurfaceColorMapperModuleWidget methods

//-----------------------------------------------------------------------------
qSlicerSurfaceColorMapperModuleWidget::qSlicerSurfaceColorMapperModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicerSurfaceColorMapperModuleWidgetPrivate )
{
}

//-----------------------------------------------------------------------------
qSlicerSurfaceColorMapperModuleWidget::~qSlicerSurfaceColorMapperModuleWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicerSurfaceColorMapperModuleWidget::setMRMLScene(vtkMRMLScene *newScene)
{
  Q_D(qSlicerSurfaceColorMapperModuleWidget);

  vtkMRMLScene* oldScene = this->mrmlScene();
  this->Superclass::setMRMLScene(newScene);
  qSlicerApplication * app = qSlicerApplication::application();
  if (!app)
    {
    return;
    }
  
  if (oldScene != newScene)
    {
    if (d->inputVolumeSelector)
      {
      d->inputVolumeSelector->setMRMLScene(newScene);
      }
    if (d->surfaceModelSelector)
      {
      d->surfaceModelSelector->setMRMLScene(newScene);
      }
    if (d->colorTableSelector)
      {
      d->colorTableSelector->setMRMLScene(newScene);
      }
    }
  newScene->InitTraversal();
}

//-----------------------------------------------------------------------------
void qSlicerSurfaceColorMapperModuleWidget::setup()
{
  Q_D(qSlicerSurfaceColorMapperModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();

  if (d->inputVolumeSelector)
    {
    connect(d->inputVolumeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
            this, SLOT(setInputVolumeNode(vtkMRMLNode*)));
    }
  if (d->surfaceModelSelector)
    {
    connect(d->surfaceModelSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
            this, SLOT(setSurfaceModelNode(vtkMRMLNode*)));
    }
  if (d->colorTableSelector)
    {
    connect(d->colorTableSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
            this, SLOT(setColorTableNode(vtkMRMLNode*)));
    }
  if (d->colorRangeWidget)
    {
    connect(d->colorRangeWidget, SIGNAL(rangeChanged(double, double)),
            this, SLOT(setColorRange(double, double)));
    connect(d->colorRangeWidget, SIGNAL(valueChanged(double, double)),
            this, SLOT(setColorValue(double, double)));
    }

}

//-----------------------------------------------------------------------------
void qSlicerSurfaceColorMapperModuleWidget::setInputVolumeNode(vtkMRMLNode* n)
{
  Q_D(qSlicerSurfaceColorMapperModuleWidget);
  
  vtkMRMLScalarVolumeNode* node = vtkMRMLScalarVolumeNode::SafeDownCast(n);
  vtkSlicerSurfaceColorMapperLogic* logic = vtkSlicerSurfaceColorMapperLogic::SafeDownCast(this->logic());
  if (node && logic)
    {
    logic->SetInputVolumeID(node->GetID());
    }
}

//-----------------------------------------------------------------------------
void qSlicerSurfaceColorMapperModuleWidget::setSurfaceModelNode(vtkMRMLNode* n)
{
  Q_D(qSlicerSurfaceColorMapperModuleWidget);

  vtkMRMLModelNode* node = vtkMRMLModelNode::SafeDownCast(n);
  vtkSlicerSurfaceColorMapperLogic* logic = vtkSlicerSurfaceColorMapperLogic::SafeDownCast(this->logic());
  if (node && logic)
    {
    logic->SetInputModelID(node->GetID());
    }
}

//-----------------------------------------------------------------------------
void qSlicerSurfaceColorMapperModuleWidget::setColorTableNode(vtkMRMLNode*)
{
  Q_D(qSlicerSurfaceColorMapperModuleWidget);
}


//-----------------------------------------------------------------------------
void setColorRange(double min, double max)
{
  Q_D(qSlicerSurfaceColorMapperModuleWidget);
}


//-----------------------------------------------------------------------------
void setColorValues(double min, double max)
{
  Q_D(qSlicerSurfaceColorMapperModuleWidget);
}

