# Microsoft Developer Studio Project File - Name="zipios" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=zipios - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "zipios.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "zipios.mak" CFG="zipios - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "zipios - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "zipios - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "zipios - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I ".." /I "d:\home\kev\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x406
# ADD RSC /l 0x406
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"zipios.lib"

!ELSEIF  "$(CFG)" == "zipios - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Z7 /Od /I ".." /I "..\..\zlib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x406
# ADD RSC /l 0x406
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"zipiosd.lib"

!ENDIF 

# Begin Target

# Name "zipios - Win32 Release"
# Name "zipios - Win32 Debug"
# Begin Group "src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\backbuffer.h
# End Source File
# Begin Source File

SOURCE=..\src\basicentry.cpp
# End Source File
# Begin Source File

SOURCE=..\src\collcoll.cpp
# End Source File
# Begin Source File

SOURCE=..\src\deflateoutputstreambuf.cpp
# End Source File
# Begin Source File

SOURCE=..\src\dircoll.cpp
# End Source File
# Begin Source File

SOURCE=..\src\directory.cpp
# End Source File
# Begin Source File

SOURCE=..\src\directory.h
# End Source File
# Begin Source File

SOURCE=..\src\fcoll.cpp
# End Source File
# Begin Source File

SOURCE=..\src\fcollexceptions.cpp
# End Source File
# Begin Source File

SOURCE=..\src\fileentry.cpp
# End Source File
# Begin Source File

SOURCE=..\src\filepath.cpp
# End Source File
# Begin Source File

SOURCE=..\src\filterinputstreambuf.cpp
# End Source File
# Begin Source File

SOURCE=..\src\inflateinputstreambuf.cpp
# End Source File
# Begin Source File

SOURCE=..\src\outputstringstream.h
# End Source File
# Begin Source File

SOURCE=..\src\zipfile.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ziphead.cpp
# End Source File
# Begin Source File

SOURCE=..\src\zipinputstream.cpp
# End Source File
# Begin Source File

SOURCE=..\src\zipinputstreambuf.cpp
# End Source File
# Begin Source File

SOURCE="..\src\zipios_common.h"
# End Source File
# Begin Source File

SOURCE=..\src\zipoutputstream.cpp
# End Source File
# Begin Source File

SOURCE=..\src\zipoutputstreambuf.cpp
# End Source File
# End Group
# Begin Group "zipios++"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\zipios++\basicentry.h"
# End Source File
# Begin Source File

SOURCE="..\zipios++\collcoll.h"
# End Source File
# Begin Source File

SOURCE="..\zipios++\deflateoutputstreambuf.h"
# End Source File
# Begin Source File

SOURCE="..\zipios++\dircoll.h"
# End Source File
# Begin Source File

SOURCE="..\zipios++\fcoll.h"
# End Source File
# Begin Source File

SOURCE="..\zipios++\fcollexceptions.h"
# End Source File
# Begin Source File

SOURCE="..\zipios++\fileentry.h"
# End Source File
# Begin Source File

SOURCE="..\zipios++\filepath.h"
# End Source File
# Begin Source File

SOURCE="..\zipios++\filterinputstreambuf.h"
# End Source File
# Begin Source File

SOURCE="..\zipios++\inflateinputstreambuf.h"
# End Source File
# Begin Source File

SOURCE="..\zipios++\meta-iostreams.h"
# End Source File
# Begin Source File

SOURCE="..\zipios++\simplesmartptr.h"
# End Source File
# Begin Source File

SOURCE="..\zipios++\virtualseeker.h"
# End Source File
# Begin Source File

SOURCE="..\zipios++\zipfile.h"
# End Source File
# Begin Source File

SOURCE="..\zipios++\ziphead.h"
# End Source File
# Begin Source File

SOURCE="..\zipios++\zipinputstream.h"
# End Source File
# Begin Source File

SOURCE="..\zipios++\zipinputstreambuf.h"
# End Source File
# Begin Source File

SOURCE="..\zipios++\zipoutputstream.h"
# End Source File
# Begin Source File

SOURCE="..\zipios++\zipoutputstreambuf.h"
# End Source File
# End Group
# End Target
# End Project
