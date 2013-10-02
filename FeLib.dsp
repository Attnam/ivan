# Microsoft Developer Studio Project File - Name="FeLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=FeLib - Win32 PowerDebug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FeLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FeLib.mak" CFG="FeLib - Win32 PowerDebug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FeLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "FeLib - Win32 PowerDebug" (based on "Win32 (x86) Static Library")
!MESSAGE "FeLib - Win32 FastDebug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe

!IF  "$(CFG)" == "FeLib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "FeLib/Release"
# PROP Intermediate_Dir "FeLib/Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /vd0 /GX /O2 /Ob2 /I "FeLib/Include" /D "NDEBUG" /D "USE_SDL" /D "WIN32" /D "_WINDOWS" /D "VC" /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "FeLib - Win32 PowerDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "FeLib___"
# PROP BASE Intermediate_Dir "FeLib___"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "FeLib/PowerDebug"
# PROP Intermediate_Dir "FeLib/PowerDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /I "FeLib/Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "VC" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /vd0 /GX /Zi /Od /Ob0 /I "FeLib/Include" /D "_DEBUG" /D "USE_SDL" /D "WIN32" /D "_WINDOWS" /D "VC" /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "FeLib - Win32 FastDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "FeLib__0"
# PROP BASE Intermediate_Dir "FeLib__0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "FeLib/FastDebug"
# PROP Intermediate_Dir "FeLib/FastDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /Ob2 /I "FeLib/Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "VC" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /vd0 /GX /Zi /O2 /Ob2 /I "FeLib/Include" /D "USE_SDL" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "VC" /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "FeLib - Win32 Release"
# Name "FeLib - Win32 PowerDebug"
# Name "FeLib - Win32 FastDebug"
# Begin Group "Source"

# PROP Default_Filter "c,cc,cpp"
# Begin Source File

SOURCE=.\FeLib\Source\bitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\FeLib\Source\config.cpp
# End Source File
# Begin Source File

SOURCE=.\FeLib\Source\error.cpp
# End Source File
# Begin Source File

SOURCE=.\FeLib\Source\feio.cpp
# End Source File
# Begin Source File

SOURCE=.\FeLib\Source\felist.cpp
# End Source File
# Begin Source File

SOURCE=.\FeLib\Source\femain.cpp
# End Source File
# Begin Source File

SOURCE=.\FeLib\Source\femath.cpp
# End Source File
# Begin Source File

SOURCE=.\FeLib\Source\festring.cpp
# End Source File
# Begin Source File

SOURCE=.\FeLib\Source\fetime.cpp
# End Source File
# Begin Source File

SOURCE=.\FeLib\Source\graphics.cpp
# End Source File
# Begin Source File

SOURCE=.\FeLib\Source\hscore.cpp
# End Source File
# Begin Source File

SOURCE=.\FeLib\Source\rawbit.cpp
# End Source File
# Begin Source File

SOURCE=.\FeLib\Source\save.cpp
# End Source File
# Begin Source File

SOURCE=.\FeLib\Source\whandler.cpp
# End Source File
# End Group
# Begin Group "Include"

# PROP Default_Filter "h,hh,hpp"
# Begin Source File

SOURCE=.\FeLib\Include\allocate.h
# End Source File
# Begin Source File

SOURCE=.\FeLib\Include\bitmap.h
# End Source File
# Begin Source File

SOURCE=.\FeLib\Include\config.h
# End Source File
# Begin Source File

SOURCE=.\FeLib\Include\error.h
# End Source File
# Begin Source File

SOURCE=.\FeLib\Include\fearray.h
# End Source File
# Begin Source File

SOURCE=.\FeLib\Include\feio.h
# End Source File
# Begin Source File

SOURCE=.\FeLib\Include\felibdef.h
# End Source File
# Begin Source File

SOURCE=.\FeLib\Include\felist.h
# End Source File
# Begin Source File

SOURCE=.\FeLib\Include\feloops.h
# End Source File
# Begin Source File

SOURCE=.\FeLib\Include\femath.h
# End Source File
# Begin Source File

SOURCE=.\FeLib\Include\festring.h
# End Source File
# Begin Source File

SOURCE=.\FeLib\Include\fetime.h
# End Source File
# Begin Source File

SOURCE=.\FeLib\Include\graphics.h
# End Source File
# Begin Source File

SOURCE=.\FeLib\Include\hscore.h
# End Source File
# Begin Source File

SOURCE=.\FeLib\Include\pragmas.h
# End Source File
# Begin Source File

SOURCE=.\FeLib\Include\rawbit.h
# End Source File
# Begin Source File

SOURCE=.\FeLib\Include\rect.h
# End Source File
# Begin Source File

SOURCE=.\FeLib\Include\save.h
# End Source File
# Begin Source File

SOURCE=.\FeLib\Include\typedef.h
# End Source File
# Begin Source File

SOURCE=.\FeLib\Include\v2.h
# End Source File
# Begin Source File

SOURCE=.\FeLib\Include\whandler.h
# End Source File
# End Group
# End Target
# End Project
