#
# makefile for the whole Characters Map/2 package
#

PARTS   = chmap
VERSION = 2_0_5

all: $(PARTS)

chmap: 
	cd Source
	@$(MAKE)
	@cd ..

distclean:
	-@del WPI\chmap2-$(VERSION).exe 2> nul
	-@del Bin\chmap.exe 2> nul
	-@del Bin\chmap.cat 2> nul

dist: chmap
	-@del WPI\chmap2-$(VERSION).exe 2> nul
	wic.exe -a WPI\chmap2-$(VERSION).exe 1 changes license readme
	wic.exe -a WPI\chmap2-$(VERSION).exe 1 -cBin chmap.exe chmap.cat -U+ -s WPI\chmap.wis

clean:
	cd Source
	@$(MAKE) clean
	@cd ..
