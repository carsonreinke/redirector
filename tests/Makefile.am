TESTS = check_redirector
check_PROGRAMS = check_redirector
check_redirector_SOURCES = query.c $(top_builddir)/src/query.h
check_redirector_CFLAGS = @CHECK_CFLAGS@
check_redirector_LDFLAGS = -lresolv -lcheck
check_redirector_LDADD = $(top_builddir)/src/libredirector.la @CHECK_LIBS@
