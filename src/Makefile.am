#redirector_SOURCES=query.c

lib_LTLIBRARIES = libredirector.la
libredirector_la_SOURCES = query.c query.h
libredirector_la_LDFLAGS = -lresolv -lcheck -lidn2

bin_PROGRAMS = main
main_SOURCES = main.c
main_LDADD = libredirector.la