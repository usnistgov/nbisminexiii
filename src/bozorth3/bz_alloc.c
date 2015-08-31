/*******************************************************************************

License: 
This software was developed at the National Institute of Standards and 
Technology (NIST) by employees of the Federal Government in the course 
of their official duties. Pursuant to title 17 Section 105 of the 
United States Code, this software is not subject to copyright protection 
and is in the public domain. It has been determined that the export control 
restriction did not apply to the NFSEG and BOZORTH3 software, due to both
being outside the scope of EAR(see Part 734.3 of the EAR for exact details); 
they are freely distributed and considered public domain. NIST assumes no 
responsibility  whatsoever for its use by other parties, and makes no 
guarantees, expressed or implied, about its quality, reliability, or any 
other characteristic. 

Disclaimer: 
This software was developed to promote biometric standards and biometric
technology testing for the Federal Government in accordance with the USA
PATRIOT Act and the Enhanced Border Security and Visa Entry Reform Act.
Specific hardware and software products identified in this software were used
in order to perform the software development.  In no case does such
identification imply recommendation or endorsement by the National Institute
of Standards and Technology, nor does it imply that the products and equipment
identified are necessarily the best available for the purpose.  

*******************************************************************************/

/***********************************************************************
      LIBRARY: FING - NIST Fingerprint Systems Utilities

      FILE:           BZ_ALLOC.C
      ALGORITHM:      Allan S. Bozorth (FBI)
      MODIFICATIONS:  Michael D. Garris (NIST)
                      Stan Janet (NIST)
      DATE:           09/21/2004

      Contains routines responsible for supporting the
      Bozorth3 fingerprint matching algorithm.

***********************************************************************

      ROUTINES:
#cat: malloc_or_exit - allocates a buffer of bytes from the heap of
#cat:        specified length exiting directly upon system error
#cat: malloc_or_return_error - allocates a buffer of bytes from the heap
#cat:        of specified length returning an error code upon system error

***********************************************************************/

#include <stdio.h>
#include <string.h>
#include <bozorth.h>


/***********************************************************************/
char * malloc_or_exit( int nbytes, const char * what )
{
char * p;

/* These are now externally defined in bozorth.h */
/* extern FILE * errorfp; */
/* extern char * get_progname( void ); */


p = malloc( (size_t) nbytes );
if ( p == CNULL ) {
	fprintf( errorfp, "%s: ERROR: malloc() of %d bytes for %s failed: %s\n",
						get_progname(),
						nbytes,
						what,
						strerror( errno )
						);
	exit(1);
}
return p;
}

/***********************************************************************/
/* returns CNULL on error */
char * malloc_or_return_error( int nbytes, const char * what )
{
char * p;

p = malloc( (size_t) nbytes );
if ( p == CNULL ) {
	fprintf( errorfp, "%s: ERROR: malloc() of %d bytes for %s failed: %s\n",
						get_progname(),
						nbytes,
						what,
						strerror( errno )
						);
	return(CNULL);
}
return p;
}
