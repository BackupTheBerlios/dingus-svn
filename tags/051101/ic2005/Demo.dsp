# Microsoft Developer Studio Project File - Name="Demo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Demo - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Demo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Demo.mak" CFG="Demo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Demo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Demo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Demo - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "build/release"
# PROP Intermediate_Dir "build/release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "../dingus" /I "../dingus/lib" /I "src/extern/ode/inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x427 /d "NDEBUG"
# ADD RSC /l 0x427 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 d3dx9.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib d3d9.lib winmm.lib ../dingus/lib/lua.lib dinput8.lib dxguid.lib version.lib src/extern/bass.lib /nologo /subsystem:windows /machine:I386 /out:"inoutside.exe"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "Demo - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "build/debug"
# PROP Intermediate_Dir "build/debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../dingus" /I "../dingus/lib" /I "src/extern/ode/inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x427 /d "_DEBUG"
# ADD RSC /l 0x427 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 d3dx9dt.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib d3d9.lib winmm.lib ../dingus/lib/lua.lib dinput8.lib dxguid.lib version.lib src/extern/bass.lib /nologo /subsystem:windows /debug /machine:I386 /out:"inoutside_d.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Demo - Win32 Release"
# Name "Demo - Win32 Debug"
# Begin Group "demo"

# PROP Default_Filter "cpp;h;c"
# Begin Group "wallz"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\demo\wallz\AABox2.h
# End Source File
# Begin Source File

SOURCE=.\src\demo\wallz\FractureScenario.cpp
# End Source File
# Begin Source File

SOURCE=.\src\demo\wallz\FractureScenario.h
# End Source File
# Begin Source File

SOURCE=.\src\demo\wallz\Physics.cpp
# End Source File
# Begin Source File

SOURCE=.\src\demo\wallz\Physics.h
# End Source File
# Begin Source File

SOURCE=.\src\demo\wallz\Quadtree.h
# End Source File
# Begin Source File

SOURCE=.\src\demo\wallz\Triangulate.cpp

!IF  "$(CFG)" == "Demo - Win32 Release"

!ELSEIF  "$(CFG)" == "Demo - Win32 Debug"

# ADD CPP /Od

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\demo\wallz\Triangulate.h
# End Source File
# Begin Source File

SOURCE=.\src\demo\wallz\WallFracturer.cpp

!IF  "$(CFG)" == "Demo - Win32 Release"

!ELSEIF  "$(CFG)" == "Demo - Win32 Debug"

# ADD CPP /Od

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\demo\wallz\WallFracturer.h
# End Source File
# Begin Source File

SOURCE=.\src\demo\wallz\WallPhysics.cpp

!IF  "$(CFG)" == "Demo - Win32 Release"

!ELSEIF  "$(CFG)" == "Demo - Win32 Debug"

# ADD CPP /Od

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\demo\wallz\WallPhysics.h
# End Source File
# Begin Source File

SOURCE=.\src\demo\wallz\WallPieces.cpp

!IF  "$(CFG)" == "Demo - Win32 Release"

!ELSEIF  "$(CFG)" == "Demo - Win32 Debug"

# ADD CPP /Od

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\demo\wallz\WallPieces.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\demo\ComplexStuffEntity.cpp
# End Source File
# Begin Source File

SOURCE=.\src\demo\ComplexStuffEntity.h
# End Source File
# Begin Source File

SOURCE=.\src\demo\ControllableCharacter.cpp
# End Source File
# Begin Source File

SOURCE=.\src\demo\ControllableCharacter.h
# End Source File
# Begin Source File

SOURCE=.\src\demo\Demo.cpp
# End Source File
# Begin Source File

SOURCE=.\src\demo\Demo.h
# End Source File
# Begin Source File

SOURCE=.\src\demo\DemoResources.h
# End Source File
# Begin Source File

SOURCE=.\src\demo\Entity.cpp
# End Source File
# Begin Source File

SOURCE=.\src\demo\Entity.h
# End Source File
# Begin Source File

SOURCE=.\src\demo\MeshEntity.cpp
# End Source File
# Begin Source File

SOURCE=.\src\demo\MeshEntity.h
# End Source File
# Begin Source File

SOURCE=.\src\demo\PostProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\src\demo\PostProcess.h
# End Source File
# Begin Source File

SOURCE=.\src\demo\Scene.cpp
# End Source File
# Begin Source File

SOURCE=.\src\demo\Scene.h
# End Source File
# Begin Source File

SOURCE=.\src\demo\SceneInteractive.cpp
# End Source File
# Begin Source File

SOURCE=.\src\demo\SceneInteractive.h
# End Source File
# Begin Source File

SOURCE=.\src\demo\SceneMain.cpp
# End Source File
# Begin Source File

SOURCE=.\src\demo\SceneMain.h
# End Source File
# Begin Source File

SOURCE=.\src\demo\SceneScroller.cpp
# End Source File
# Begin Source File

SOURCE=.\src\demo\SceneScroller.h
# End Source File
# Begin Source File

SOURCE=.\src\demo\SceneShared.cpp
# End Source File
# Begin Source File

SOURCE=.\src\demo\SceneShared.h
# End Source File
# Begin Source File

SOURCE=.\src\demo\ThirdPersonCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\src\demo\ThirdPersonCamera.h
# End Source File
# Begin Source File

SOURCE=.\src\demo\Tweaker.cpp
# End Source File
# Begin Source File

SOURCE=.\src\demo\Tweaker.h
# End Source File
# End Group
# Begin Group "system"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\system\Globals.cpp
# End Source File
# Begin Source File

SOURCE=.\src\system\Globals.h
# End Source File
# Begin Source File

SOURCE=.\src\system\main.cpp
# End Source File
# Begin Source File

SOURCE=.\src\system\MusicPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\system\MusicPlayer.h
# End Source File
# Begin Source File

SOURCE=.\src\system\System.cpp
# End Source File
# Begin Source File

SOURCE=.\src\system\System.h
# End Source File
# End Group
# Begin Group "fx"

# PROP Default_Filter "fx"
# Begin Group "lib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\data\fx\lib\commonWalls.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\lib\defines.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\lib\dof.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\lib\global.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\lib\shadowmap.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\lib\shared.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\lib\skinning.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\lib\structs.fx
# End Source File
# End Group
# Begin Source File

SOURCE=.\data\fx\attack2.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\bed.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\bedanim.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\casterSimple.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\casterSimpleA.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\compositeDOF.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\debug.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\electricity.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\electricityAtk.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\electricityAtk2.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\fadeinmesh.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\filterBloom.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\filterGaussX.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\filterGaussY.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\filterPoisson.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\normalMap.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\normalMapL.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\normalMaps.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\scrollerFloor.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\skin.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\skinCaster.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\skinCasterSimple.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\skinRefl.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\wall_Dn.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\wall_DnR.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\wall_DnS.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\wall_DnSA.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\wall_DnSR.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\wall_St.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\wall_StR.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\wall_StS.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\wall_StS2R.fx
# End Source File
# Begin Source File

SOURCE=.\data\fx\walls.fx
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\Demo.rc
# End Source File
# Begin Source File

SOURCE=.\src\DirectX.ico
# End Source File
# Begin Source File

SOURCE=.\src\logo.bmp
# End Source File
# Begin Source File

SOURCE=.\readme.txt
# End Source File
# Begin Source File

SOURCE=.\src\resource.h
# End Source File
# Begin Source File

SOURCE=.\src\stdafx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\src\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\todo.txt
# End Source File
# End Target
# End Project
