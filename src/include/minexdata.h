/*
* This software was developed at the National Institute of Standards and
* Technology (NIST) by employees of the Federal Government in the course
* of their official duties. Pursuant to title 17 Section 105 of the
* United States Code, this software is not subject to copyright protection
* and is in the public domain. NIST assumes no responsibility  whatsoever for
* its use by other parties, and makes no guarantees, expressed or implied,
* about its quality, reliability, or any other characteristic.
*/

#ifndef _MINEXATA_H
#define _MINEXATA_H

#define MD_BUFSZERR	1	/* user-supplied buffer too small */
#define MD_MEMERR	2	/* error allocating memory */
#define MD_DATAERR	3	/* Data item from record is invalid */
#define MD_PARMERR	4	/* Parameter error */

/*
 * The representation of a finger minutiae data point for MINEX purposes.
 * The minutiae are in M1 format, where the angle is 1/2 the true theta
 * value.
 */
struct raw_fmd {
	unsigned short		type;
	unsigned short		x_coord;
	unsigned short		y_coord;
	unsigned short		angle;
	unsigned short		quality;
};

/*
 * getFingerMinutiae() returns the minutiae data from a buffer containing
 * an INCITS-378 finger minutiae record. Only the minutiae from the
 * first or second finger view can be returned.
 * 
 * Applications can use this function to retrieve minutiae from a 378 record
 * in a M1 format, so the angle value is 1/2 the real theta.
 * Example:
 *     databuf_len = FMR_MAX_NUM_MINUTIAE * FMD_DATA_LENGTH;
 *     databuf = malloc(databuf_len);
 *     ret = getFingerMinutiae(1, rec, databuf, &databuf_len, &count);
 *     pfmd = (struct piv_fmd *)databuf;
 *     for (i = 0; i < count; i++) {
 *         // do something with pfmd->x_coord, pfmd->y_coord, etc.
 *         pfmd++;
 *     }
 * 
 * Parameters:
 *   view   - (in) The finger view number, 1 or 2.
 *   rec    - (in) Buffer containing the INCITS-378 data record.
 *   buffer - (out) Output buffer that will contain the minutiae.
 *            This buffer must be allocated by the caller. 
 *            allocate a buffer of size FMR_MAX_NUM_MINUTIAE * FMD_DATA_LENGTH
 *   bufsz  - (in/out) The size of the allocated buffer on input, the size
 *            of the minutiae data on output.
 *   count    (out) The number of minutiae.
 *
 * Returns: 
 *   0           - Success.
 *   MD_BUFSZ   - Insufficient memory in the caller's buffer.
 *   MD_PARMERR - Incorrect parameter, most likely incorrect view.
 *   MD_DATAERR - Data from the record could not be processed.
 */
int
getFingerMinutiae(unsigned int view, uint8_t *rec, uint8_t *buffer,
    unsigned int *bufsz, unsigned int *count);

/*
 * subsetFingerMinutiaeRec() returns a new FMR record containing the single
 * finger view specifed by the view parameter. Both the original and new
 * finger minutiae records are INCITS-378 format.
 * 
 * Parameters:
 *   view      - (in) The finger view number, 1 or 2.
 *   rec       - (in) Buffer containing the INCITS-378 data record.
 *   newrec    - (out) Output buffer that will contain the new record.
 *               This buffer must be allocated by the caller.
 *   newrecsz  - (in/out) The size of the new record buffer on input, the size
 *            of the minutiae record data on output.
 *
 * Returns: 
 *   0            - Success.
 *   MD_BUFSZERR - Insufficient memory in the caller's buffer.
 *   MD_PARMERR  - Incorrect parameter, most likely incorrect view.
 *   MD_DATAERR  - Data from the record could not be processed.
 */
int
subsetFingerMinutiaeRec(unsigned int view, uint8_t *rec, uint8_t *newrec,
    unsigned int *newrecsz);

#endif	/* _MINEXDATA_H */
