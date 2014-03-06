#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(gengdic, 0.1, shyblue@ncsoft.net)
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE
MA_MAINTAINER_MODE

AC_PREFIX_DEFAULT(/usr/local)

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lz':
AC_CHECK_LIB([z], [gzopen])

PKG_CHECK_MODULES([GENGDIC],[gtk+-2.0 libglade-2.0])
pkg_modules="libgnomeui-2.0"

GETTEXT_PACKAGE="libglade-2.0"
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE],"$GETTEXT_PACKAGE",[gettext package])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([libintl.h locale.h stdlib.h string.h unistd.h zlib.h])
# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_STAT
AC_CHECK_FUNCS([setlocale])

AC_OUTPUT([
        Makefile
        src/Makefile
        ])
