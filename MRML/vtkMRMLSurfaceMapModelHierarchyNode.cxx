/*=auto=========================================================================

Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH) All Rights Reserved.

See COPYRIGHT.txt
or http://www.slicer.org/copyright/copyright.txt for details.

Program:   3D Slicer
Module:    $RCSfile: vtkMRMLSurfaceMapModelHierarchyNode.cxx,v $
Date:      $Date: 2006/03/03 22:26:39 $
Version:   $Revision: 1.3 $

=========================================================================auto=*/

// MRML includes
#include "vtkMRMLSurfaceMapModelDisplayNode.h"
#include "vtkMRMLSurfaceMapModelHierarchyNode.h"
#include "vtkMRMLSurfaceMapModelNode.h"
#include "vtkMRMLScene.h"

// VTK includes
#include <vtkObjectFactory.h>
#include <vtkCallbackCommand.h>
#include <vtkCollection.h>

//----------------------------------------------------------------------------
vtkMRMLNodeNewMacro(vtkMRMLSurfaceMapModelHierarchyNode);


//----------------------------------------------------------------------------
vtkMRMLSurfaceMapModelHierarchyNode::vtkMRMLSurfaceMapModelHierarchyNode()
{
  this->ModelDisplayNode = NULL;
  this->HideFromEditors = 1;
}

//----------------------------------------------------------------------------
vtkMRMLSurfaceMapModelHierarchyNode::~vtkMRMLSurfaceMapModelHierarchyNode()
{
}

//----------------------------------------------------------------------------
void vtkMRMLSurfaceMapModelHierarchyNode::WriteXML(ostream& of, int nIndent)
{
  // Write all attributes not equal to their defaults
  
  Superclass::WriteXML(of, nIndent);

}

//----------------------------------------------------------------------------
void vtkMRMLSurfaceMapModelHierarchyNode::UpdateReferenceID(const char *oldID, const char *newID)
{
  Superclass::UpdateReferenceID(oldID, newID);
}

//----------------------------------------------------------------------------
void vtkMRMLSurfaceMapModelHierarchyNode::ReadXMLAttributes(const char** atts)
{
  int disabledModify = this->StartModify();

  Superclass::ReadXMLAttributes(atts);

  const char* attName;
  const char* attValue;
  while (*atts != NULL) 
    {
    attName = *(atts++);
    attValue = *(atts++);
    if (!strcmp(attName, "modelNodeRef") ||
        !strcmp(attName, "modelNodeID") )
      {
      this->SetDisplayableNodeID(attValue);
      //this->Scene->AddReferencedNodeID(this->ModelNodeID, this);
      }
    }

  this->EndModify(disabledModify);
}


//----------------------------------------------------------------------------
// Copy the node's attributes to this object.
// Does NOT copy: ID, FilePrefix, Name, ID
void vtkMRMLSurfaceMapModelHierarchyNode::Copy(vtkMRMLNode *anode)
{
  int disabledModify = this->StartModify();

  Superclass::Copy(anode);
//  vtkMRMLSurfaceMapModelHierarchyNode *node = (vtkMRMLSurfaceMapModelHierarchyNode *) anode;

  this->EndModify(disabledModify);

}

//----------------------------------------------------------------------------
void vtkMRMLSurfaceMapModelHierarchyNode::PrintSelf(ostream& os, vtkIndent indent)
{
  
  Superclass::PrintSelf(os,indent);

  if (this->ModelDisplayNode)
    {
    os << indent << "ModelDisplayNode ID = " <<
      (this->ModelDisplayNode->GetID() ? this->ModelDisplayNode->GetID() : "(none)") << "\n";
    }
}

//-----------------------------------------------------------
void vtkMRMLSurfaceMapModelHierarchyNode::UpdateScene(vtkMRMLScene *scene)
{
  Superclass::UpdateScene(scene);

}

//-----------------------------------------------------------
void vtkMRMLSurfaceMapModelHierarchyNode::UpdateReferences()
{
  Superclass::UpdateReferences();  
}

vtkMRMLSurfaceMapModelNode* vtkMRMLSurfaceMapModelHierarchyNode::GetModelNode()
{
  vtkMRMLSurfaceMapModelNode* node = vtkMRMLSurfaceMapModelNode::SafeDownCast(
    this->GetAssociatedNode());
  return node;
}

//----------------------------------------------------------------------------
vtkMRMLSurfaceMapModelDisplayNode* vtkMRMLSurfaceMapModelHierarchyNode::GetModelDisplayNode()
{
  vtkMRMLSurfaceMapModelDisplayNode* node = NULL;
  vtkMRMLNode* snode = Superclass::GetDisplayNode();
  if (snode)
    {
    node = vtkMRMLSurfaceMapModelDisplayNode::SafeDownCast(snode);
    }
  return node;
}



//---------------------------------------------------------------------------
void vtkMRMLSurfaceMapModelHierarchyNode::ProcessMRMLEvents ( vtkObject *caller,
                                           unsigned long event, 
                                           void *callData )
{
  Superclass::ProcessMRMLEvents(caller, event, callData);

  vtkMRMLSurfaceMapModelDisplayNode *dnode = this->GetModelDisplayNode();
  if (dnode != NULL && dnode == vtkMRMLSurfaceMapModelDisplayNode::SafeDownCast(caller) &&
      event ==  vtkCommand::ModifiedEvent)
    {
    this->InvokeEvent(vtkCommand::ModifiedEvent, NULL);
    }
  return;
}

//----------------------------------------------------------------------------
vtkMRMLSurfaceMapModelHierarchyNode* vtkMRMLSurfaceMapModelHierarchyNode::GetCollapsedParentNode()
{
  vtkMRMLSurfaceMapModelHierarchyNode *node = NULL;
  vtkMRMLDisplayableHierarchyNode *dhnode = Superclass::GetCollapsedParentNode();
  if (dhnode != NULL)
    {
    node = vtkMRMLSurfaceMapModelHierarchyNode::SafeDownCast(dhnode);
    }
  return node;
}


  
//---------------------------------------------------------------------------
void vtkMRMLSurfaceMapModelHierarchyNode:: GetChildrenModelNodes(vtkCollection *models)
{
  if (models == NULL)
    {
    return;
    }
  vtkMRMLScene *scene = this->GetScene();
  vtkMRMLNode *mnode = NULL;
  vtkMRMLSurfaceMapModelHierarchyNode *hnode = NULL;
  for (int n=0; n < scene->GetNumberOfNodes(); n++) 
    {
    mnode = scene->GetNthNode(n);
    if (mnode->IsA("vtkMRMLSurfaceMapModelNode"))
      {
      hnode = vtkMRMLSurfaceMapModelHierarchyNode::SafeDownCast(
          vtkMRMLDisplayableHierarchyNode::GetDisplayableHierarchyNode(scene, mnode->GetID()));
      while (hnode)
        {
        if (hnode == this) 
          {
          models->AddItem(mnode);
          break;
          }
          hnode = vtkMRMLSurfaceMapModelHierarchyNode::SafeDownCast(hnode->GetParentNode());
        }// end while
      }// end if
    }// end for
}


