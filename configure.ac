AC_INIT([redirector], [0.0.1], [carson@reinke.co])
AM_PROG_AR

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for libraries.
AC_CHECK_LIB(resolv, res_nquery)
PKG_CHECK_MODULES([CHECK], [check >= 0.10.0])
PKG_CHECK_MODULES([LIBIDN2], [libidn2 >= 2.0.0])
AC_CHECK_LIB(uriparser, uriParseSingleUriA)

# 
AC_HEADER_STDC
AC_CHECK_HEADERS([check.h])
AC_CHECK_HEADERS([idn2.h])
AC_CHECK_HEADERS([uriparser/Uri.h])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
 tests/Makefile
])
AC_OUTPUT