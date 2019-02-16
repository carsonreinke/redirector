#redirector_SOURCES=query.c

lib_LTLIBRARIES = libredirector.la
libredirector_la_SOURCES = query.c query.h utils.c utils.h uri.c uri.h
libredirector_la_CFLAGS = @CHECK_CFLAGS@ -DDEBUG
libredirector_la_LDFLAGS = -lresolv -lcheck -lidn2 -luriparser

bin_PROGRAMS = main
main_SOURCES = main.c
main_LDADD = libredirector.la