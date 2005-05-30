/*=========================================================================

  Program:   ParaView
  Module:    vtkPVAnimationCue.cxx

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkPVAnimationCue.h"

#include "vtkObjectFactory.h"
#include "vtkAnimationCue.h"
#include "vtkKWApplication.h"
#include "vtkPVTimeLine.h"
#include "vtkKWLabel.h"
#include "vtkKWFrame.h"
#include "vtkCommand.h"
#include "vtkKWParameterValueFunctionEditor.h"
#include "vtkKWTkUtilities.h"
#include "vtkKWEvent.h"
#include "vtkSMAnimationCueProxy.h"
#include "vtkSMKeyFrameAnimationCueManipulatorProxy.h"
#include "vtkPVKeyFrame.h"
#include "vtkPVRampKeyFrame.h"
#include "vtkCollection.h"
#include "vtkCollectionIterator.h"
#include "vtkSMProxyManager.h"
#include "vtkSMProxyProperty.h"
#include "vtkSMKeyFrameProxy.h"
#include "vtkPVApplication.h"
#include "vtkPVWindow.h"
#include "vtkPVAnimationManager.h"
#include "vtkPVVerticalAnimationInterface.h"
#include "vtkPVAnimationScene.h"
#include "vtkPVSource.h"
#include "vtkSMPropertyStatusManager.h"
#include "vtkSMVectorProperty.h"
#include "vtkSMIdTypeVectorProperty.h"
#include "vtkSMIntVectorProperty.h"
#include "vtkSMDoubleVectorProperty.h"
#include "vtkSMStringVectorProperty.h"
#include "vtkKWRange.h"
#include "vtkPVTraceHelper.h"

#include <vtkstd/string>
#define VTK_PV_ANIMATON_ENTRY_HEIGHT 20

/* 
 * This part was generated by ImageConvert from image:
 *    close.png (zlib, base64)
 */
#define image_close_width         9
#define image_close_height        9
#define image_close_pixel_size    4
#define image_close_buffer_length 48

static unsigned char image_close[] = 
  "eNpjYGD4z0AEBgIGXBibGmx8UtTgcgMt7CLkL0IYANH+oGA=";


/* 
 * This part was generated by ImageConvert from image:
 *    open.png (zlib, base64)
 */
#define image_open_width         9
#define image_open_height        9
#define image_open_pixel_size    4
#define image_open_buffer_length 40

static unsigned char image_open[] = 
  "eNpjYGD4z0AEBgIGXJgWanC5YSDcQwgDAO0pqFg=";

vtkStandardNewMacro(vtkPVAnimationCue);
vtkCxxRevisionMacro(vtkPVAnimationCue, "1.31");
vtkCxxSetObjectMacro(vtkPVAnimationCue, TimeLineParent, vtkKWWidget);
vtkCxxSetObjectMacro(vtkPVAnimationCue, PVSource, vtkPVSource);

//***************************************************************************
//Helper methods to down cast the property and set value.
inline static int DoubleVectPropertySetElement(vtkSMProxy *proxy, 
  const char* propertyname, double val, int index = 0)
{
  vtkSMDoubleVectorProperty* dvp = vtkSMDoubleVectorProperty::SafeDownCast(
    proxy->GetProperty(propertyname));
  if (!dvp)
    {
    return 0;
    }
  return dvp->SetElement(index, val);
}

//-----------------------------------------------------------------------------
inline static int StringVectPropertySetElement(vtkSMProxy *proxy, 
  const char* propertyname, const char* val, int index = 0)
{
  vtkSMStringVectorProperty* dvp = vtkSMStringVectorProperty::SafeDownCast(
    proxy->GetProperty(propertyname));
  if (!dvp)
    {
    return 0;
    }
  return dvp->SetElement(index, val);
}

//-----------------------------------------------------------------------------
inline static int IntVectPropertySetElement(vtkSMProxy *proxy, 
  const char* propertyname, int val, int index = 0)
{
  vtkSMIntVectorProperty* dvp = vtkSMIntVectorProperty::SafeDownCast(
    proxy->GetProperty(propertyname));
  if (!dvp)
    {
    return 0;
    }
  return dvp->SetElement(index, val);
}

//-----------------------------------------------------------------------------
vtkPVAnimationCue::vtkPVAnimationCue()
{
  this->TimeLineParent = NULL;
  this->TimeLineContainer = vtkKWFrame::New();
  this->Label = vtkKWLabel::New();
  this->TimeLine = vtkPVTimeLine::New();
  this->TimeLine->GetTraceHelper()->SetReferenceHelper(this->GetTraceHelper());
  this->TimeLine->GetTraceHelper()->SetReferenceCommand("GetTimeLine");

  this->ImageType = vtkPVAnimationCue::NONE;
  this->Image = vtkKWLabel::New();
  this->Frame = vtkKWFrame::New();
  this->TimeLineFrame = vtkKWFrame::New();
  this->ShowTimeLine = 1;
  this->Focus = 0;

  this->PVAnimationScene = NULL;
  this->PVSource = NULL;

  this->Name = NULL;
  this->TclNameCommand = 0;
  this->CueVisibility = 1;
}


//-----------------------------------------------------------------------------
vtkPVAnimationCue::~vtkPVAnimationCue()
{
  this->SetPVSource(NULL);
  //  this->UnregisterProxies();

  this->SetTimeLineParent(0);
  this->TimeLineContainer->Delete();
  this->Label->Delete();
  this->TimeLine->Delete();
  this->Image->Delete();
  this->Frame->Delete();
  this->TimeLineFrame->Delete();

  this->SetAnimationScene(NULL);

  this->SetName(NULL);
  this->SetTclNameCommand(0);
}

//-----------------------------------------------------------------------------
void vtkPVAnimationCue::SetLabelText(const char* label)
{
  this->Superclass::SetLabelText(label);
  this->Label->SetText(label);
}


//-----------------------------------------------------------------------------
void vtkPVAnimationCue::SetAnimationScene(vtkPVAnimationScene* scene)
{
  this->PVAnimationScene = scene;
}

//-----------------------------------------------------------------------------
void vtkPVAnimationCue::RegisterProxies()
{
  if (this->ProxiesRegistered)
    {
    return;

    }
  this->Superclass::RegisterProxies();
  if (this->ProxiesRegistered)
    {
    if (this->PVAnimationScene)
      {
      this->PVAnimationScene->AddAnimationCue(this);
      }
    }
}

//-----------------------------------------------------------------------------
void vtkPVAnimationCue::UnregisterProxies()
{
  if (!this->ProxiesRegistered)
    {
    return;
    }
  this->Superclass::UnregisterProxies();
  if (this->PVAnimationScene)
    {
    this->PVAnimationScene->RemoveAnimationCue(this);
    }
}

//-----------------------------------------------------------------------------
void vtkPVAnimationCue::SetEnableZoom(int zoom)
{
  this->TimeLine->SetShowParameterRange(zoom);
}

//-----------------------------------------------------------------------------
void vtkPVAnimationCue::DeleteKeyFrame(int id)
{
  this->TimeLine->RemovePoint(id);
}

//-----------------------------------------------------------------------------
void vtkPVAnimationCue::SetTimeMarker(double time)
{
  this->TimeLine->SetTimeMarker(time);
}

//-----------------------------------------------------------------------------
double vtkPVAnimationCue::GetTimeMarker()
{
  return this->TimeLine->GetTimeMarker();
}

//-----------------------------------------------------------------------------
void vtkPVAnimationCue::ReplaceKeyFrame(vtkPVKeyFrame* oldFrame, 
  vtkPVKeyFrame* newFrame)
{
  if (this->Virtual)
    {
    vtkErrorMacro("Cue has no actual keyframes.");
    return;
    }
  // Removing a point can change its selection. So, we save the current 
  // selection and restore it.
  int selection_id = this->TimeLine->GetSelectedPoint();
  this->Superclass::ReplaceKeyFrame(oldFrame, newFrame);  
  this->TimeLine->SelectPoint(selection_id);
}

//-----------------------------------------------------------------------------
void vtkPVAnimationCue::Create(vtkKWApplication* app, const char* args)
{
  if (this->IsCreated())
    {
    vtkErrorMacro("Widget already created.");
    return;
    }

  if (!this->TimeLineParent)
    {
    vtkErrorMacro("TimeLineParent must be set");
    return;
    }

  this->CreateWidget(app, "frame", args);
  
  this->TimeLineContainer->SetParent(this->TimeLineParent);
  this->TimeLineContainer->Create(app, NULL);
  
  this->TimeLineFrame->SetParent(this->TimeLineContainer);
  this->TimeLine->SetParameterCursorInteractionStyle(
    vtkKWParameterValueFunctionEditor::ParameterCursorInteractionStyleDragWithLeftButton |
    vtkKWParameterValueFunctionEditor::ParameterCursorInteractionStyleSetWithRighButton |
    vtkKWParameterValueFunctionEditor::ParameterCursorInteractionStyleSetWithControlLeftButton);

  ostrstream tf_options;
  tf_options << "-height " << VTK_PV_ANIMATON_ENTRY_HEIGHT << ends;
  this->TimeLineFrame->Create(app, tf_options.str());
  tf_options.rdbuf()->freeze(0);
  
  // Create the time line associated with this entry.
  this->TimeLine->SetShowLabel(0);
  this->TimeLine->SetCanvasHeight(VTK_PV_ANIMATON_ENTRY_HEIGHT);
  this->TimeLine->SetPointMarginToCanvas(
    vtkKWParameterValueFunctionEditor::PointMarginHorizontalSides);
  this->TimeLine->SetAnimationCue(this);
  this->TimeLine->SetParent(this->TimeLineFrame);
  this->TimeLine->Create(app, 0);
  this->TimeLine->SetCanvasOutlineStyle(
    vtkKWParameterValueFunctionEditor::CanvasOutlineStyleHorizontalSides |
    vtkKWParameterValueFunctionEditor::CanvasOutlineStyleBottomSide);

  this->Frame->SetParent(this);
  ostrstream frame_options;
  int height = (this->TimeLine->GetShowParameterRange())? 
    this->TimeLine->GetParameterRange()->GetThickness() : 0;
  frame_options << "-relief flat -height " 
    << this->TimeLine->GetCanvasHeight() + height
    << ends;
  this->Frame->Create(app,frame_options.str());
  frame_options.rdbuf()->freeze(0);
  
  this->Label->SetParent(this->Frame);
  this->Label->Create(app, args);

  this->Script("pack propagate %s 0", this->Frame->GetWidgetName());
  this->Script("bind %s <ButtonPress-1> {%s GetFocus}",
    this->Label->GetWidgetName(), this->GetTclName());
  this->Image->SetParent(this->Frame);
  this->Image->Create(app, "-relief flat");
  this->SetImageType(this->ImageType);
  this->InitializeObservers(this->TimeLine);
}

//-----------------------------------------------------------------------------
void vtkPVAnimationCue::InitializeObservers(vtkObject* object)
{
  this->Observe(object, vtkKWParameterValueFunctionEditor::PointMovedEvent);
  this->Observe(object, vtkKWParameterValueFunctionEditor::PointMovingEvent);
  this->Observe(object, vtkKWParameterValueFunctionEditor::SelectionChangedEvent);
  this->Observe(object, vtkKWEvent::FocusInEvent);
  this->Observe(object, vtkKWEvent::FocusOutEvent);
  this->Observe(object, vtkPVSimpleAnimationCue::KeysModifiedEvent);
  this->Observe(object, vtkKWParameterValueFunctionEditor::ParameterCursorMovedEvent);
  this->Observe(object, vtkKWParameterValueFunctionEditor::ParameterCursorMovingEvent);
  if (this->TimeLine->GetShowParameterRange())
    {
    this->Observe(object, 
      vtkKWParameterValueFunctionEditor::VisibleParameterRangeChangedEvent);
    this->Observe(object,
      vtkKWParameterValueFunctionEditor::VisibleParameterRangeChangingEvent);
    }
}

//-----------------------------------------------------------------------------
void vtkPVAnimationCue::Zoom(double range[2])
{
  double old_range[2];
  this->TimeLine->GetVisibleParameterRange(old_range);
  if (old_range[0] != range[0] || old_range[1] != range[1])
    {
    this->TimeLine->SetVisibleParameterRange(range);
    }
}

//-----------------------------------------------------------------------------
void vtkPVAnimationCue::ExecuteEvent(vtkObject* wdg, unsigned long event, void* calldata)
{
  vtkPVApplication* pvApp = vtkPVApplication::SafeDownCast(
    this->GetApplication());
  vtkPVWindow* pvWin = pvApp->GetMainWindow();
  vtkPVAnimationManager* pvAM = pvWin->GetAnimationManager(); 

  if (wdg == this->TimeLine)
    {
    switch(event)
      {
    case vtkKWEvent::FocusInEvent:
      this->GetFocus();
      return;

    case vtkKWEvent::FocusOutEvent:
      // NOTE: we are removing the focus of only self and not that of
      // the children (if any), since the focus presently was on self,
      // otherwise FocusOutEvent would have never been triggerred.
      this->RemoveSelfFocus();
      return;

    case vtkKWParameterValueFunctionEditor::SelectionChangedEvent:
      // raise this event on this cue, so that the VAnimationInterface (if
      // it is listening) will know that selection has changed and will
      // update to show the right key frame.
      //this->InvokeEvent(event, calldata);
      
      this->SelectKeyFrameInternal(this->TimeLine->GetSelectedPoint());
      return;

    case vtkKWParameterValueFunctionEditor::ParameterCursorMovingEvent:
        {
        double param = this->TimeLine->GetParameterCursorPosition();
        pvAM->SetTimeMarker(param);
        }
      return;
    case vtkKWParameterValueFunctionEditor::ParameterCursorMovedEvent:
        {
        double param = this->TimeLine->GetParameterCursorPosition();
        pvAM->SetCurrentTime(param);
        }
      }
    }
  else if (vtkSMKeyFrameAnimationCueManipulatorProxy::SafeDownCast(wdg))
    {
    switch (event)
      {
    case vtkCommand::ModifiedEvent:
      // Triggerred when the keyframes have been changed in someway.
      this->TimeLine->ForceUpdate();
      if (this->PVAnimationScene)
        {
        this->PVAnimationScene->InvalidateAllGeometries();
        }
      break;
      }
    }
  this->Superclass::ExecuteEvent(wdg, event, calldata);
}

//-----------------------------------------------------------------------------
void vtkPVAnimationCue::PackWidget()
{
  int label_frame_width = 1;
  if (!this->IsCreated())
    {
    vtkErrorMacro("Widget must be created before packing");
    return;
    }
  
  if (this->ShowTimeLine)
    {
    this->Script("pack %s -anchor n -side top -fill x -expand t",
      this->TimeLine->GetWidgetName());
    }

  this->Script("pack %s -anchor n -side top -fill x -expand t",
    this->TimeLineFrame->GetWidgetName());

  this->Script("pack %s -anchor n -side top -fill x -expand t",
    this->TimeLineContainer->GetWidgetName());


  if (this->ImageType != vtkPVAnimationCue::NONE)
    {
    this->Script("pack %s -anchor w -side left",
      this->Image->GetWidgetName());
    label_frame_width += 
      atoi(this->Script("winfo reqwidth %s", this->Image->GetWidgetName()));
    }
  
  this->Script("pack %s -anchor w -side left",
    this->Label->GetWidgetName());
  label_frame_width += 
    atoi(this->Script("winfo reqwidth %s", this->Label->GetWidgetName()));

  this->Script("pack %s -anchor nw -side top -fill x -expand t",
    this->Frame->GetWidgetName());
  
  this->Script("pack %s -anchor n -side top -fill x -expand t",
    this->GetWidgetName());

  // Set the label width properly, since we have disabled pack propagate on the frame
  // containig the label.
  if (label_frame_width != 1)
    {
    ostrstream str;
    // the addition 50 is added to take care for the expansion of label 
    // when it becomes bold.
    str << "-width " << (label_frame_width + 50) << ends;
    this->Frame->ConfigureOptions(str.str());
    str.rdbuf()->freeze(0);
    }
}

//-----------------------------------------------------------------------------
void vtkPVAnimationCue::UnpackWidget()
{
  if (!this->IsCreated())
    {
    vtkErrorMacro("Widget must be created before unpacking");
    return;
    }
  this->Script("pack forget %s",
    this->TimeLine->GetWidgetName());

  this->Script("pack forget %s",
    this->TimeLineFrame->GetWidgetName());

  this->Script("pack forget %s",
    this->TimeLineContainer->GetWidgetName());

  this->Script("pack forget %s",
    this->Image->GetWidgetName());
  
  this->Script("pack forget %s",
    this->Label->GetWidgetName());

  this->Script("pack forget %s",
    this->GetWidgetName()); 
}

//-----------------------------------------------------------------------------
void vtkPVAnimationCue::SetImageType(int type)
{
  if (this->IsCreated())
    {
    switch (type)
      {
    case vtkPVAnimationCue::NONE:
      break;
    case vtkPVAnimationCue::IMAGE_OPEN:
      this->Image->SetImageOption(
        image_open,
        image_open_width,
        image_open_height,
        image_open_pixel_size,
        image_open_buffer_length);
      break;
    case vtkPVAnimationCue::IMAGE_CLOSE:
      this->Image->SetImageOption(
        image_close,
        image_close_width,
        image_close_height,
        image_close_pixel_size,
        image_close_buffer_length);
      break;
    default:
      vtkErrorMacro("Invalid image type " << type);
      return;
      }
    }
  this->ImageType = type;
}
//-----------------------------------------------------------------------------
void vtkPVAnimationCue::SetTimeBounds(double bounds[2], int enable_scaling)
{
  this->TimeLine->MoveStartToParameter(bounds[0], enable_scaling);
  this->TimeLine->MoveEndToParameter(bounds[1], enable_scaling);
}

//-----------------------------------------------------------------------------
int vtkPVAnimationCue::GetTimeBounds(double * bounds)
{
  return this->TimeLine->GetParameterBounds(bounds);
}

//-----------------------------------------------------------------------------
void vtkPVAnimationCue::GetFocus()
{
  if (!this->Focus)
    {
    this->GetSelfFocus();
    }
  this->GetTraceHelper()->AddEntry("$kw(%s) GetFocus", this->GetTclName());
}

//-----------------------------------------------------------------------------
void vtkPVAnimationCue::GetSelfFocus()
{
  this->Focus = 1;
  // TODO: change color
  vtkKWTkUtilities::ChangeFontWeightToBold(
    this->GetApplication()->GetMainInterp(), this->Label->GetWidgetName());
  this->TimeLine->GetFocus();
  this->InvokeEvent(vtkKWEvent::FocusInEvent);
}

//-----------------------------------------------------------------------------
void vtkPVAnimationCue::RemoveFocus()
{
  if (this->Focus)
    {
    this->RemoveSelfFocus();
    }
}

//-----------------------------------------------------------------------------
void vtkPVAnimationCue::RemoveSelfFocus()
{
  this->Focus = 0;
  this->TimeLine->RemoveFocus();
  // TODO: change color
  vtkKWTkUtilities::ChangeFontWeightToNormal(
    this->GetApplication()->GetMainInterp(), this->Label->GetWidgetName());
  this->InvokeEvent(vtkKWEvent::FocusOutEvent);
}

//-----------------------------------------------------------------------------
void vtkPVAnimationCue::StartRecording()
{
  if (this->InRecording)
    {
    return;
    }
  this->Superclass::StartRecording();
  if (!this->Virtual)
    {
    this->TimeLine->DisableAddAndRemoveOn();
    }
}

//-----------------------------------------------------------------------------
void vtkPVAnimationCue::StopRecording()
{
  if (!this->InRecording)
    {
    return;
    }

  this->Superclass::StopRecording();

  if (!this->Virtual)
    {
    this->TimeLine->SetDisableAddAndRemove(0);
    }
}

//-----------------------------------------------------------------------------
void vtkPVAnimationCue::RecordState(double ntime, double offset,
  int onlyFocus)
{
  if (!this->InRecording)
    {
    vtkErrorMacro("Not in recording mode.");  
    return;
    }
  
  if (onlyFocus && !this->HasFocus())
    {
    return;
    }

  this->TimeLine->DisableAddAndRemoveOff();
  this->Superclass::RecordState(ntime, offset); 
  this->TimeLine->DisableAddAndRemoveOn();
}

//-----------------------------------------------------------------------------
void vtkPVAnimationCue::Detach()
{
  this->RemoveAllKeyFrames();
  this->RemoveFocus();
  this->UnpackWidget();
  this->SetParent(NULL);
  // We don't explictly remove from Scene or delete all key frames
  // since that happens when the cue is destroyed.
  // Also, that will help us catch any forgetten reference to the cue.
}

//-----------------------------------------------------------------------------
void vtkPVAnimationCue::RemoveAllKeyFrames()
{
  if (this->Virtual)
    {
    return;
    }
  // Don;t directly remove the keyframes...instead pretend that 
  // the timeline nodes are being deleted.
  this->TimeLine->RemoveAll();
}

//-----------------------------------------------------------------------------
void vtkPVAnimationCue::SelectKeyFrame(int id)
{
  this->TimeLine->SelectPoint(id);
//  this->Superclass::SelectKeyFrame(id); don't call Superclass::SelectKeyFrame
//  it will get called as a result of 
//  vtkKWParameterValueFunctionEditor::SelectionChangedEvent
}

//-----------------------------------------------------------------------------
void vtkPVAnimationCue::UpdateCueVisibility(int advanced)
{
  if (this->Virtual)
    {
    // cannot determine visibility for virtual cues.
    return;
    }
  
  vtkSMProperty* property = this->CueProxy->GetAnimatedProperty();
  if (!property)
    {
    return;
    }
  if (advanced || property->GetAnimateable() == 1)
    {
    this->CueVisibility = 1;
    }
  else
    {
    this->CueVisibility = 0;
    }
}

//-----------------------------------------------------------------------------
void vtkPVAnimationCue::UpdateEnableState()
{
  this->Superclass::UpdateEnableState();
  
  this->PropagateEnableState(this->TimeLineParent);
  this->PropagateEnableState(this->Label);
  this->PropagateEnableState(this->Image);
  this->PropagateEnableState(this->Frame);
  this->PropagateEnableState(this->TimeLineContainer);
  this->PropagateEnableState(this->TimeLineFrame);
  this->PropagateEnableState(this->TimeLine);
}

//-----------------------------------------------------------------------------
const char* vtkPVAnimationCue::GetTclNameCommand()
{
  if (this->TclNameCommand)
    {
    return this->TclNameCommand;
    }

  this->SetTclNameCommand(0);
  if (!this->Name | !this->PVSource)
    {
    this->SetTclNameCommand(this->Name);
    return this->TclNameCommand;
    }

  vtkstd::string str = vtkstd::string(this->Name);
  vtkstd::string::size_type sindex = str.find(this->PVSource->GetName());
  if (sindex == vtkstd::string::npos)
    {
    this->SetTclNameCommand(this->Name);
    return this->TclNameCommand;
    }

  ostrstream command;
  command << "[$kw(" << this->PVSource->GetTclName() << ") GetName]" << ends;
  
  str.replace(sindex, strlen(this->PVSource->GetName()),  command.str());
  command.rdbuf()->freeze(0);

  this->SetTclNameCommand(str.c_str());
  return this->TclNameCommand;
}

//-----------------------------------------------------------------------------
void vtkPVAnimationCue::SaveState(ofstream* file)
{
  // All the properties are automatically set except the keyframes
  // So, save the keyframe states. Also, keyframes are saved only when they
  // are non-virtual.
  if (this->Focus)
    {
    *file << "$kw(" << this->GetTclName() << ") GetFocus" << endl;
    }
  if (this->Virtual)
    {
    return;
    }

  // We do remove all keyframes in the state file to get rid of any
  // default animations added while creating PVSources. We must remove the
  // animations, since the state saves the default animations (if any) also.
  *file << "$kw(" << this->GetTclName() << ") RemoveAllKeyFrames" << endl;

  vtkPVApplication* pvApp = vtkPVApplication::SafeDownCast(
    this->GetApplication());
  vtkPVWindow* pvWin = pvApp->GetMainWindow();
  vtkPVAnimationManager* pvAM = pvWin->GetAnimationManager(); 

  vtkCollectionIterator* iter = this->PVKeyFrames->NewIterator();
  for (iter->InitTraversal(); !iter->IsDoneWithTraversal(); iter->GoToNextItem())
    {
    vtkPVKeyFrame* pvKF = vtkPVKeyFrame::SafeDownCast(iter->GetCurrentObject());
    *file << endl; 
    *file << "set tempid [$kw(" << this->GetTclName() << ") CreateAndAddKeyFrame " << pvKF->GetKeyTime()
      << " " << pvAM->GetKeyFrameType(pvKF) << "]" << endl;
    *file << "set kw(" << pvKF->GetTclName() << ") [$kw(" << this->GetTclName() <<
      ") GetKeyFrame $tempid ]" << endl;
    pvKF->SaveState(file);
    }
  iter->Delete();
}

//-----------------------------------------------------------------------------
int vtkPVAnimationCue::CreateAndAddKeyFrame(double time, int type)
{
  int keyid = this->Superclass::CreateAndAddKeyFrame(time, type);
  if (keyid < 0)
    {
    return keyid;
    }
  vtkPVKeyFrame* keyf = this->GetKeyFrame(keyid);
  vtkPVApplication* pvApp = vtkPVApplication::SafeDownCast(
    this->GetApplication());
  vtkPVWindow* pvWin = pvApp->GetMainWindow();
  vtkPVAnimationManager* pvAM = pvWin->GetAnimationManager(); 

  keyf->SetAnimationScene(pvAM->GetAnimationScene());
  return keyid;
}

//-----------------------------------------------------------------------------
void vtkPVAnimationCue::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "Name: " << ((this->Name) ? this->Name : "NULL") << endl;
  os << indent << "ImageType: " << this->ImageType << endl;
  os << indent << "ShowTimeLine: " << this->ShowTimeLine << endl;
  os << indent << "Focus: " << this->Focus << endl;
  os << indent << "PVAnimationScene: " << this->PVAnimationScene << endl;
  os << indent << "PVSource: " << this->PVSource << endl;
  os << indent << "TimeLine: " << this->TimeLine << endl;
  os << indent << "CueVisibility: " << this->CueVisibility << endl;
}
