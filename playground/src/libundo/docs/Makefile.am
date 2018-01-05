if HAVE_DOXYGEN

doxyfile.stamp:
	$(DOXYGEN) Doxyfile
	echo Timestamp > doxyfile.stamp

CLEANFILES = doxyfile.stamp

all-local: doxyfile.stamp

clean-local:
	rm -rf $(builddir)/html
	rm -rf $(builddir)/latex

install-data-local:
	@installfiles=`echo $(builddir)/html/*`; \
	if test "$$installfiles" = '$(builddir)/html/*'; \
	then echo 1>&2 'Nothing to install' ; \
	else \
	  installdir="$(DESTDIR)$(docdir)"; \
	  $(mkinstalldirs) $${installdir} ; \
	  for i in $$installfiles; do \
	    echo ' $(INSTALL_DATA) '$$i ; \
	    $(INSTALL_DATA) $$i $${installdir}; \
	  done; \
	fi

uninstall-local:
	installdir="$(DESTDIR)$(docdir)"; \
	rm -rf $${installdir}

endif
