# [open]lipc - Makefile.am
# Copyright (c) 2016 Arkadiusz Bokowy

AM_CFLAGS = -I$(top_srcdir)/include
LDADD = -llipc

bin_PROGRAMS =

if WITH_LIPC_PROP
bin_PROGRAMS += lipc-get-prop lipc-set-prop
endif

if WITH_LIPC_PROBE
bin_PROGRAMS += lipc-probe
lipc_probe_CFLAGS = $(AM_CFLAGS) @GLIB20_CFLAGS@ @GIO20_CFLAGS@
lipc_probe_LDADD = $(LDADD) @GLIB20_LIBS@ @GIO20_LIBS@
endif

if ENABLE_KINDLE_ENV
AM_LDFLAGS = \
	-L$(KINDLE_ROOTDIR)/lib \
	-L$(KINDLE_ROOTDIR)/usr/lib \
	-Wl,-rpath=$(KINDLE_ROOTDIR)/lib \
	-Wl,-rpath=$(KINDLE_ROOTDIR)/usr/lib
endif
