/*=========================================================================

  Program:   ParaView
  Module:    vtkSIIntVectorProperty.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkSIIntVectorProperty
// .SECTION Description
// ServerSide Property use to set int array as method argument.


#ifndef __vtkSIIntVectorProperty_h
#define __vtkSIIntVectorProperty_h

#include "vtkPVServerImplementationCoreModule.h" //needed for exports
#include "vtkSIVectorProperty.h"
#include "vtkSIVectorPropertyTemplate.h" // real superclass

#ifndef __WRAP__
#define vtkSIVectorProperty vtkSIVectorPropertyTemplate<int>
#endif
class VTKPVSERVERIMPLEMENTATIONCORE_EXPORT vtkSIIntVectorProperty : public vtkSIVectorProperty
#ifndef __WRAP__
#undef vtkSIVectorProperty
#endif
{
public:
  static vtkSIIntVectorProperty* New();
  vtkTypeMacro(vtkSIIntVectorProperty, vtkSIVectorProperty);
  void PrintSelf(ostream& os, vtkIndent indent);

//BTX
protected:
  vtkSIIntVectorProperty();
  ~vtkSIIntVectorProperty();

private:
  vtkSIIntVectorProperty(const vtkSIIntVectorProperty&); // Not implemented
  void operator=(const vtkSIIntVectorProperty&); // Not implemented
//ETX
};

#endif
