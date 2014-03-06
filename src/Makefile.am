INCLUDE = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@
	
bin_PROGRAMS = gengdic

gengdic_SOURCES = \
	gengdic.c gengdic.h eggtrayicon.c eggtrayicon.h

gengdic_LDFLAGS = -export-dynamic
gengdic_LDADD = @GENGDIC_LIBS@ $(INTLLIBS)
gengdic_CFLAGS = @GENGDIC_CFLAGS@ -D_DATADIR_=\"@datadir@\"
