# Microsoft Developer Studio Project File - Name="BidsIsapi" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 61000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BidsIsapi - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BidsIsapi.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BidsIsapi.mak" CFG="BidsIsapi - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BidsIsapi - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BidsIsapi - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BidsIsapi - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE ComPlus 0
# PROP BASE Target_Dir ""
# PROP BASE Debug_Exe ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP ComPlus 0
# PROP Target_Dir ""
# PROP Debug_Exe ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BIDSISAPI_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo
# ADD CPP /MT
# ADD CPP /W3
# ADD CPP /GX
# ADD CPP /O2
# ADD CPP /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BIDSISAPI_EXPORTS"
# ADD CPP /Yu"stdafx.h"
# ADD CPP /FD
# ADD CPP /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo
# ADD MTL /D "NDEBUG"
# ADD MTL /mktyplib203
# ADD MTL /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409
# ADD RSC /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
# ADD LINK32 /nologo
# ADD LINK32 /subsystem:windows
# ADD LINK32 /dll
# ADD LINK32 /machine:I386

!ELSEIF  "$(CFG)" == "BidsIsapi - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE ComPlus 0
# PROP BASE Target_Dir ""
# PROP BASE Debug_Exe ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP ComPlus 0
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# PROP Debug_Exe ""
# ADD BASE CPP /nologo /MTd /RTC1 /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BIDSISAPI_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo
# ADD CPP /MTd
# ADD CPP /W3
# ADD CPP /Gm
# ADD CPP /GX
# ADD CPP /ZI
# ADD CPP /Od
# ADD CPP /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BIDSISAPI_EXPORTS"
# ADD CPP /Yu"stdafx.h"
# ADD CPP /FD
# ADD CPP /c
# SUBTRACT CPP /RTC<none>
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo
# ADD MTL /D "_DEBUG"
# ADD MTL /mktyplib203
# ADD MTL /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409
# ADD RSC /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
# ADD LINK32 /nologo
# ADD LINK32 /subsystem:windows
# ADD LINK32 /dll
# ADD LINK32 /debug
# ADD LINK32 /machine:I386

!ENDIF 

# Begin Target

# Name "BidsIsapi - Win32 Release"
# Name "BidsIsapi - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BidsIsapi.cpp
# End Source File
# Begin Source File

SOURCE=.\BidsIsapi.def
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
