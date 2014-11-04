# Microsoft Developer Studio Project File - Name="Sentinel HASP cpp API" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Sentinel HASP cpp API - Win32 Release LIBMT
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libhasp_cpp_windows_msc6.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libhasp_cpp_windows_msc6.mak" CFG="Sentinel HASP cpp API - Win32 Release LIBMT"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Sentinel HASP cpp API - Win32 Debug LIBMT" (based on "Win32 (x86) Static Library")
!MESSAGE "Sentinel HASP cpp API - Win32 Debug LIBMTD" (based on "Win32 (x86) Static Library")
!MESSAGE "Sentinel HASP cpp API - Win32 Debug Diag LIBMT" (based on "Win32 (x86) Static Library")
!MESSAGE "Sentinel HASP cpp API - Win32 Debug Diag LIBMTD" (based on "Win32 (x86) Static Library")
!MESSAGE "Sentinel HASP cpp API - Win32 Release LIBMT" (based on "Win32 (x86) Static Library")
!MESSAGE "Sentinel HASP cpp API - Win32 Release LIBMTD" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Sentinel HASP cpp API - Win32 Debug LIBMT"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug_LIBMT"
# PROP BASE Intermediate_Dir "Debug_LIBMT"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_LIBMT"
# PROP Intermediate_Dir "Debug_LIBMT"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\..\..\inc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /Zl /FD /GZ /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug_LIBMT\libhasp_cpp_windows_mt_msc6_d.lib"

!ELSEIF  "$(CFG)" == "Sentinel HASP cpp API - Win32 Debug LIBMTD"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug_LIBMTD"
# PROP BASE Intermediate_Dir "Debug_LIBMTD"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_LIBMTD"
# PROP Intermediate_Dir "Debug_LIBMTD"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\..\..\inc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /Zl /FD /GZ /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug_LIBMTD\libhasp_cpp_windows_mtd_msc6_d.lib"

!ELSEIF  "$(CFG)" == "Sentinel HASP cpp API - Win32 Debug Diag LIBMT"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug_LIBMT_Diag"
# PROP BASE Intermediate_Dir "Debug_LIBMT_Diag"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_LIBMT_Diag"
# PROP Intermediate_Dir "Debug_LIBMT_Diag"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\..\..\inc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /D "_DEBUG" /D "DIAGNOSTICS" /D "WIN32" /D "_MBCS" /D "_LIB" /Zl /FD /GZ /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug_LIBMT_Diag\libhasp_cpp_windows_mt_msc6_d.lib"

!ELSEIF  "$(CFG)" == "Sentinel HASP cpp API - Win32 Debug Diag LIBMTD"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug_LIBMTD_Diag"
# PROP BASE Intermediate_Dir "Debug_LIBMTD_Diag"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_LIBMTD_Diag"
# PROP Intermediate_Dir "Debug_LIBMTD_Diag"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\..\..\inc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /D "_DEBUG" /D "DIAGNOSTICS" /D "WIN32" /D "_MBCS" /D "_LIB" /Zl /FD /GZ /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug_LIBMTD_Diag\libhasp_cpp_windows_mtd_msc6_d.lib"

!ELSEIF  "$(CFG)" == "Sentinel HASP cpp API - Win32 Release LIBMT"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release_LIBMT"
# PROP BASE Intermediate_Dir "Release_LIBMT"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_LIBMT"
# PROP Intermediate_Dir "Release_LIBMT"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O1 /I "..\..\..\..\inc" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O1 /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /Zl /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Release_LIBMT\libhasp_cpp_windows_mt_msc6.lib"

!ELSEIF  "$(CFG)" == "Sentinel HASP cpp API - Win32 Release LIBMTD"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release_LIBMTD"
# PROP BASE Intermediate_Dir "Release_LIBMTD"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_LIBMTD"
# PROP Intermediate_Dir "Release_LIBMTD"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O1 /I "..\..\..\..\inc" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O1 /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /Zl /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Release_LIBMTD\libhasp_cpp_windows_mtd_msc6.lib"

!ENDIF 

# Begin Target

# Name "Sentinel HASP cpp API - Win32 Debug LIBMT"
# Name "Sentinel HASP cpp API - Win32 Debug LIBMTD"
# Name "Sentinel HASP cpp API - Win32 Debug Diag LIBMT"
# Name "Sentinel HASP cpp API - Win32 Debug Diag LIBMTD"
# Name "Sentinel HASP cpp API - Win32 Release LIBMT"
# Name "Sentinel HASP cpp API - Win32 Release LIBMTD"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\haspbase.cpp
# End Source File
# Begin Source File

SOURCE=..\haspdiag.cpp
# End Source File
# Begin Source File

SOURCE=..\haspenc64.cpp
# End Source File
# Begin Source File

SOURCE=..\haspfeature.cpp
# End Source File
# Begin Source File

SOURCE=..\haspfile.cpp
# End Source File
# Begin Source File

SOURCE=..\hasphandle.cpp
# End Source File
# Begin Source File

SOURCE=..\hasphasp.cpp
# End Source File
# Begin Source File

SOURCE=..\haspimpl.cpp
# End Source File
# Begin Source File

SOURCE=..\haspinfo.cpp
# End Source File
# Begin Source File

SOURCE=..\hasplegacy.cpp
# End Source File
# Begin Source File

SOURCE=..\hasplock.cpp
# End Source File
# Begin Source File

SOURCE=..\haspmain.cpp
# End Source File
# Begin Source File

SOURCE=..\haspmap.cpp
# End Source File
# Begin Source File

SOURCE=..\haspversion.cpp
# End Source File
# Begin Source File

SOURCE=..\hasptime.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\hasp_api.h
# End Source File
# Begin Source File

SOURCE=..\hasp_api_cpp.h
# End Source File
# Begin Source File

SOURCE=..\hasp_api_cpp_.h
# End Source File
# Begin Source File

SOURCE=..\haspdiag.h
# End Source File
# Begin Source File

SOURCE=..\resource.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "rc; rc2"
# Begin Source File

SOURCE=..\haspcpp.rc
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\haspcpp.rc2
# PROP Exclude_From_Build 1
# End Source File
# End Group
# End Target
# End Project
