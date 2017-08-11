#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([powermenu], [1.0.0], [webmaster@thefarbeyond.com])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])

#Enable automake
AM_INIT_AUTOMAKE([-Wall -Werror subdir-objects])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_LIBTOOL

#AM_PATH_GTK_3_0([3.0.12],,AC_MSG_ERROR([Gtk+ 3.0.12 or higher required.]))
PKG_CHECK_MODULES([GTK], [gtk+-3.0])

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile
])
AC_OUTPUT
