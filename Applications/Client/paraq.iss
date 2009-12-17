; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define ParaViewBuildDir "C:\projects\build\ParaQ2"
#define ParaViewSourceDir"C:\projects\ParaQ2"
#define QtInstallDir  "C:\Qt\4.1.3"
#define MSDEVHomeDir  "c:\Program Files\Microsoft Visual Studio .NET 2003\SDK\v1.1\Bin"
#define ParaViewVersion "3.0b"

[Setup]
AppName=ParaView
AppVerName=ParaView {#ParaViewVersion}
AppPublisher=Kitware Inc
AppPublisherURL=http://www.paraview.org
AppSupportURL=http://www.paraview.org
AppUpdatesURL=http://www.paraview.org
DefaultDirName={pf}\ParaView {#ParaViewVersion}
DefaultGroupName=ParaView {#ParaViewVersion}
InfoAfterFile={#ParaViewSourceDir}\Applications\Client\README
OutputBaseFilename=ParaView-{#ParaViewVersion}_setup
OutputDir=C:\projects\release\paraq\{#ParaViewVersion}\win32
Compression=lzma
SolidCompression=yes

[Languages]
Name: english; MessagesFile: compiler:Default.isl

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked
Name: quicklaunchicon; Description: {cm:CreateQuickLaunchIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked

[Files]
Source: {#ParaViewBuildDir}\bin\release\pqClient.exe; DestDir: {app}; Flags: ignoreversion
Source: {#QtInstallDir}\bin\QtGui4.dll; DestDir: {app}
Source: {#QtInstallDir}\bin\QtCore4.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\QtPython.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\QtTesting.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\pqWidgets.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\QVTK.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\pqWidgets.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\pqPython.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\KWCommon.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkPVServerManager.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkPVFiltersCS.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkPVFilters.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkPVServerCommonCS.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkPVServerCommon.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkXdmfCS.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkWidgetsCS.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkParallelCS.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkParallel.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkHybridCS.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkHybrid.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkVolumeRenderingCS.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkVolumeRendering.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkRenderingCS.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkRendering.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkGenericFilteringCS.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkGenericFiltering.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkGraphicsCS.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkGraphics.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkImagingCS.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkImaging.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkIOCS.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkIO.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkFilteringCS.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkFiltering.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkCommonCS.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkCommon.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtksys.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkDICOMParser.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkpng.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtktiff.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkjpeg.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkzlib.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkexpat.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkMPEG2Encode.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkzlib.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkexoIIc.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkfreetype.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkftgl.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkClientServer.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkCommonPython.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkhdf5.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkNetCDF.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkPVServerCommonPython.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkPVServerManagerPython.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkWidgets.dll; DestDir: {app}
Source: {#ParaViewBuildDir}\bin\release\vtkXdmf.dll; DestDir: {app}
Source: {#MSDEVHomeDir}\msvcp71.dll; DestDir: {app}
Source: {#MSDEVHomeDir}\msvcr71.dll; DestDir: {app}
Source: README; DestDir: {app}; Flags: ignoreversion

[Icons]
Name: {group}\ParaView; Filename: {app}\pqClient.exe; WorkingDir: {app}
Name: {userdesktop}\ParaView; Filename: {app}\pqClient.exe; Tasks: desktopicon; WorkingDir: {app}
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\ParaView; Filename: {app}\pqClient.exe; Tasks: quicklaunchicon; WorkingDir: {app}

[Registry]
; remove settings for all versions ...
; beta settings
Root: HKCU; SubKey: Software\Kitware Inc; Flags: uninsdeletekeyifempty


[Run]
Filename: {app}\pqClient.exe; Description: {cm:LaunchProgram,ParaView}; Flags: nowait postinstall skipifsilent