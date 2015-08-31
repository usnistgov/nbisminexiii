/*
 * This software was developed at the National Institute of Standards and
 * Technology (NIST) by employees of the Federal Government in the course
 * of their official duties. Pursuant to title 17 Section 105 of the
 * United States Code, this software is not subject to copyright protection
 * and is in the public domain. NIST assumes no responsibility whatsoever for
 * its use by other parties, and makes no guarantees, expressed or implied,
 * about its quality, reliability, or any other characteristic.
 */

#ifndef __MINEX_H__
#define __MINEX_H__

/*
 * This file defines the MINEX-II interface to the minutiae extractor and
 * matcher used by the NBIS/MINEX project. See
 * http://fingerprint.nist.gov/minexII/nistir_7485.pdf
 */
#include <stdint.h>

/* Image constraints */
#define NBIS_IMAGE_PPI	500
#define NBIS_IMAGE_DEPTH	8
 
/* Minimum quality map value for a given minutiae point; see mindtct. */
#define NBIS_QMAP_MIN	3

/* Finger quality values */
#define QUAL_POOR	20	// NFIQ value 5 
#define QUAL_FAIR	40	// NFIQ value 4 
#define QUAL_GOOD	60	// NFIQ value 3 
#define QUAL_VGOOD	80	// NFIQ value 2 
#define QUAL_EXCELLENT	100	// NFIQ value 1 
 
/* Impression type codes */
 
#define IMPTYPE_LP	0x00	// Live-scan plain 
#define IMPTYPE_NP	0x02	// Nonlive-scan plain 

/* Finger position codes */
 
#define FINGPOS_UK	0x00	// Unknown finger 
#define FINGPOS_RT	0x01	// Right thumb 
#define FINGPOS_RI	0x02	// Right index finger 
#define FINGPOS_RM	0x03	// Right middle finger 
#define FINGPOS_RR	0x04	// Right ring finger 
#define FINGPOS_RL	0x05	// Right little finger 
#define FINGPOS_LT	0x06	// Left thumb 
#define FINGPOS_LI	0x07	// Left index finger 
#define FINGPOS_LM	0x08	// Left middle finger 
#define FINGPOS_LR	0x09	// Left ring finger 
#define FINGPOS_LL	0x0A	// Left little finger

/* Return codes */
#define MINEX_SUCCESS					0
#define MINEX_IMAGE_SIZE_UNSUPPORTED			1
#define MINEX_EXTRACTION_FAILED_UNSPECIFIED		2
#define MINEX_EXTRACTION_FAILED_TYPE_UNSUPPORTED	3
#define MINEX_MATCH_FAILED_NULL_PARAMETER		4
#define MINEX_MATCH_FAILED_PROBE_PARSING		5
#define MINEX_MATCH_FAILED_GALLERY_PARSING		6

/*
 * Create an INCITS-378 compliant template from a raw image. The template
 * is returned as an array of octets.
 * 
 * This function takes a raw image as input and outputs the corresponding
 * MINEX compliant template. The memory for the template is allocated before
 * the call (i.e., create_template() does not handle the memory allocation
 * for the template parameter). The function returns either success (0) or
 * failure (non-zero). Failure indicates a failure to enroll the image and will 
 * result in the output of a null template which will be used in later
 * comparisons. 
 *
 * Parameters:
 * raw_image (in): The uncompressed raw image used for template creation.
 *     The image must be 8 bits/pixel in depth, and have a resolution of
 *     500 pixels/inch.
 * finger_quality (in): The quality of the fingerprint image (e.g. QUAL_GOOD). 
 * finger_position (in): The finger position code (e.g. FINGPOS_RI). 
 * impression_type (in): The impression type code (e.g. IMPTYPE_LP). 
 * height (in): The number of pixels indicating the height of the image. 
 * width (in): The number of pixels indicating the width of the image. 
 * out_template (out): The processed template.
 *
 * Return Value 
 * This function returns zero on success or a documented non-zero error
 * code otherwise.
 */
uint32_t
create_template(const uint8_t* raw_image, const uint8_t finger_quality, 
    const uint8_t finger_position, const uint8_t impression_type, 
    const uint16_t height, const uint16_t width, uint8_t *out_template);

/*
 * Match two INCITS-378 finger minutiae templates, presented as arrays of
 * octets.
 * This function compares two MINEX compliant templates and outputs a match
 * score.  The probe_template parameter shall be compared to the
 * gallery_template parameter (in that precise order where the underlying
 * matcher is order dependent). The score returned is a floating-point number
 * which represents the similarity of the original fingerprint images from which
 * the templates where created.  Scores should not be quantized.  Memory for
 * the score parameter must be allocated before the call.
 *
 * Parameters:
 * probe_template (in): A template returned by create_template(). 
 * gallery_template (in): A template returned by create_template(). 
 * score (out): A similarity score resulting from comparison of the templates.
 *
 * Return Value 
 * This function returns zero on success (i.e. a valid score was produced) or
 * a documented non-zero error code on failure. In the latter case, the
 * function shall return a score of -1.
 */
uint32_t 
match_templates(const uint8_t *probe_template, const uint8_t *gallery_template, 
    float *score); 

#endif /* __MINEX_H__ */
