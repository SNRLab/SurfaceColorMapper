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

#include "vtkImageData.h"
#include <vtkColorTransferFunction.h>
#include <vtkLookupTable.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkScalarBarActor.h>
#include <vtkScalarBarWidget.h>

#include "vtkSlicerSurfaceColorMapperLogic.h"

#include "qMRMLThreeDView.h"
#include "qMRMLThreeDWidget.h"
#include "qSlicerApplication.h"
#include "qSlicerLayoutManager.h"
#include "vtkMRMLScene.h"

#include "vtkMRMLScalarVolumeNode.h"
#include "vtkMRMLModelNode.h"
#include "vtkMRMLColorTableNode.h"
#include "vtkMRMLProceduralColorNode.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerSurfaceColorMapperModuleWidgetPrivate: public Ui_qSlicerSurfaceColorMapperModule
{
public:
  qSlicerSurfaceColorMapperModuleWidgetPrivate();
  ~qSlicerSurfaceColorMapperModuleWidgetPrivate();

  vtkScalarBarWidget* ScalarBarWidget;
};

//-----------------------------------------------------------------------------
// qSlicerSurfaceColorMapperModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerSurfaceColorMapperModuleWidgetPrivate::qSlicerSurfaceColorMapperModuleWidgetPrivate()
{
  this->ScalarBarWidget = vtkScalarBarWidget::New();
  this->ScalarBarWidget->GetScalarBarActor()->SetOrientationToVertical();
  this->ScalarBarWidget->GetScalarBarActor()->SetNumberOfLabels(11);
  this->ScalarBarWidget->GetScalarBarActor()->SetTitle("");
  this->ScalarBarWidget->GetScalarBarActor()->SetLabelFormat(" %#8.3f");

  this->ScalarBarWidget->GetScalarBarActor()->SetPosition(0.1, 0.1);
  this->ScalarBarWidget->GetScalarBarActor()->SetWidth(0.1);
  this->ScalarBarWidget->GetScalarBarActor()->SetHeight(0.8);
  this->ScalarBarWidget->SetEnabled(false);
}

qSlicerSurfaceColorMapperModuleWidgetPrivate::~qSlicerSurfaceColorMapperModuleWidgetPrivate()
{
  if (this->ScalarBarWidget)
    {
    this->ScalarBarWidget->Delete();
    this->ScalarBarWidget = 0;
    }
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

  if (d->mappingCheckBox)
    {
    connect(d->mappingCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(enableMapping(int)));
    }

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
    //connect(d->colorRangeWidget, SIGNAL(rangeChanged(double, double)),
    //        this, SLOT(setColorRange(double, double)));
    connect(d->colorRangeWidget, SIGNAL(valuesChanged(double, double)),
            this, SLOT(setColorValues(double, double)));
    }
  if (d->scalarBarCheckBox)
    {
    connect(d->scalarBarCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(enableScalarBar(int)));
    }
  if (d->unitLineEdit)
    {
    connect(d->unitLineEdit, SIGNAL(textChanged(const QString&)),
            this, SLOT(updateUnit(const QString&)));
    }
  

  // Set vtk renderer
  //d->VTKScalarBar->setScalarBarWidget(d->ScalarBarWidget);

  qSlicerApplication * app = qSlicerApplication::application();

  if (app && app->layoutManager())
    {
    qMRMLThreeDView* threeDView = app->layoutManager()->threeDWidget(0)->threeDView();
    vtkRenderer* activeRenderer = app->layoutManager()->activeThreeDRenderer();
    if (activeRenderer)
      {
      d->ScalarBarWidget->SetInteractor(activeRenderer->GetRenderWindow()->GetInteractor());
      }
    //connect(d->ScalarBarWidget, SIGNAL(modified()), threeDView, SLOT(scheduleRender()));
    }

}


//-----------------------------------------------------------------------------
void qSlicerSurfaceColorMapperModuleWidget::enableMapping(int s)
{
  Q_D(qSlicerSurfaceColorMapperModuleWidget);

  vtkSlicerSurfaceColorMapperLogic* logic = vtkSlicerSurfaceColorMapperLogic::SafeDownCast(this->logic());
  if (logic)
    {
    logic->SetEnabled(s);
    }
  enableScalarBar(s&&d->mappingCheckBox->isChecked());
  forceRender();
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

    vtkImageData* image = node->GetImageData();
    double range[2];
    image->GetScalarRange(range);
    if (d->colorRangeWidget)
      {
      d->colorRangeWidget->setRange(range[0], range[1]);
      }
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
    bool s = logic->SetInputModelID(node->GetID());
    d->mappingCheckBox->setChecked(s);
    }
}

//-----------------------------------------------------------------------------
void qSlicerSurfaceColorMapperModuleWidget::setColorTableNode(vtkMRMLNode* n)
{
  Q_D(qSlicerSurfaceColorMapperModuleWidget);

  vtkMRMLColorTableNode* colorNode = vtkMRMLColorTableNode::SafeDownCast(n);
  vtkSlicerSurfaceColorMapperLogic* logic = vtkSlicerSurfaceColorMapperLogic::SafeDownCast(this->logic());
  if (colorNode && logic)
    {
    logic->SetColorTableNodeID(colorNode->GetID());
    
    if (colorNode->GetLookupTable())
      {
      d->ScalarBarWidget->GetScalarBarActor()->SetLookupTable(colorNode->GetLookupTable());
      }
    else
      {
      vtkMRMLProceduralColorNode *procColorNode = vtkMRMLProceduralColorNode::SafeDownCast(colorNode);
      if (procColorNode)
        {
        d->ScalarBarWidget->GetScalarBarActor()->SetLookupTable(procColorNode->GetColorTransferFunction());
        }
      }
    colorNode->Modified();
    forceRender();
    }
}


//-----------------------------------------------------------------------------
void qSlicerSurfaceColorMapperModuleWidget::setColorRange(double min, double max)
{
  Q_D(qSlicerSurfaceColorMapperModuleWidget);
}


//-----------------------------------------------------------------------------
void qSlicerSurfaceColorMapperModuleWidget::setColorValues(double min, double max)
{
  Q_D(qSlicerSurfaceColorMapperModuleWidget);
  vtkSlicerSurfaceColorMapperLogic* logic = vtkSlicerSurfaceColorMapperLogic::SafeDownCast(this->logic());
  qSlicerApplication * app = qSlicerApplication::application();
  if (logic && app)
    {
    logic->ProcessWindowLevel(min, max);
    if (d->ScalarBarWidget) // Update scalar range for the scalar bar
      {
      vtkMRMLColorNode* colorNode = vtkMRMLColorNode::SafeDownCast(d->colorTableSelector->currentNode());
      colorNode->GetLookupTable()->SetRange(min, max);
      }
    }
}


//-----------------------------------------------------------------------------
void qSlicerSurfaceColorMapperModuleWidget::enableScalarBar(int s)
{
  Q_D(qSlicerSurfaceColorMapperModuleWidget);

  if (d->ScalarBarWidget)
    {
    d->ScalarBarWidget->SetEnabled(s);
    d->ScalarBarWidget->GetScalarBarActor()->Modified();
    forceRender();
    }
}


//-----------------------------------------------------------------------------
void qSlicerSurfaceColorMapperModuleWidget::updateUnit(const QString & text)
{
  Q_D(qSlicerSurfaceColorMapperModuleWidget);

  std::string str(text.toAscii());

  if (d->ScalarBarWidget)
    {
    d->ScalarBarWidget->GetScalarBarActor()->SetTitle(text.toLatin1());
    d->ScalarBarWidget->GetScalarBarActor()->Modified();
    forceRender();
    }

}


//-----------------------------------------------------------------------------
void qSlicerSurfaceColorMapperModuleWidget::forceRender()
{
  Q_D(qSlicerSurfaceColorMapperModuleWidget);

  qSlicerApplication * app = qSlicerApplication::application();
  if (app)
    {
    vtkRenderer* activeRenderer = app->layoutManager()->activeThreeDRenderer();
    if (activeRenderer)
      {
      vtkRenderWindow* rwindow = activeRenderer->GetRenderWindow();
      if (rwindow)
        {
        rwindow->Render();
        }
      }
    }
}
