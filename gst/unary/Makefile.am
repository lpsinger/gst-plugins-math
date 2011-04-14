plugin_LTLIBRARIES = libgstunary.la

# sources used to compile this plug-in
libgstunary_la_SOURCES = \
  unary.c \
  unary_base.h unary_base.c \
  unary_abs.c \
  unary_exp.c \
  unary_ln.c \
  unary_log.c \
  unary_log10.c \
  unary_pow.c

# compiler and linker flags used to compile this plugin, set in configure.ac
libgstunary_la_CFLAGS = $(AM_CFLAGS) $(GST_CFLAGS)
libgstunary_la_LIBADD = $(GST_LIBS)
libgstunary_la_LDFLAGS = $(AM_LDFLAGS) $(GST_PLUGIN_LDFLAGS)
libgstunary_la_LIBTOOLFLAGS = --tag=disable-static

# headers we need but don't want installed
noinst_HEADERS = unary_base.h
