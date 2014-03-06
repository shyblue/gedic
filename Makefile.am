SUBDIRS = src

EXTRA_DIST = engdic \
			 pixmaps

install-data-local:
	@$(NORMAL_INSTALL)
	if test -d $(srcdir)/pixmaps; then \
      $(mkinstalldirs) $(datadir)/pixmaps/$(PACKAGE); \
      for pixmap in $(srcdir)/pixmaps/*; do \
        if test -f $$pixmap; then \
          $(INSTALL_DATA) $$pixmap $(datadir)/pixmaps/$(PACKAGE); \
        fi \
      done \
    fi
	if test -d $(srcdir)/engdic; then \
	  $(mkinstalldirs) $(datadir)/engdic; \
	  for dicdata in $(srcdir)/engdic/*; do \
	    if test -f $$dicdata; then \
		  $(INSTALL_DATA) $$dicdata $(datadir)/engdic/; \
		fi \
	  done \
	fi
