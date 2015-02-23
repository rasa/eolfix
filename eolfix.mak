# Microsoft Developer Studio Generated NMAKE File, Based on eolfix.dsp
!IF "$(CFG)" == ""
CFG=eolfix - Win32 Debug
!MESSAGE No configuration specified. Defaulting to eolfix - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "eolfix - Win32 Release" && "$(CFG)" != "eolfix - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "eolfix.mak" CFG="eolfix - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "eolfix - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "eolfix - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "eolfix - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\eolfix.exe"


CLEAN :
	-@erase "$(INTDIR)\eolfix.obj"
	-@erase "$(INTDIR)\eolfix.res"
	-@erase "$(INTDIR)\fnmatch.obj"
	-@erase "$(INTDIR)\getopt.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\eolfix.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "win32" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\eolfix.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\eolfix.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\eolfix.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=binmode.obj kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\eolfix.pdb" /machine:I386 /out:"$(OUTDIR)\eolfix.exe" 
LINK32_OBJS= \
	"$(INTDIR)\eolfix.obj" \
	"$(INTDIR)\fnmatch.obj" \
	"$(INTDIR)\getopt.obj" \
	"$(INTDIR)\eolfix.res"

"$(OUTDIR)\eolfix.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "eolfix - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\eolfix.exe" "$(OUTDIR)\eolfix.bsc"


CLEAN :
	-@erase "$(INTDIR)\eolfix.obj"
	-@erase "$(INTDIR)\eolfix.res"
	-@erase "$(INTDIR)\eolfix.sbr"
	-@erase "$(INTDIR)\fnmatch.obj"
	-@erase "$(INTDIR)\fnmatch.sbr"
	-@erase "$(INTDIR)\getopt.obj"
	-@erase "$(INTDIR)\getopt.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\eolfix.bsc"
	-@erase "$(OUTDIR)\eolfix.exe"
	-@erase "$(OUTDIR)\eolfix.ilk"
	-@erase "$(OUTDIR)\eolfix.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "win32" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\eolfix.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\eolfix.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\eolfix.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\eolfix.sbr" \
	"$(INTDIR)\fnmatch.sbr" \
	"$(INTDIR)\getopt.sbr"

"$(OUTDIR)\eolfix.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=binmode.obj kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\eolfix.pdb" /debug /machine:I386 /out:"$(OUTDIR)\eolfix.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\eolfix.obj" \
	"$(INTDIR)\fnmatch.obj" \
	"$(INTDIR)\getopt.obj" \
	"$(INTDIR)\eolfix.res"

"$(OUTDIR)\eolfix.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("eolfix.dep")
!INCLUDE "eolfix.dep"
!ELSE 
!MESSAGE Warning: cannot find "eolfix.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "eolfix - Win32 Release" || "$(CFG)" == "eolfix - Win32 Debug"
SOURCE=.\eolfix.c

!IF  "$(CFG)" == "eolfix - Win32 Release"


"$(INTDIR)\eolfix.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "eolfix - Win32 Debug"


"$(INTDIR)\eolfix.obj"	"$(INTDIR)\eolfix.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\fnmatch.c

!IF  "$(CFG)" == "eolfix - Win32 Release"


"$(INTDIR)\fnmatch.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "eolfix - Win32 Debug"


"$(INTDIR)\fnmatch.obj"	"$(INTDIR)\fnmatch.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\getopt.c

!IF  "$(CFG)" == "eolfix - Win32 Release"


"$(INTDIR)\getopt.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "eolfix - Win32 Debug"


"$(INTDIR)\getopt.obj"	"$(INTDIR)\getopt.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\eolfix.rc

"$(INTDIR)\eolfix.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

