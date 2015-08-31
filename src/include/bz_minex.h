/*
* This software was developed at the National Institute of Standards and
* Technology (NIST) by employees of the Federal Government in the course
* of their official duties. Pursuant to title 17 Section 105 of the
* United States Code, this software is not subject to copyright protection
* and is in the public domain. NIST assumes no responsibility  whatsoever for
* its use by other parties, and makes no guarantees, expressed or implied,
* about its quality, reliability, or any other characteristic.
*/
/*
 * Set up Bozorth's global variable that controls logging of errors.
 */
void bz_minex_setup() {
	errorfp = stderr;
}

/*
 * Tell Bozorth to match using LFS format minutiae.
 */
void bz_minex_normal_matching() {
	m1_xyt = 0;
}

/*
 * Tell Bozorth to match using minutiae in INCITS/M1 format.
 */
void bz_minex_m1_matching() {
	m1_xyt = 1;
}
