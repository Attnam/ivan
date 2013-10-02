# Microsoft Developer Studio Project File - Name="MIHAIL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MIHAIL - Win32 PowerDebug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MIHAIL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MIHAIL.mak" CFG="MIHAIL - Win32 PowerDebug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MIHAIL - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MIHAIL - Win32 FastDebug" (based on "Win32 (x86) Application")
!MESSAGE "MIHAIL - Win32 PowerDebug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MIHAIL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /Ob2 /I "FeLib/Include" /D "NDEBUG" /D "USE_SDL" /D "WIN32" /D "_WINDOWS" /D "VC" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x40b /d "NDEBUG"
# ADD RSC /l 0x40b /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 SDL.lib SDLmain.lib FeLib/Release/FeLib.lib user32.lib /nologo /subsystem:console /machine:I386 /nodefaultlib:"libc"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE=$(InputPath)
PostBuild_Cmds=copy Release\MIHAIL.exe MIHAIL.exe
# End Special Build Tool

!ELSEIF  "$(CFG)" == "MIHAIL - Win32 FastDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MIHAIL___W"
# PROP BASE Intermediate_Dir "MIHAIL___W"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "FastDebug"
# PROP Intermediate_Dir "FastDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GR /GX /O2 /Ob2 /I "FeLib/Include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "VC" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /O2 /Ob2 /I "FeLib/Include" /D "NDEBUG" /D "USE_SDL" /D "WIN32" /D "_WINDOWS" /D "VC" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x40b /d "NDEBUG"
# ADD RSC /l 0x40b /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 dxguid.lib ddraw.lib FeLib/Release/FeLib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 SDL.lib SDLmain.lib FeLib/FastDebug/FeLib.lib user32.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"msvcrt"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE=$(InputPath)
PostBuild_Cmds=copy FastDebug\MIHAIL.exe MIHAIL.exe
# End Special Build Tool

!ELSEIF  "$(CFG)" == "MIHAIL - Win32 PowerDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MIHAIL___0"
# PROP BASE Intermediate_Dir "MIHAIL___0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PowerDebug"
# PROP Intermediate_Dir "PowerDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GR /GX /Zi /Od /I "Felib/Include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "VC" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I "Felib/Include" /D "_DEBUG" /D "USE_SDL" /D "WIN32" /D "_WINDOWS" /D "VC" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x40b /d "_DEBUG"
# ADD RSC /l 0x40b /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 dxguid.lib ddraw.lib FeLib/Debug/FeLib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 SDL.lib SDLmain.lib FeLib/PowerDebug/FeLib.lib user32.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /nodefaultlib:"msvcrt" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE=$(InputPath)
PostBuild_Cmds=copy PowerDebug\MIHAIL.exe MIHAIL.exe
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "MIHAIL - Win32 Release"
# Name "MIHAIL - Win32 FastDebug"
# Name "MIHAIL - Win32 PowerDebug"
# Begin Group "Source"

# PROP Default_Filter "c,cc,cpp"
# Begin Source File

SOURCE=.\Source\mihail.cpp
# End Source File
# End Group
# Begin Group "Include"

# PROP Default_Filter "h,hh,hpp"
# End Group
# End Target
# End Project
