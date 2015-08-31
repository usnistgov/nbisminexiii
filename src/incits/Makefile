#
# This software was developed at the National Institute of Standards and
# Technology (NIST) by employees of the Federal Government in the course
# of their official duties. Pursuant to title 17 Section 105 of the
# United States Code, this software is not subject to copyright protection
# and is in the public domain. NIST assumes no responsibility  whatsoever for
# its use by other parties, and makes no guarantees, expressed or implied,
# about its quality, reliability, or any other characteristic.
#
include ../common.mk
#SOURCES = fmr.c fvmr.c fmd.c fedb.c polar.c random.c xy.c angle.c quality.c ansi2iso.c iso2ansi.c validate.c fb.c fdb.c fpb.c frf_validate.c
SOURCES = fmr.c fvmr.c fmd.c fedb.c 
OBJECTS = $(SOURCES:%.c=%.o)

#
# On OS-X, when using gcc 4.0 or later, the SystemStubs library must be
# linked in. This may be true on other OS as well.
#
LIBNAME = libincits
ifeq ($(OS), Darwin)
#ifeq ($(findstring gcc-4, $(GCCV)), gcc-4)
ifeq ($(GCCV), 4)
EXTRALIBS := -lSystemStubs
endif
endif

all: $(SOURCES)
ifeq ($(OS), Darwin)
	$(CC) -c $(CFLAGS) -I$(LOCALINC) $(SOURCES)
	libtool -dynamic -o $(LIBNAME).dylib -lc $(OBJECTS) $(EXTRALIBS)
	$(CP) $(LIBNAME).dylib $(LOCALLIB)
else
ifeq ($(findstring CYGWIN,$(OS)), CYGWIN)
	$(CC) $(CFLAGS) -I$(LOCALINC) -c $(SOURCES)
	ar rs $(LIBNAME).a $(OBJECTS)
	ranlib $(LIBNAME).a
	$(CC) -shared -o $(LIBNAME).dll -Wl,--out-implib=$(LIBNAME).dll.a -Wl,--export-all-symbols -Wl,--enable-auto-import -Wl,--whole-archive $(LIBNAME).a -Wl,--no-whole-archive
	$(CP) $(LIBNAME).a $(LOCALLIB)
	$(CP) $(LIBNAME).dll.a $(LOCALLIB)
	$(CP) $(LIBNAME).dll $(LOCALLIB)
else
	$(CC) $(CFLAGS) -I$(LOCALINC) -shared $(SOURCES) -lm -o $(LIBNAME).so
	$(CP) $(LIBNAME).so $(LOCALLIB)
endif
endif

clean:
	$(RM) $(DISPOSABLEFILES)
	$(RM) -r $(DISPOSABLEDIRS)
