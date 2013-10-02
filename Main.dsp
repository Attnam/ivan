# Microsoft Developer Studio Project File - Name="Main" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Main - Win32 PowerDebug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Main.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Main.mak" CFG="Main - Win32 PowerDebug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Main - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Main - Win32 PowerDebug" (based on "Win32 (x86) Application")
!MESSAGE "Main - Win32 FastDebug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Main - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Main/Release"
# PROP Intermediate_Dir "Main/Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /vd0 /GX /O2 /Ob2 /I "Main/Include" /I "FeLib/Include" /D "NDEBUG" /D "USE_SDL" /D "WIN32" /D "_WINDOWS" /D "VC" /D "WIZARD" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x40b /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 SDL.lib SDLmain.lib FeLib/Release/FeLib.lib user32.lib /nologo /version:0.500 /subsystem:windows /profile /machine:I386 /nodefaultlib:"libc"
# SUBTRACT LINK32 /nodefaultlib
# Begin Special Build Tool
SOURCE=$(InputPath)
PostBuild_Cmds=copy Main\Release\Main.exe IVAN.exe
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Main - Win32 PowerDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Main___W"
# PROP BASE Intermediate_Dir "Main___W"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Main/PowerDebug"
# PROP Intermediate_Dir "Main/PowerDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GR /GX /Zi /Od /I "Main/Include" /I "Main/Resource" /I "FeLib/Include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "VC" /YX /FD /I /" " /c
# ADD CPP /nologo /MDd /W3 /vd0 /GX /Zi /Od /I "Main/Include" /I "FeLib/Include" /D "_DEBUG" /D "USE_SDL" /D "WIN32" /D "_WINDOWS" /D "VC" /D "WIZARD" /YX /FD /I /" " /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x40b /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 dxguid.lib ddraw.lib FeLib/Debug/FeLib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /profile /debug /machine:I386
# ADD LINK32 SDL.lib SDLmain.lib FeLib/PowerDebug/FeLib.lib user32.lib /nologo /version:0.50 /subsystem:windows /profile /debug /machine:I386 /nodefaultlib:"msvcrt"
# SUBTRACT LINK32 /verbose /map /nodefaultlib
# Begin Special Build Tool
SOURCE=$(InputPath)
PostBuild_Cmds=copy Main\PowerDebug\Main.exe IVAN.exe
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Main - Win32 FastDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Main___0"
# PROP BASE Intermediate_Dir "Main___0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Main/FastDebug"
# PROP Intermediate_Dir "Main/FastDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GR /GX /O2 /Ob2 /I "Main/Include" /I "Main/Resource" /I "FeLib/Include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "VC" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /vd0 /GX /Zi /O2 /Ob2 /I "Main/Include" /I "FeLib/Include" /D "NDEBUG" /D "USE_SDL" /D "WIN32" /D "_WINDOWS" /D "VC" /D "WIZARD" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x40b /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 dxguid.lib ddraw.lib FeLib/Release/FeLib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT BASE LINK32 /pdb:none /debug
# ADD LINK32 SDL.lib SDLmain.lib FeLib/FastDebug/FeLib.lib user32.lib /nologo /version:0.500 /subsystem:windows /debug /machine:I386 /nodefaultlib:"msvcrt" /pdbtype:sept
# SUBTRACT LINK32 /verbose /pdb:none /incremental:yes /map
# Begin Special Build Tool
SOURCE=$(InputPath)
PostBuild_Cmds=copy Main\FastDebug\Main.exe IVAN.exe
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Main - Win32 Release"
# Name "Main - Win32 PowerDebug"
# Name "Main - Win32 FastDebug"
# Begin Group "Source"

# PROP Default_Filter "c,cc,cpp"
# Begin Source File

SOURCE=.\Main\Source\action.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\actions.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\actset.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\area.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\areaset.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\bodypart.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\char.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\charset.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\charsset.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\command.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\cont.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\coreset.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\database.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\dataset.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\dungeon.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\entity.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\fluid.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\game.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\gear.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\god.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\gods.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\godset.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\human.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\iconf.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\id.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\igraph.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\item.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\itemset.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\level.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\levelset.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\lsquare.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\lterra.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\lterras.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\main.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\materia.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\materias.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\materset.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\message.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\miscitem.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\nonhuman.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\object.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\pool.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\proto.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\rain.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\room.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\rooms.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\roomset.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\script.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\slot.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\slotset.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\smoke.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\square.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\stack.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\team.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\terra.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\trap.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\traps.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\trapset.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\wmapset.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\worldmap.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\wskill.cpp
# End Source File
# Begin Source File

SOURCE=.\Main\Source\wsquare.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\wterra.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Main\Source\wterras.cpp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Include"

# PROP Default_Filter "h,hh,hpp"
# Begin Source File

SOURCE=.\Main\Include\action.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\actions.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\area.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\balance.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\bodypart.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\char.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\command.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\confdef.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\cont.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\database.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\dungeon.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\entity.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\fluid.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\game.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\gear.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\god.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\gods.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\human.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\iconf.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\id.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\igraph.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\iloops.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\item.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\ivandef.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\level.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\lock.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\lsquare.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\lterra.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\lterras.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\materia.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\materias.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\message.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\miscitem.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\nonhuman.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\object.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\pool.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\proto.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\rain.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\room.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\rooms.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\script.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\slot.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\smoke.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\square.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\stack.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\team.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\terra.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\trap.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\traps.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\worldmap.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\wskill.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\wsquare.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\wterra.h
# End Source File
# Begin Source File

SOURCE=.\Main\Include\wterras.h
# End Source File
# End Group
# Begin Group "Script"

# PROP Default_Filter "*.dat"
# Begin Source File

SOURCE=.\Script\char.dat
# End Source File
# Begin Source File

SOURCE=.\Script\define.dat
# End Source File
# Begin Source File

SOURCE=.\Script\dungeon.dat
# End Source File
# Begin Source File

SOURCE=.\Script\glterra.dat
# End Source File
# Begin Source File

SOURCE=.\Script\item.dat
# End Source File
# Begin Source File

SOURCE=.\Script\material.dat
# End Source File
# Begin Source File

SOURCE=.\Script\olterra.dat
# End Source File
# End Group
# Begin Group "Misc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ChangeLog
# End Source File
# Begin Source File

SOURCE=.\Main\Resource\Ivan.ico
# End Source File
# Begin Source File

SOURCE=.\Main\Resource\Ivan.rc

!IF  "$(CFG)" == "Main - Win32 Release"

!ELSEIF  "$(CFG)" == "Main - Win32 PowerDebug"

!ELSEIF  "$(CFG)" == "Main - Win32 FastDebug"

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
