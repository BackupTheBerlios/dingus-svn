# Microsoft Developer Studio Project File - Name="AtlasCreationTool" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=AtlasCreationTool - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AtlasCreationTool.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AtlasCreationTool.mak" CFG="AtlasCreationTool - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AtlasCreationTool - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "AtlasCreationTool - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AtlasCreationTool - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x427 /d "NDEBUG"
# ADD RSC /l 0x427 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib d3d9.lib d3dx9.lib advapi32.lib gdi32.lib setargv.obj /nologo /subsystem:console /machine:I386 /out:"AtlasCreationTool.exe"

!ELSEIF  "$(CFG)" == "AtlasCreationTool - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ  /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ  /c
# ADD BASE RSC /l 0x427 /d "_DEBUG"
# ADD RSC /l 0x427 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib d3d9.lib d3dx9d.lib advapi32.lib gdi32.lib setargv.obj /nologo /subsystem:console /debug /machine:I386 /out:"AtlasCreationTool_d.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "AtlasCreationTool - Win32 Release"
# Name "AtlasCreationTool - Win32 Debug"
# Begin Group "src"

# PROP Default_Filter "cpp;c;h"
# Begin Source File

SOURCE=.\AtlasContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\AtlasContainer.h
# End Source File
# Begin Source File

SOURCE=.\CmdLineOptions.cpp
# End Source File
# Begin Source File

SOURCE=.\CmdLineOptions.h
# End Source File
# Begin Source File

SOURCE=.\Packer.cpp
# End Source File
# Begin Source File

SOURCE=.\Packer.h
# End Source File
# Begin Source File

SOURCE=.\TATypes.h
# End Source File
# Begin Source File

SOURCE=.\TextureAtlasTool.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureObject.cpp
# End Source File
# Begin Source File

SOURCE=.\TextureObject.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\changes.txt
# End Source File
# Begin Source File

SOURCE=.\readme.txt
# End Source File
# End Target
# End Project
