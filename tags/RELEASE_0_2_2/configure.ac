#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([Osmo], [0.2.2], [pasp@users.sf.net])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_SRCDIR([src/contacts.c])
AC_CONFIG_HEADER([config.h])

# Checks for additional options

AC_MSG_CHECKING(for type of build)
AC_ARG_ENABLE(
	debug,
	[  --enable-debug=yes,no   compile with debugging support (default: no)],
	debug="$enableval", 
	debug="no")
if test "$debug" = "yes"; then
	BUILD_CFLAGS="-rdynamic -ggdb -g -O0"
	AC_DEFINE([DEBUG_BUILD], [1], [Defined if doing a debug build])
fi

# Checks for programs.
AC_PROG_CC

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([libintl.h limits.h stdlib.h string.h sys/time.h unistd.h pthread.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_STRUCT_TM

# Checks for library functions.
AC_FUNC_STAT
AC_FUNC_STRFTIME
AC_CHECK_FUNCS([mkdir strncasecmp])

# Checks for libraries.

AM_PATH_GTK_2_0(2.10.0,,
	AC_MSG_ERROR([GTK+ not found or too old (version < 2.10)]))

AM_PATH_XML2(2.0.0,,
	AC_MSG_ERROR([You do not appear to have libxml2 installed.]))

# Checks for libical
AC_ARG_WITH(libical,
	[[  --without-libical       disable iCal support]],
	[if test "x$withval" = "xno" ; then use_ical=0 ; else use_ical=1; fi],
	use_ical=1)
AC_CHECK_LIB(pthread, pthread_attr_init, [lib=yes], [lib=no])
if test "$lib" = "yes" && test "$use_ical" = "1"; then
    LDFLAGS="${LDFLAGS} -lpthread"
    AC_CHECK_LIB(ical, icalparser_new, [lib=yes], [lib=no])
    if test "$lib" = "yes"; then
        ical_LIBS="-lical -lpthread"
        AC_DEFINE([HAVE_LIBICAL], [1], [Defined if compile with libical support])
        libical="yes"
    else
        libical="no"
    fi
else
    libical="no"
fi

# Checks for libgringotts
AC_ARG_WITH(gringotts,
	[[  --without-gringotts     disable Gringotts support]],
	[if test "x$withval" = "xno" ; then use_gring=0 ; else use_gring=1; fi],
	use_gring=1)
AC_CHECK_LIB(gringotts, grg_context_initialize_defaults, [lib=yes], [lib=no])
if test "$lib" = "yes" && test "$use_gring" = "1"; then
    gringotts_LIBS="-lgringotts"
    AC_DEFINE([HAVE_LIBGRINGOTTS], [1], [Defined if compile with libgringotts support])
    libgringotts="yes"
else
    libgringotts="no"
fi

# Checks for libnotify
PKG_CHECK_MODULES(LIBNOTIFY, libnotify >= 0.4.4, have_libnotify=true,
	have_libnotify=false)
if test "x$have_libnotify" = "xtrue"; then
	AC_DEFINE([HAVE_LIBNOTIFY], [1], [Definded if compile with libnotify support])
else
    AC_MSG_ERROR([libnotify >= 0.4.4 couldn't be found])
fi
AM_CONDITIONAL([HAVE_LIBNOTIFY], test "x$have_libnotify" = "xtrue")

# Checks for libsyncml
AC_ARG_WITH(libsyncml,
	[[  --with-libsyncml        enable experimental SyncML plugin]],
	[if test "x$withval" = "xyes" ; then use_syncml=1 ; else use_syncml=0; fi],
	use_syncml=0)
PKG_CHECK_MODULES(LIBSYNCML, libsyncml-1.0 >= 0.4.0, have_syncml=true,
	have_syncml=false)
libsyncml="no"
if test "x$have_syncml" = "xtrue"; then
if test "x$use_syncml" = "x1"; then
	AC_DEFINE([HAVE_LIBSYNCML], [1], [have SyncML support])
	libsyncml="yes"
fi; fi
AM_CONDITIONAL([HAVE_LIBSYNCML], test "x$libsyncml" = "xyes")

# Checks for additional options
AC_ARG_WITH([config-path],
              AS_HELP_STRING([--with-config-path=PATH], [use a non-default config path]),
              [configpath=$withval AC_SUBST([configpath])])
AM_CONDITIONAL([CONFIGPATH], [test x$configpath != x])
AC_ARG_WITH([config-dir],
              AS_HELP_STRING([--with-config-dir=DIR], [use a non-default config directory]),
              [configdir=$withval AC_SUBST([configdir])])
AM_CONDITIONAL([CONFIGDIR], [test x$configdir != x])

CFLAGS="$BUILD_CFLAGS -Wall $PLATFORM_CFLAGS -D_GNU_SOURCE"
CXXFLAGS="$CFLAGS"
CPPFLAGS=""
LIBS="$ical_LIBS $gringotts_LIBS"

AC_CONFIG_FILES([ 
    Makefile
    src/Makefile
    po/Makefile
])
AC_OUTPUT

echo "
Configuration:

    Prefix:             $prefix
    Debug enabled:      $debug

Optional features:

    libical:            $libical
    libgringotts:       $libgringotts
    libsyncml:          $libsyncml
"
