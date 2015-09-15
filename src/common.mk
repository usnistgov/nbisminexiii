#
# This software was developed at the National Institute of Standards and
# Technology (NIST) by employees of the Federal Government in the course
# of their official duties. Pursuant to title 17 Section 105 of the
# United States Code, this software is not subject to copyright protection
# and is in the public domain. NIST assumes no responsibility  whatsoever for
# its use by other parties, and makes no guarantees, expressed or implied,
# about its quality, reliability, or any other characteristic.
#
LOCALINC := ../include
LOCALLIB := ../../lib
LOCALBIN := ../../bin
DISPOSABLEFILES = *.o *.exe .gdb_history *.dll *.dylib *.so *.a
DISPOSABLEDIRS  = *.dSYM
CP := cp -pf
RM := rm -f
PWD := $(shell pwd)
OS := $(shell uname -s)
ARCH := $(shell uname -m)
CFLAGS := -std=c99
VERSION = $(shell grep NBIS_VERSION_TMPL $(LOCALINC)/nbisminex.h | sed 's/.*0x\(.*\);/\1/g')
ifeq ($(OS), Linux)
	CFLAGS += -fPIC
endif

#
# Define some rules that make files using this common file can rely on.
# The 'first' rule ensures that the required directories are created, and
# then fires the 'all' rule, which is defined by clients of this file.
#
first: dirs all
dirs:
	test -d $(LOCALLIB) || mkdir $(LOCALLIB)
	test -d $(LOCALBIN) || mkdir $(LOCALBIN)
