/*
 * This software was developed at the National Institute of Standards and
 * Technology (NIST) by employees of the Federal Government in the course
 * of their official duties. Pursuant to title 17 Section 105 of the
 * United States Code, this software is not subject to copyright protection
 * and is in the public domain. NIST assumes no responsibility whatsoever for
 * its use by other parties, and makes no guarantees, expressed or implied,
 * about its quality, reliability, or any other characteristic.
 */

#ifndef NBISMINEX_H_
#define NBISMINEX_H_

/** Image constraints not defined in MINEX header. */
static const uint8_t NBIS_IMAGE_DEPTH = 8;
/** Minimum quality map value for a given minutiae point; see mindtct. */
static const uint8_t NBIS_QMAP_MIN = 3;

/** Template generator CBEFF product ID */
static const uint16_t NBIS_CBEFF_PRODUCT_ID_TMPL = 0x0000;
/* Version of template generator: (NBIS version << 12) + NBISMINEX version */
static const uint16_t NBIS_VERSION_TMPL = 0x5001;
/** Template matcher CBEFF product ID */
static const uint16_t NBIS_CBEFF_PRODUCT_ID_MATCH = 0x0000;
/* Version of template matcher: (NBIS version << 12) + NBISMINEX version */
static const uint16_t NBIS_VERSION_MATCH = 0x5001;

#endif /* NBISMINEX_H_ */

