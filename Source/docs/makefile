DFLAGS = -nocdsm -norptpri -db cmap.ctf

# list of documentation modules

DOCGUI = ../pm_combobox.h      \
         ../pm_error.h         \
         ../pm_noncopyable.h   \
         ../pm_exclusion.h     \
         ../pm_frame.h         \
         ../pm_messages.h      \
         ../pm_profile.h       \
         ../pm_thread.h        \
         ../pm_window.h        \
         ../pm_gui.h

DOCMAP = ../cmap_about.h       \
         ../cmap_application.h \
         ../cmap_clipboard.h   \
         ../cmap_fonts.h       \
         ../cmap_glyph.h       \
         ../cmap_grid.h        \
         ../cmap_specs.h       \
         ../cmap_status.h

# documentation generation

doc:
  -@erase *.html
  -@erase *.ctf
   @echo  Analyzing GUI...
   @ccdoc $(DFLAGS) $(DOCGUI) -pkg "GUI"
   @echo  Analyzing GUI::Characters Map...
   @ccdoc $(DFLAGS) $(DOCMAP) -pkg "GUI::Characters Map"

   @echo  Indexing...
   @ccdoc -db cmap.ctf -index
   @echo  Generating documentation...
   @ccdoc -db cmap.ctf -root CMap -norptsci -rptdefa "Dmitry A.Steklenev" -norptdpv -rptmac -rptpro -bg \"#FFFFFF\" -fg \"#000000\" -html ./ -srcurl ./

# cleanup

clean:
  -@erase *.ctf
  -@erase *.html
