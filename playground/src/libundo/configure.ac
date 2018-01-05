#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)


m4_define([undo_major_version], [0])
m4_define([undo_minor_version], [7])
m4_define([undo_version],
          [undo_major_version.undo_minor_version])

# This is the X.Y used in -lundo-X.Y
m4_define([undo_api_version], [1.0])

# required versions of other packages
m4_define([glib_required_version], [2.32.0])

AC_INIT([undo], [undo_version])

AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE(no-define dist-bzip2)
AC_PROG_CXX(cl KCC CC cxx cc++ xlC aCC c++ g++ egcs gcc)

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_PROG_CC
AM_PROG_LIBTOOL

CFLAGS="$CFLAGS -Wall"
CXXFLAGS="$CXXFLAGS -Wall -std=c++0x"

AM_PATH_GLIB_2_0(glib_required_version,,, gobject gio gthread)

PKG_CHECK_MODULES(UNDO, glib-2.0)

AC_SUBST(UNDO_LIBS)
AC_SUBST(UNDO_CFLAGS)


# Disable deprecated APIs

AC_MSG_CHECKING([if GLib is version 2.33.0 or newer])
if $PKG_CONFIG --atleast-version=2.33.0 glib-2.0; then
  have_glib_2_33=yes
else
  have_glib_2_33=no
fi
AC_MSG_RESULT($have_glib_2_33)

if test "x$have_glib_2_33" != "xyes"; then
  CPPFLAGS="${CPPFLAGS} -DG_DISABLE_DEPRECATED"
fi


UNDO_MAJOR_VERSION=undo_major_version
UNDO_MINOR_VERSION=undo_minor_version
AC_SUBST(UNDO_MAJOR_VERSION)
AC_SUBST(UNDO_MINOR_VERSION)

UNDO_API_VERSION=undo_api_version
AC_SUBST(UNDO_API_VERSION)

GLIB_REQUIRED_VERSION=glib_required_version
AC_SUBST(GLIB_REQUIRED_VERSION)



undodatadir="$datadir/undo-undo_api_version"
AC_SUBST(undodatadir)

AC_CONFIG_COMMANDS([undo/version.h], [
  outfile=version.h-tmp
  cat > $outfile <<\_______EOF
/* version.h
 *
 * This is a generated file.  Please modify 'configure.ac'
 */

#ifndef __UNDO_VERSION_H__
#define __UNDO_VERSION_H__


#define UNDO_MAJOR_VERSION  ($undo_major_ver)
#define UNDO_MINOR_VERSION  ($undo_minor_ver)
#define UNDO_VERSION        "$undo_ver"
#define UNDO_API_VERSION    "$undo_api_ver"


#endif /* __UNDO_VERSION_H__ */
_______EOF

  if cmp -s $outfile undo/version.h; then
    AC_MSG_NOTICE([undo/version.h is unchanged])
    rm -f $outfile
  else
    mv $outfile undo/version.h
  fi
],[
  undo_major_ver=undo_major_version
  undo_minor_ver=undo_minor_version
  undo_ver=undo_version
  undo_api_ver=undo_api_version
])


AC_CONFIG_FILES([
Makefile
undo/Makefile
docs/Makefile
])

# Files with versions in their names
AC_CONFIG_FILES(
undo-undo_api_version.pc:undo.pc.in
)

AC_CHECK_PROGS([DOXYGEN], [doxygen])
if test -z "$DOXYGEN";
   then AC_MSG_WARN([Doxygen not found - continuing without Doxygen support])
fi

AM_CONDITIONAL([HAVE_DOXYGEN],
[test -n "$DOXYGEN"])AM_COND_IF([HAVE_DOXYGEN], [AC_CONFIG_FILES([docs/Doxyfile])])

AC_OUTPUT
