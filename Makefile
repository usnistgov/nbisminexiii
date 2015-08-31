# This software was developed at the National Institute of Standards and
# Technology (NIST) by employees of the Federal Government in the course
# of their official duties. Pursuant to title 17 Section 105 of the
# United States Code, this software is not subject to copyright protection
# and is in the public domain. NIST assumes no responsibility  whatsoever for
# its use by other parties, and makes no guarantees, expressed or implied,
# about its quality, reliability, or any other characteristic.
#

LOCALLIB = $(PWD)/lib
LOCALBIN = $(PWD)/bin
CLIBDIRS = $(PWD)/src/mindtct $(PWD)/src/incits $(PWD)/src/bozorth3 $(PWD)/src/nbisminex
CBINDIRS = $(PWD)/src/test

default: libs bins

libs: 
	test -d $(LOCALLIB) || mkdir $(LOCALLIB)
	@for subdir in $(CLIBDIRS); do \
		(cd $$subdir && $(MAKE) all) || exit 1; \
	done

bins: 
	test -d $(LOCALBIN) || mkdir $(LOCALBIN)
	@for subdir in $(CBINDIRS); do \
		(cd $$subdir && $(MAKE) all) || exit 1; \
	done

clean:
	@for subdir in $(CLIBDIRS); do \
		(cd $$subdir && $(MAKE) clean) || exit 1; \
	done
	@for subdir in $(CBINDIRS); do \
		(cd $$subdir && $(MAKE) clean) || exit 1; \
	done
	$(RM) -r lib bin
