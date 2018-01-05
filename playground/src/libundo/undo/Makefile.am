## Process this file with automake to produce Makefile.in

libundoincludedir = $(includedir)/undo-$(UNDO_API_VERSION)/undo

AM_CPPFLAGS = \
	-DDATADIR=\""$(datadir)"\"		\
	-DSYSCONFDIR=\""$(sysconfdir)"\"	\
	-DG_LOG_DOMAIN=\"Undo\"		\
	-I$(top_srcdir)				\
	$(UNDO_CFLAGS)

lib_LTLIBRARIES = libundo-1.0.la

libundo_sources = \
	ActionCommand.cpp\
	ActionCommand.h\
	Command.cpp\
	Command.h\
	Scope.cpp\
	Scope.h\
	Transaction.cpp\
	Transaction.h\
	TransactionCreationScope.cpp\
	TransactionCreationScope.h

libundo_1_0_la_SOURCES = $(libundo_sources)

libundoinclude_HEADERS = \
	ActionCommand.h\
	Command.h\
	Scope.h\
	Transaction.h\
	TransactionCreationScope.h

libundo_1_0_la_LIBADD = $(UNDO_LIBS)

#
# rules to generate built sources
#

gen_sources = xgen-sec xgen-smc xgen-smh
CLEANFILES = $(EXTRA_PROGRAMS) $(gen_sources)
