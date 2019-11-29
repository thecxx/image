# Microsoft Developer Studio Project File - Name="image" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=image - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "image.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "image.mak" CFG="image - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "image - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "image - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "image - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "image - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "image - Win32 Release"
# Name "image - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "asm"

# PROP Default_Filter ""
# Begin Group "x64"

# PROP Default_Filter ""
# End Group
# Begin Group "x86"

# PROP Default_Filter ""
# Begin Group "inc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\traps\trap.inc
# End Source File
# End Group
# Begin Source File

SOURCE=.\traps\GetModuleFileNameA.asm

!IF  "$(CFG)" == "image - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=.\traps\GetModuleFileNameA.asm
InputName=GetModuleFileNameA

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml /coff /c /Fo $(OutDir)\$(InputName).obj $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "image - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=.\traps\GetModuleFileNameA.asm
InputName=GetModuleFileNameA

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml /coff /c /Fo $(OutDir)\$(InputName).obj $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\traps\GetModuleFileNameW.asm

!IF  "$(CFG)" == "image - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=.\traps\GetModuleFileNameW.asm
InputName=GetModuleFileNameW

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml /coff /c /Fo $(OutDir)\$(InputName).obj $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "image - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=.\traps\GetModuleFileNameW.asm
InputName=GetModuleFileNameW

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml /coff /c /Fo $(OutDir)\$(InputName).obj $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\traps\GetModuleHandleA.asm

!IF  "$(CFG)" == "image - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=.\traps\GetModuleHandleA.asm
InputName=GetModuleHandleA

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml /coff /c /Fo $(OutDir)\$(InputName).obj $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "image - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=.\traps\GetModuleHandleA.asm
InputName=GetModuleHandleA

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml /coff /c /Fo $(OutDir)\$(InputName).obj $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\traps\GetModuleHandleW.asm

!IF  "$(CFG)" == "image - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=.\traps\GetModuleHandleW.asm
InputName=GetModuleHandleW

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml /coff /c /Fo $(OutDir)\$(InputName).obj $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "image - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=.\traps\GetModuleHandleW.asm
InputName=GetModuleHandleW

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml /coff /c /Fo $(OutDir)\$(InputName).obj $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\traps\GetProcAddress.asm

!IF  "$(CFG)" == "image - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=.\traps\GetProcAddress.asm
InputName=GetProcAddress

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml /coff /c /Fo $(OutDir)\$(InputName).obj $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "image - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=.\traps\GetProcAddress.asm
InputName=GetProcAddress

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml /coff /c /Fo $(OutDir)\$(InputName).obj $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\image.cpp
# End Source File
# Begin Source File

SOURCE=.\mm.cpp
# End Source File
# Begin Source File

SOURCE=.\pe.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\image.h
# End Source File
# Begin Source File

SOURCE=.\mm.h
# End Source File
# Begin Source File

SOURCE=.\pe.h
# End Source File
# Begin Source File

SOURCE=.\trap.h
# End Source File
# End Group
# End Target
# End Project
