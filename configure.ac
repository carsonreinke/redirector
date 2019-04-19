AC_INIT([redirector], [1.0], [carson@reinke.co])
AM_PROG_AR

AM_INIT_AUTOMAKE([-Wall -Wextra -Werror foreign])

#?
#AC_CONFIG_MACRO_DIRS([m4])

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
#AC_CHECK_HEADERS([check.h])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
 tests/Makefile
])
AC_OUTPUT