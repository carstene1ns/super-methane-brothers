# Microsoft Developer Studio Project File - Name="methane" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=methane - Win32 DebugSound
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "methane.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "methane.mak" CFG="methane - Win32 DebugSound"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "methane - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "methane - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "methane - Win32 DebugSound" (based on "Win32 (x86) Application")
!MESSAGE "methane - Win32 ReleaseSound" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "methane - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "METHANE_OLD_CPP_NEW" /D "_AFXDLL" /FD /Zm1000 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "methane - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "METHANE_OLD_CPP_NEW" /D "_AFXDLL" /FD /GZ /Zm1000 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "methane - Win32 DebugSound"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "methane___Win32_DebugSound"
# PROP BASE Intermediate_Dir "methane___Win32_DebugSound"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugSound"
# PROP Intermediate_Dir "DebugSound"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "METHANE_OLD_CPP_NEW" /FD /GZ /Zm1000 /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "METHANE_OLD_CPP_NEW" /D "METHANE_MIKMOD" /D "_AFXDLL" /FD /GZ /Zm1000 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 libmikmodd.lib dsound.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "methane - Win32 ReleaseSound"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "methane___Win32_ReleaseSound"
# PROP BASE Intermediate_Dir "methane___Win32_ReleaseSound"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseSound"
# PROP Intermediate_Dir "ReleaseSound"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "METHANE_OLD_CPP_NEW" /FD /Zm1000 /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "METHANE_OLD_CPP_NEW" /D "METHANE_MIKMOD" /D "_AFXDLL" /FD /Zm1000 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 libmikmodr.lib dsound.lib /nologo /subsystem:windows /machine:I386

!ENDIF 

# Begin Target

# Name "methane - Win32 Release"
# Name "methane - Win32 Debug"
# Name "methane - Win32 DebugSound"
# Name "methane - Win32 ReleaseSound"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\about.cpp
# End Source File
# Begin Source File

SOURCE=..\..\mikmod\audiodrv.cpp
# End Source File
# Begin Source File

SOURCE=..\..\baddie.cpp
# End Source File
# Begin Source File

SOURCE=..\..\bitdraw.cpp
# End Source File
# Begin Source File

SOURCE=..\..\bitgroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\bititem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\boss.cpp
# End Source File
# Begin Source File

SOURCE=.\doc.cpp
# End Source File
# Begin Source File

SOURCE=.\frame.cpp
# End Source File
# Begin Source File

SOURCE=..\..\game.cpp
# End Source File
# Begin Source File

SOURCE=..\..\gasobj.cpp
# End Source File
# Begin Source File

SOURCE=..\..\data\gfxdata.cpp
# End Source File
# Begin Source File

SOURCE=..\..\data\gfxdata2.cpp
# End Source File
# Begin Source File

SOURCE=..\..\data\gfxdata3.cpp
# End Source File
# Begin Source File

SOURCE=..\..\gfxoff.cpp
# End Source File
# Begin Source File

SOURCE=..\..\global.cpp
# End Source File
# Begin Source File

SOURCE=..\..\goodie.cpp
# End Source File
# Begin Source File

SOURCE=.\help.cpp
# End Source File
# Begin Source File

SOURCE=..\..\data\mapdata.cpp
# End Source File
# Begin Source File

SOURCE=..\..\maps.cpp
# End Source File
# Begin Source File

SOURCE=.\methane.cpp
# End Source File
# Begin Source File

SOURCE=..\..\misc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\objlist.cpp
# End Source File
# Begin Source File

SOURCE=..\..\player.cpp
# End Source File
# Begin Source File

SOURCE=..\..\power.cpp
# End Source File
# Begin Source File

SOURCE=..\..\data\snddata.cpp
# End Source File
# Begin Source File

SOURCE=..\..\sound.cpp
# End Source File
# Begin Source File

SOURCE=.\speed.cpp
# End Source File
# Begin Source File

SOURCE=..\..\suck.cpp
# End Source File
# Begin Source File

SOURCE=..\..\target.cpp
# End Source File
# Begin Source File

SOURCE=.\view.cpp
# End Source File
# Begin Source File

SOURCE=..\..\weapon.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\about.h
# End Source File
# Begin Source File

SOURCE=..\..\mikmod\audiodrv.h
# End Source File
# Begin Source File

SOURCE=..\..\baddie.h
# End Source File
# Begin Source File

SOURCE=..\..\bitdraw.h
# End Source File
# Begin Source File

SOURCE=..\..\bitgroup.h
# End Source File
# Begin Source File

SOURCE=..\..\bititem.h
# End Source File
# Begin Source File

SOURCE=..\..\boss.h
# End Source File
# Begin Source File

SOURCE=.\doc.h
# End Source File
# Begin Source File

SOURCE=.\frame.h
# End Source File
# Begin Source File

SOURCE=..\..\game.h
# End Source File
# Begin Source File

SOURCE=..\..\gasobj.h
# End Source File
# Begin Source File

SOURCE=..\..\gfxdef.h
# End Source File
# Begin Source File

SOURCE=..\..\global.h
# End Source File
# Begin Source File

SOURCE=..\..\goodie.h
# End Source File
# Begin Source File

SOURCE=.\help.h
# End Source File
# Begin Source File

SOURCE=..\..\mapdef.h
# End Source File
# Begin Source File

SOURCE=..\..\maps.h
# End Source File
# Begin Source File

SOURCE=.\methane.h
# End Source File
# Begin Source File

SOURCE=..\..\misc.h
# End Source File
# Begin Source File

SOURCE=..\..\objlist.h
# End Source File
# Begin Source File

SOURCE=..\..\objtypes.h
# End Source File
# Begin Source File

SOURCE=..\..\player.h
# End Source File
# Begin Source File

SOURCE=..\..\power.h
# End Source File
# Begin Source File

SOURCE=..\..\snddef.h
# End Source File
# Begin Source File

SOURCE=..\..\sound.h
# End Source File
# Begin Source File

SOURCE=.\speed.h
# End Source File
# Begin Source File

SOURCE=..\..\suck.h
# End Source File
# Begin Source File

SOURCE=..\..\target.h
# End Source File
# Begin Source File

SOURCE=.\view.h
# End Source File
# Begin Source File

SOURCE=..\..\weapon.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\methane.rc
# End Source File
# End Group
# End Target
# End Project
