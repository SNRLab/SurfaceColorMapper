/*=auto=========================================================================

Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH) All Rights Reserved.

See COPYRIGHT.txt
or http://www.slicer.org/copyright/copyright.txt for details.

Program:   3D Slicer
Module:    $RCSfile: vtkMRMLSurfaceMapModelDisplayNode.cxx,v $
Date:      $Date: 2006/03/03 22:26:39 $
Version:   $Revision: 1.3 $

=========================================================================auto=*/
// MRML includes
#include "vtkMRMLSurfaceMapModelDisplayNode.h"

// VTK includes
#include <vtkAlgorithmOutput.h>
#include <vtkAssignAttribute.h>
#include <vtkCommand.h>
#include <vtkObjectFactory.h>
#include <vtkPassThrough.h>
#include <vtkPolyData.h>

//----------------------------------------------------------------------------
vtkMRMLNodeNewMacro(vtkMRMLSurfaceMapModelDisplayNode);

//-----------------------------------------------------------------------------
vtkMRMLSurfaceMapModelDisplayNode::vtkMRMLSurfaceMapModelDisplayNode()
{
  this->PassThrough = vtkPassThrough::New();
  this->AssignAttribute = vtkAssignAttribute::New();
  // Be careful, virtualization doesn't work in constructors
  this->UpdatePolyDataPipeline();
}

//-----------------------------------------------------------------------------
vtkMRMLSurfaceMapModelDisplayNode::~vtkMRMLSurfaceMapModelDisplayNode()
{
  this->PassThrough->Delete();
  this->AssignAttribute->Delete();
}

//---------------------------------------------------------------------------
void vtkMRMLSurfaceMapModelDisplayNode::ProcessMRMLEvents(vtkObject *caller,
                                                unsigned long event,
                                                void *callData )
{
  this->Superclass::ProcessMRMLEvents(caller, event, callData);

  if (event == vtkCommand::ModifiedEvent)
    {
    this->UpdatePolyDataPipeline();
    }
}

//---------------------------------------------------------------------------
void vtkMRMLSurfaceMapModelDisplayNode::SetInputPolyData(vtkPolyData* polyData)
{
  this->SetInputToPolyDataPipeline(polyData);
  this->Modified();
}

//---------------------------------------------------------------------------
void vtkMRMLSurfaceMapModelDisplayNode::SetInputToPolyDataPipeline(vtkPolyData* polyData)
{
  this->PassThrough->SetInput(polyData);
  this->AssignAttribute->SetInput(polyData);
}

//---------------------------------------------------------------------------
vtkPolyData* vtkMRMLSurfaceMapModelDisplayNode::GetInputPolyData()
{
  return vtkPolyData::SafeDownCast(this->AssignAttribute->GetInput());
}

//---------------------------------------------------------------------------
vtkPolyData* vtkMRMLSurfaceMapModelDisplayNode::GetOutputPolyData()
{
  if (!this->GetOutputPort())
    {
    return 0;
    }
  if (!this->GetInputPolyData())
    {
    return 0;
    }
  return vtkPolyData::SafeDownCast(
    this->GetOutputPort()->GetProducer()->GetOutputDataObject(
      this->GetOutputPort()->GetIndex()));
}

//---------------------------------------------------------------------------
vtkAlgorithmOutput* vtkMRMLSurfaceMapModelDisplayNode::GetOutputPort()
{
  if (this->GetActiveScalarName())
    {
    return this->AssignAttribute->GetOutputPort();
    }
  else
    {
    return this->PassThrough->GetOutputPort();
    }
}

//---------------------------------------------------------------------------
void vtkMRMLSurfaceMapModelDisplayNode::SetActiveScalarName(const char *scalarName)
{
  int wasModifying = this->StartModify();
  this->Superclass::SetActiveScalarName(scalarName);
  this->UpdatePolyDataPipeline();
  this->EndModify(wasModifying);
}

//---------------------------------------------------------------------------
void vtkMRMLSurfaceMapModelDisplayNode::SetActiveAttributeLocation(int location)
{
  int wasModifying = this->StartModify();
  this->Superclass::SetActiveAttributeLocation(location);
  this->UpdatePolyDataPipeline();
  this->EndModify(wasModifying);
}

//---------------------------------------------------------------------------
void vtkMRMLSurfaceMapModelDisplayNode::UpdatePolyDataPipeline()
{
  this->AssignAttribute->Assign(
    this->GetActiveScalarName(),
    this->GetActiveScalarName() ? vtkDataSetAttributes::SCALARS : -1,
    this->GetActiveAttributeLocation());
}
