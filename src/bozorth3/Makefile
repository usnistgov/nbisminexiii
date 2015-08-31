# ******************************************************************************
# This software was developed at the National Institute of Standards and
# Technology (NIST) by employees of the Federal Government in the course
# of their official duties. Pursuant to title 17 Section 105 of the
# United States Code, this software is not subject to copyright protection
# and is in the public domain. NIST assumes no responsibility  whatsoever for
# its use by other parties, and makes no guarantees, expressed or implied,
# about its quality, reliability, or any other characteristic.
#

include ../common.mk

SOURCES = bozorth3.c bz_alloc.c bz_drvrs.c bz_gbls.c bz_io.c bz_sort.c bz_minex.c
LIBRARY = libbozorth3

all: $(SOURCES)
ifeq ($(OS), Darwin)
	$(CC) -I$(LOCALINC) -c $(CFLAGS) $(SOURCES)
	libtool -dynamic -o $(LIBRARY).dylib -lc $(SOURCES:.c=.o)
	$(CP) $(LIBRARY).dylib $(LOCALLIB)
else
ifeq ($(findstring CYGWIN,$(OS)), CYGWIN)
	$(CC) -I$(LOCALINC) $(CFLAGS) -c $(SOURCES)
	ar rs $(LIBRARY).a $(SOURCES:.c=.o)
	ranlib $(LIBRARY).a
	$(CC) -I$(LOCALINC) -shared -o $(LIBRARY).dll -Wl,--out-implib=$(LIBRARY).dll.a -Wl,--export-all-symbols -Wl,--enable-auto-import -Wl,--whole-archive $(LIBRARY).a -Wl,--no-whole-archive
	$(CP) $(LIBRARY).a $(LOCALLIB)
	$(CP) $(LIBRARY).dll.a $(LOCALLIB)
	$(CP) $(LIBRARY).dll $(LOCALLIB)
else
	$(CC) -I$(LOCALINC)  $(CFLAGS) -shared $(SOURCES) -o $(LIBRARY).so
	$(CP) *.so $(LOCALLIB)
endif
endif

clean:
	$(RM) *.so *.dll *.a *.dylib *.o
