/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// FooBar Widgets includes
#include "qSlicerSurfaceColorMapperFooBarWidget.h"
#include "ui_qSlicerSurfaceColorMapperFooBarWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_SurfaceColorMapper
class qSlicerSurfaceColorMapperFooBarWidgetPrivate
  : public Ui_qSlicerSurfaceColorMapperFooBarWidget
{
  Q_DECLARE_PUBLIC(qSlicerSurfaceColorMapperFooBarWidget);
protected:
  qSlicerSurfaceColorMapperFooBarWidget* const q_ptr;

public:
  qSlicerSurfaceColorMapperFooBarWidgetPrivate(
    qSlicerSurfaceColorMapperFooBarWidget& object);
  virtual void setupUi(qSlicerSurfaceColorMapperFooBarWidget*);
};

// --------------------------------------------------------------------------
qSlicerSurfaceColorMapperFooBarWidgetPrivate
::qSlicerSurfaceColorMapperFooBarWidgetPrivate(
  qSlicerSurfaceColorMapperFooBarWidget& object)
  : q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qSlicerSurfaceColorMapperFooBarWidgetPrivate
::setupUi(qSlicerSurfaceColorMapperFooBarWidget* widget)
{
  this->Ui_qSlicerSurfaceColorMapperFooBarWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qSlicerSurfaceColorMapperFooBarWidget methods

//-----------------------------------------------------------------------------
qSlicerSurfaceColorMapperFooBarWidget
::qSlicerSurfaceColorMapperFooBarWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qSlicerSurfaceColorMapperFooBarWidgetPrivate(*this) )
{
  Q_D(qSlicerSurfaceColorMapperFooBarWidget);
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
qSlicerSurfaceColorMapperFooBarWidget
::~qSlicerSurfaceColorMapperFooBarWidget()
{
}
