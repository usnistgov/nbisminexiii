/*
* This software was developed at the National Institute of Standards and
* Technology (NIST) by employees of the Federal Government in the course
* of their official duties. Pursuant to title 17 Section 105 of the
* United States Code, this software is not subject to copyright protection
* and is in the public domain. NIST assumes no responsibility  whatsoever for
* its use by other parties, and makes no guarantees, expressed or implied,
* about its quality, reliability, or any other characteristic.
*/
/******************************************************************************/
/* Define the global variables that are referenced in the bozorth library.    */
/* Set them to values that are approrpriate for the Bozorth MINEX project.    */
/* Users of the MINEX Bozorth library must call bz_minex().                   */
/******************************************************************************/
#include <bozorth.h>
#include <stdio.h>

FILE * errorfp;
int verbose_main            = 0;
int verbose_load            = 0;
int verbose_bozorth         = 0;
int m1_xyt                  = 0;
int max_minutiae            = DEFAULT_BOZORTH_MINUTIAE;
int min_computable_minutiae = MIN_COMPUTABLE_BOZORTH_MINUTIAE;

void bz_minex_setup() {
	errorfp = stderr;
}
void bz_minex_normal_matching() {
	m1_xyt = 0;
}
void bz_minex_m1_matching() {
	m1_xyt = 1;
}
