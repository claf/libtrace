prefix=$(HOME)/opt
libdir=$(prefix)/lib
incdir=$(prefix)/include

define compile_rule
        libtool --quiet --mode=compile $(CC) $(CFLAGS) -c $<
endef
define link_rule
        libtool --quiet --mode=link $(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS) -rpath $(libdir)
endef

CC=gcc -Wall -std=gnu99
CFLAGS=-g -I$(incdir)
LDFLAGS=-L$(libdir) -lgtg

SRC=trace_gtg.c
HEADER=trace_gtg.h
OBJ=$(SRC:.c=.lo)
LIB=libtrace_gtg.la

all: $(LIB)

%.lo: %.c
	$(call compile_rule)

$(LIB): $(OBJ)
	$(call link_rule)

install/%.la: %.la
	cp $(HEADER) $(incdir)
	libtool --quiet --mode=install install -c $(notdir $@) $(libdir)/$(notdir $@)
install: $(addprefix install/,$(LIB))
	libtool --quiet --mode=finish $(libdir)

clean:
	rm -fr *.la *.lo *.o

veryclean:clean
	rm -fr .libs/

.SILENT: all $(LIB)
