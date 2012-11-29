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

// SurfaceColorMapper Logic includes
#include "vtkSlicerSurfaceColorMapperLogic.h"

// MRML includes

// VTK includes
#include <vtkNew.h>

#include <vtkPolyData.h>
#include <vtkPolyDataReader.h>
#include <vtkImageData.h>
#include <vtkImageReader.h>
#include <vtkStructuredPointsReader.h>
#include <vtkStructuredPoints.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkDoubleArray.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkCell.h>
#include <vtkCellData.h>
#include <vtkDataSet.h>
#include <vtkDataSetAttributes.h>
#include <vtkProperty.h>
#include <vtkSmartPointer.h>
#include <vtkTubeFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkCleanPolyData.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <vtkMatrix4x4.h>

#include <vtkMRMLScalarVolumeNode.h>
#include <vtkMRMLModelNode.h>

// STD includes
#include <cassert>

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkSlicerSurfaceColorMapperLogic);

//----------------------------------------------------------------------------
vtkSlicerSurfaceColorMapperLogic::vtkSlicerSurfaceColorMapperLogic()
{
  this->PointValue   = NULL;
  this->NeedModelUpdate = 1;

  this->Lower   = 0.0; 
  this->Upper   = 1.0; 
  this->Step    = 0.5; 
  this->Window  = 3000;
  this->Level   = 1000;

  this->PolyDataNormals = NULL;
  this->PolyData = NULL;
  this->Mapper   = NULL;
  this->Actor    = NULL;
  this->Renderer = NULL;
}

//----------------------------------------------------------------------------
vtkSlicerSurfaceColorMapperLogic::~vtkSlicerSurfaceColorMapperLogic()
{
}

//----------------------------------------------------------------------------
void vtkSlicerSurfaceColorMapperLogic::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//---------------------------------------------------------------------------
void vtkSlicerSurfaceColorMapperLogic::SetMRMLSceneInternal(vtkMRMLScene * newScene)
{
  vtkNew<vtkIntArray> events;
  events->InsertNextValue(vtkMRMLScene::NodeAddedEvent);
  events->InsertNextValue(vtkMRMLScene::NodeRemovedEvent);
  events->InsertNextValue(vtkMRMLScene::EndBatchProcessEvent);
  this->SetAndObserveMRMLSceneEventsInternal(newScene, events.GetPointer());
}

//-----------------------------------------------------------------------------
void vtkSlicerSurfaceColorMapperLogic::RegisterNodes()
{
  assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicerSurfaceColorMapperLogic::UpdateFromMRMLScene()
{
  assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicerSurfaceColorMapperLogic
::OnMRMLSceneNodeAdded(vtkMRMLNode* vtkNotUsed(node))
{
}

//---------------------------------------------------------------------------
void vtkSlicerSurfaceColorMapperLogic
::OnMRMLSceneNodeRemoved(vtkMRMLNode* vtkNotUsed(node))
{
}

//----------------------------------------------------------------------------
void vtkSlicerSurfaceColorMapperLogic::ProcessWindowLevel(double min, double max)
{
  this->Window = max-min;
  this->Level  = (max+min)/2;
  UpdateTexture();
}

//----------------------------------------------------------------------------
void vtkSlicerSurfaceColorMapperLogic::ProcessOuterLayer(double min, double max)
{
  this->Lower = min;
  this->Upper = max;
  this->NeedModelUpdate = 1;
  UpdateTexture();
}


//---------------------------------------------------------------------------
void vtkSlicerSurfaceColorMapperLogic::UpdateTexture()
{

  vtkMRMLScene* scene  = this->GetMRMLScene();

  if (!scene)
    {
    std::cerr << "MRML scene node was not found." << std::endl;
    return;
    }

  vtkMRMLScalarVolumeNode* vnode
    = vtkMRMLScalarVolumeNode::SafeDownCast(scene->GetNodeByID(this->VolumeNodeID.c_str()));
  if (!vnode)
    {
    std::cerr << "Volume node was not found." << std::endl;
    return;
    }

  vtkMRMLModelNode* mnode
    = vtkMRMLModelNode::SafeDownCast(scene->GetNodeByID(this->ModelNodeID.c_str()));
  if (!mnode)
    {
    std::cerr << "Volume node was not found." << std::endl;
    return;
    }

  vtkPolyData* poly = mnode->GetPolyData();

  // Calculate normals
  if (!this->PolyDataNormals)
    {
    this->PolyDataNormals = vtkPolyDataNormals::New();
    }

  //polyDataNormals->SetInputConnection(reader->GetOutputPort());
  PolyDataNormals->SetInput(poly);
  PolyDataNormals->SplittingOn();
  PolyDataNormals->SetFeatureAngle(170.0);  // Need to be examined
  PolyDataNormals->Update();
  
  this->PolyData = this->PolyDataNormals->GetOutput();
  int n = this->PolyData->GetNumberOfPoints();

  vtkSmartPointer<vtkUnsignedCharArray> colors =
    vtkSmartPointer<vtkUnsignedCharArray>::New();
  colors->SetName("Colors");
  colors->SetNumberOfComponents(3);
  colors->SetNumberOfTuples(n);

  const double low   = this->Level - this->Window/2.0;
  const double scale = 255.0 / this->Window;

  vtkSmartPointer<vtkDataArray> norms = this->PolyData->GetPointData()->GetNormals();

  if (!this->PointValue)
    {
    this->PointValue = vtkDoubleArray::New();
    this->NeedModelUpdate = 1;
    }

  if (this->NeedModelUpdate)
    {
    this->PointValue->Reset();
    }

  double min;
  double max;
  int init = 0;

  for (int i = 0; i < n; i ++)
    {
    if (this->NeedModelUpdate)
      {
      // Get coordinate
      double x[3];
      this->PolyData->GetPoint(i, x);
      
      // Get normal
      double n[3];
      norms->GetTuple(i, n);
      
      // Calculate projection
      double sum = 0;
      int nstep = 0;
      for (double d = this->Lower; d <= this->Upper; d += this->Step)
        {
        double p[3];
        p[0] = x[0] + n[0]*d;
        p[1] = x[1] + n[1]*d;
        p[2] = x[2] + n[2]*d;
        sum += TrilinearInterpolation(vnode, p);
        nstep ++;
        }
      double value = sum / (double)nstep;

      if (init == 1)
        {
        min = max = this->PointValue->GetValue(0);
        init = 0;
        }
      else if (value < min)
        {
        min = value;
        }
      else if (value > max)
        {
        max = value;
        }
      this->PointValue->InsertValue(i, value);
      }
      
    double intensity = (this->PointValue->GetValue(i) - low) * scale;
    if (intensity > 255.0)
      {
      intensity = 255.0;
      }
    else if (intensity < 0.0)
      {
      intensity = 0.0;
      }
    unsigned char cv = (unsigned char) intensity;

    colors->InsertTuple3(i, cv, cv, cv);
    }

  //if (this->NeedModelUpdate)
  //  {
  //  this->WindowLevelRange->SetWholeRange(min, max);
  //  }

  this->NeedModelUpdate = 0;
  this->PolyData->GetPointData()->SetScalars(colors);
  this->PolyData->Update();

  if (!this->Mapper)
    {
    this->Mapper = vtkPolyDataMapper::New();
    this->Mapper->SetInput(this->PolyData);
    this->Mapper->SetScalarModeToUsePointFieldData();
    this->Mapper->SelectColorArray("Colors");
    }
  this->Mapper->ScalarVisibilityOn();
  this->Mapper->Update();

  if (!this->Actor)
    {
    this->Actor = vtkActor::New();
    this->Actor->SetMapper(this->Mapper);
    }

  //vtkSlicerViewerWidget *viewer_widget = this->GetApplicationGUI()->GetActiveViewerWidget();
  //if (viewer_widget)
  //  {
  //  if (!this->Renderer)
  //    {
  //    this->Renderer = viewer_widget->GetMainViewer()->GetRenderer();
  //    this->Renderer->AddActor(this->Actor);
  //    }
  //  viewer_widget->RequestRender();
  //  }


}


double vtkSlicerSurfaceColorMapperLogic::TrilinearInterpolation(vtkMRMLScalarVolumeNode * vnode, double x[3])
{
  int    dim[3];
  int    ijk[3];
  double r[3];

  if (!vnode)
    {
    return 0;
    }

  vtkImageData * image = vnode->GetImageData();
  if (!image)
    {
    return 0;
    }
  image->GetDimensions(dim);

  vtkSmartPointer<vtkMatrix4x4> RASToIJKMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
  vnode->GetRASToIJKMatrix(RASToIJKMatrix);
  double in[4];
  double out[4];

  in[0] = x[0];
  in[1] = x[1];
  in[2] = x[2];
  in[3] = 1.0;
  RASToIJKMatrix->MultiplyPoint(in, out);

  ijk[0] = (int)out[0];
  ijk[1] = (int)out[1];
  ijk[2] = (int)out[2];

  r[0] = out[0] - (double)ijk[0];
  r[1] = out[1] - (double)ijk[1];
  r[2] = out[2] - (double)ijk[2];

  //if (spoints->ComputeStructuredCoordinates(x, ijk, r) == 0)
  if (ijk[0] < 0 || ijk[0] >= dim[0]-1 ||
      ijk[1] < 0 || ijk[1] >= dim[1]-1 ||
      ijk[2] < 0 || ijk[2] >= dim[2]-1)
    {
    //std::cerr << "Point (" << x[0] << ", " << x[1] << ", " << x[2]
    //          << ") is outside of the volume." << std::endl;
    return 0.0;
    }
  
  int ii = ijk[0];    int jj = ijk[1];    int kk = ijk[2];
  double v000 = (double)*((short*)image->GetScalarPointer(ii,   jj,   kk  ));
  double v100 = (double)*((short*)image->GetScalarPointer(ii+1, jj,   kk  ));
  double v010 = (double)*((short*)image->GetScalarPointer(ii,   jj+1, kk  ));
  double v110 = (double)*((short*)image->GetScalarPointer(ii+1, jj+1, kk  ));
  double v001 = (double)*((short*)image->GetScalarPointer(ii,   jj,   kk+1));
  double v101 = (double)*((short*)image->GetScalarPointer(ii+1, jj,   kk+1));
  double v011 = (double)*((short*)image->GetScalarPointer(ii,   jj+1, kk+1));
  double v111 = (double)*((short*)image->GetScalarPointer(ii+1, jj+1, kk+1));
  
  double r0 = r[0]; double r1 = r[1]; double r2 = r[2];
  double value =
    v000 * (1-r0) * (1-r1) * (1-r2) +
    v100 *    r0  * (1-r1) * (1-r2) +
    v010 * (1-r0) *    r1  * (1-r2) +
    v001 * (1-r0) * (1-r1) *    r2  +
    v101 *    r0  * (1-r1) *    r2  +
    v011 * (1-r0) *    r1  *    r2  +
    v110 *    r0  *    r1  * (1-r2) +
    v111 *    r0  *    r1  *    r2;

  return value;

}
