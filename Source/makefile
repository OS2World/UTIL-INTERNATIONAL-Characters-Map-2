CFLAGS=-Gm -Q+ -W3 -Wuse+ -O+ -Gl+ -I..
LFLAGS=/B"/OPTF /PM:PM /EXEP:2"

!ifdef DEBUG
CFLAGS= $(CFLAGS) -Ti+
!endif

.cpp.obj:
    $(CC) $(CFLAGS) /c /Fo$@ $< 

all: ..\bin\chmap.exe ..\bin\chmap.cat

!include depends

..\bin\chmap.cat: cmap_001.msg cmap_007.msg cmap_049.msg
    if not exist ..\Bin mkdir ..\Bin
    gencat $@ $**

..\bin\chmap.exe: $(OBJLST) cmap.res
    if not exist ..\Bin mkdir ..\Bin
    $(CC) $(CFLAGS) $(LFLAGS) /Fe$@ @<< 
    $(OBJLST)
<<
    rc -n cmap.res $*.exe

cmap.res: cmap.rc cmap_ids.h
    rc -n -r $*.rc $*.res

clean:
   @-erase *.o*
   @-erase *.res
