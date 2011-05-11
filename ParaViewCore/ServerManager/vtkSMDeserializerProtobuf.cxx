/*=========================================================================

  Program:   ParaView
  Module:    vtkSMDeserializerProtobuf.cxx

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkSMDeserializerProtobuf.h"

#include "vtkObjectFactory.h"
#include "vtkPVXMLElement.h"
#include "vtkSmartPointer.h"
#include "vtkSMProxy.h"
#include "vtkSMProxyLocator.h"
#include "vtkSMProxyManager.h"
#include "vtkSMSession.h"
#include "vtkSMStateLocator.h"
#include "vtkSMMessage.h"

vtkStandardNewMacro(vtkSMDeserializerProtobuf);
vtkCxxSetObjectMacro(vtkSMDeserializerProtobuf, StateLocator, vtkSMStateLocator);
//----------------------------------------------------------------------------
vtkSMDeserializerProtobuf::vtkSMDeserializerProtobuf()
{
  this->StateLocator = 0;
}

//----------------------------------------------------------------------------
vtkSMDeserializerProtobuf::~vtkSMDeserializerProtobuf()
{
  this->SetStateLocator(0);
}

//----------------------------------------------------------------------------
vtkSMProxy* vtkSMDeserializerProtobuf::NewProxy(vtkTypeUInt32 id, vtkSMProxyLocator* locator)
{
  // Make sure that the requested proxy does not already exist
  assert( "SMDeserializer should not create a proxy if that proxy exist" &&
          !this->Session->GetRemoteObject(id));

  vtkSMMessage msg;
  // First extract state for the requested proxy
  // Did not find the proxy, start the creation procedure
  if (!this->StateLocator || !this->StateLocator->FindState(id, &msg))
    {
    return 0;
    }

  // Try to NOT create a new proxy if already exist and just load the state
  vtkSMProxy* proxy =
      vtkSMProxy::SafeDownCast(this->Session->GetRemoteObject(id));
  if(proxy)
    {
    if(this->StateLocator->FindState(id, &msg))
      {
      proxy->LoadState(&msg, locator);
      }
    return proxy;
    }


  const char* group = msg.GetExtension(ProxyState::xml_group).c_str();
  const char* type = msg.GetExtension(ProxyState::xml_name).c_str();
  const char* subProxyName = (msg.HasExtension(ProxyState::xml_sub_proxy_name))
                             ? msg.GetExtension(ProxyState::xml_sub_proxy_name).c_str():
                             NULL;

  if (!type)
    {
    vtkErrorMacro("Could not create proxy from element, missing 'type'.");
    return 0;
    }

  // Create Proxy based on its XML definition
  proxy = this->CreateProxy(group, type, subProxyName);
  if (!proxy)
    {
    vtkErrorMacro("Could not create a proxy of group: "
                  << (group ? group : "(null)")
                  << " type: " << type
                  << " subProxyName: "
                  << (subProxyName ? subProxyName : "(null)"));
    return 0;
    }

  // Load the state of the proxy now
  proxy->LoadState(&msg, locator);
  proxy->UpdateVTKObjects();

  return proxy;
}

//----------------------------------------------------------------------------
void vtkSMDeserializerProtobuf::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}
