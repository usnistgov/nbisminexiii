# ******************************************************************************
# This software was developed at the National Institute of Standards and
# Technology (NIST) by employees of the Federal Government in the course
# of their official duties. Pursuant to title 17 Section 105 of the
# United States Code, this software is not subject to copyright protection
# and is in the public domain. NIST assumes no responsibility  whatsoever for
# its use by other parties, and makes no guarantees, expressed or implied,
# about its quality, reliability, or any other characteristic.
#
LOCALINC = ../include
include ../common.mk
#CFLAGS = -g
SOURCES = nbisminex.c minexdata.c
LIBS = -lc -lmindtct -lincits -lbozorth3
#
LIBRARY = libminexiii_nbisminexiii_$(VERSION)

all: $(SOURCES)
ifeq ($(OS), Darwin)
	$(CC) -I$(LOCALINC) -c $(CFLAGS) $(SOURCES)
	libtool -dynamic -o $(LIBRARY).dylib -L$(LOCALLIB) $(LIBS) $(SOURCES:.c=.o)
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
	$(CC) -I$(LOCALINC) $(CFLAGS) -shared $(SOURCES) -L$(LOCALLIB) $(LIBS) -o $(LIBRARY).so
	$(CP) *.so $(LOCALLIB)
endif
endif

clean:
	rm -f *.so *.dll *.o *.a *.dylib
