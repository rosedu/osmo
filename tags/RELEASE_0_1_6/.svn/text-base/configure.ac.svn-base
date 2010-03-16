#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([Osmo], [0.1.6], [pasp@users.sf.net])
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
AC_CHECK_HEADERS([libintl.h limits.h stdlib.h string.h sys/time.h unistd.h])
AC_CHECK_HEADERS([pthread.h ical.h])

# Checks for libraries.

AC_MSG_CHECKING(whether GTK+ version >= 2.10)
if pkg-config --exists 'gtk+-2.0 >= 2.10'; then

    gtk_CFLAGS=`pkg-config --cflags gtk+-2.0`
	gtk_LIBS=`pkg-config --libs gtk+-2.0`
    gtk_VER=`pkg-config --modversion gtk+-2.0`

    AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
	AC_MSG_ERROR(GTK+ not found or too old (version < 2.10))
fi


AC_CHECK_PROG([var], [xml2-config], [yes], [no])
if test ! "$var" = "yes"; then
	AC_MSG_ERROR(You do not appear to have xml2-config installed.)
fi

xml_CFLAGS=`xml2-config --cflags`
xml_LIBS=`xml2-config --libs`
xml_VER=`xml2-config --version`

AC_CHECK_LIB(xml2,
	xmlNewDoc,
	[],
	[AC_MSG_ERROR(You do not appear to have libxml2 installed.)])

AC_CHECK_LIB(pthread, pthread_attr_init, [lib=yes], [lib=no])
if test "$lib" = "yes"; then
    LDFLAGS="${LDFLAGS} -lpthread"
    AC_CHECK_LIB(ical, icalparser_new, [lib=yes], [lib=no])
    if test "$lib" = "yes"; then
        ical_LIBS="-lical -lpthread"
        AC_DEFINE([HAVE_ICAL], [1], [Defined if compile with libical support])
        libical="yes"
    else
        libical="no"
    fi
else
    libical="no"
fi

CFLAGS="$BUILD_CFLAGS -Wall $PLATFORM_CFLAGS -D_GNU_SOURCE"
CXXFLAGS="$CFLAGS"
CPPFLAGS="$gtk_CFLAGS $xml_CFLAGS"
LIBS="$gtk_LIBS $xml_LIBS $ical_LIBS"

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_STRUCT_TM

# Checks for library functions.
AC_FUNC_STAT
AC_FUNC_STRFTIME
AC_CHECK_FUNCS([mkdir strncasecmp])

AC_CONFIG_FILES([ 
    Makefile
    src/Makefile
    po/Makefile
])
AC_OUTPUT

echo "
Configuration:

    GTK+ version:       $gtk_VER
    libxml2 version:    $xml_VER

    Debug enabled:      $debug

Optional features:

    libical:            $libical
"
