/*
 * This software was developed at the National Institute of Standards and
 * Technology (NIST) by employees of the Federal Government in the course
 * of their official duties. Pursuant to title 17 Section 105 of the
 * United States Code, this software is not subject to copyright protection
 * and is in the public domain. NIST assumes no responsibility whatsoever for
 * its use by other parties, and makes no guarantees, expressed or implied,
 * about its quality, reliability, or any other characteristic.
 */

/*
 * This file implements the MINEX-II API, using NBIS mindtct minutiae
 * detector and bozorth matcher from the NBIS project.
 */
#include <sys/queue.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include <biomdimacro.h>
#include <bozorth.h>
#include <bz_minex.h>
#include <fmr.h>
#include <lfs.h>

#include <minexdata.h>
#include <minexiii.h>
#include <nbisminex.h>

int32_t
create_template(const uint8_t* raw_image, const uint8_t finger_quality,
    const uint8_t finger_position, const uint8_t impression_type,
    const uint16_t height, const uint16_t width, uint8_t *out_template)
{
	int status, ret, i;
	MINUTIAE *minutiae;
	MINUTIA *m;
	int *quality_map, *direction_map, *low_contrast_map,
	    *low_flow_map, *high_curve_map;
	int map_w, map_h, bw, bh, bd;
	unsigned char *bdata;
	double ppmm;
	FMR *fmr;
	FVMR *fvmr;
	FMD *fmd;
	BDB bdb;
	int mcount;
	int qmaplen, qmapidx, qmapval;

	status = MINEX_RET_FAILURE_UNSPECIFIED;
	fmr = NULL;

	/* get_minuitiae() wants image depth as pixels/mm, so convert */
	ppmm = (double)MINEX_IMAGE_PPI / (double)MM_PER_INCH;
	ret = get_minutiae(&minutiae, &quality_map, &direction_map,
	    &low_contrast_map, &low_flow_map, &high_curve_map, &map_w,
	    &map_h, &bdata, &bw, &bh, &bd,
	    (unsigned char *)raw_image, width, height, NBIS_IMAGE_DEPTH,
	    ppmm, &lfsparms_V2);

	if (ret != 0)
		return (MINEX_RET_FAILURE_UNSPECIFIED);

	ret = new_fmr(FMR_STD_ANSI, &fmr);
	if (ret != 0)
		goto err_out;
	ret = new_fvmr(FMR_STD_ANSI, &fvmr);
	if (ret != 0)
		goto err_out;

	bcopy(FMR_FORMAT_ID, fmr->format_id, FMR_FORMAT_ID_LEN);
	bcopy(FMR_ANSI_SPEC_VERSION, fmr->spec_version, FMR_SPEC_VERSION_LEN);
	fmr->record_length = FMR_ANSI_SMALL_HEADER_LENGTH;
	fmr->record_length_type = FMR_ANSI_SMALL_HEADER_LENGTH;
	fmr->x_image_size = width;
	fmr->y_image_size = height;
	fmr->x_resolution = (unsigned short)ppmm * 10;
	fmr->y_resolution = (unsigned short)ppmm * 10;
	fmr->num_views = 1;

	fvmr->finger_number = finger_position;
	fvmr->view_number = 0;
	fvmr->impression_type = impression_type;
	fvmr->finger_quality = finger_quality;
	fvmr->x_image_size = width;
	fvmr->y_image_size = height;
	fvmr->x_resolution = (unsigned short)ppmm * 10;
	fvmr->y_resolution = (unsigned short)ppmm * 10;
	add_fvmr_to_fmr(fvmr, fmr);
	fmr->record_length += FVMR_HEADER_LENGTH;

	/* Convert the minutiae from LFS format to INCITS-378 and add them
	 * to the FMR, possibly using the quality map values.
	 */
	mcount = 0;
	qmaplen = map_w * map_h;
	for (i = 0; i < minutiae->num; i++) {
		m = minutiae->list[i];
		/*
		 * If the minutia point isn't in a sufficient
		 * quality zone, don't use it.
		 */
		qmapidx = (m->x / lfsparms_V2.blocksize) +
		    ((m->y / lfsparms_V2.blocksize) * map_w);
		qmapval = quality_map[qmapidx];
		if (qmapval < NBIS_QMAP_MIN)
			continue;
		mcount++;
		if (mcount > FMR_MAX_NUM_MINUTIAE)
			break;

		ret = new_fmd(FMR_STD_ANSI, &fmd, i);
		if (ret != 0)
			goto err_out;
		lfs2m1_minutia_XYT((int *)&fmd->x_coord, (int *)&fmd->y_coord,
		    (int *)&fmd->angle, m);
		fmd->quality = (unsigned char)sround(m->reliability * 100.0);
		switch (m->type) {
			case BIFURCATION:
				fmd->type = FMD_MINUTIA_TYPE_BIFURCATION;
				break;
			case RIDGE_ENDING:
				fmd->type = FMD_MINUTIA_TYPE_RIDGE_ENDING;
				break;
			default:
				fmd->type = FMD_MINUTIA_TYPE_OTHER;
				break;
		}
		add_fmd_to_fvmr(fmd, fvmr);
		fmr->record_length += FMD_DATA_LENGTH;
		fvmr->number_of_minutiae++;
	}
	fmr->record_length += FEDB_HEADER_LENGTH;
	INIT_BDB(&bdb, out_template, fmr->record_length);
	ret = push_fmr(&bdb, fmr);
	if (ret != WRITE_OK)
		goto err_out;

	status = MINEX_RET_SUCCESS;

err_out:
	free_minutiae(minutiae);
	free(quality_map);
	free(direction_map);
	free(low_contrast_map);
	free(low_flow_map);
	free(high_curve_map);
	free(bdata);
	if (fmr != NULL)
		free_fmr(fmr);

	return (status);
}

int32_t 
match_templates(const uint8_t *probe_template, const uint8_t *gallery_template, 
    float *score)
{
	int i, ret, status;
	unsigned int mcount;
	struct xyt_struct *bzxyt_p, *bzxyt_g;
	struct raw_fmd *pfmd;
	uint8_t *buf;
	unsigned int bufsz;
	BDB bdb;
	FMR *fmr;

	bzxyt_p = bzxyt_g = NULL;
	buf = NULL;

	/* Set up Bozorth globals */
	bz_minex_setup();

	if ((probe_template == NULL) || (gallery_template == NULL) ||
	    (score == NULL))
		return (MINEX_RET_FAILURE_NULL_TEMPLATE);

	bufsz = FMR_MAX_NUM_MINUTIAE * sizeof(struct raw_fmd);
	buf = malloc(bufsz);

	/*
	 * Bozorth doesn't match properly with M1 minutiae, so convert the
	 * M1 to the internal NIST format and kick Bozorth into normal mode.
	 */
	bz_minex_normal_matching();

	/*
	 * Get the minutiae from the first view in the probe template
	 */
	status = MINEX_RET_FAILURE_BAD_VERIFICATION_TEMPLATE;
	INIT_BDB(&bdb, probe_template, FMR_ANSI_MAX_SHORT_LENGTH);
	new_fmr(FMR_STD_ANSI, &fmr);
	if (scan_fmr(&bdb, fmr) != READ_OK) {
		free_fmr(fmr);
		goto err_out;
	}
	ret = getFingerMinutiae(1, probe_template, buf, &bufsz, &mcount);
	if (ret != 0)
		goto err_out;
	if (mcount < MIN_COMPUTABLE_BOZORTH_MINUTIAE)
		goto err_out;
	if (mcount > MAX_BOZORTH_MINUTIAE)
		mcount = MAX_BOZORTH_MINUTIAE;

	bzxyt_p = (struct xyt_struct *)malloc(sizeof(struct xyt_struct));
	if (bzxyt_p == NULL)
		goto err_out;
	bzxyt_p->nrows = mcount;
	pfmd = (struct raw_fmd *)buf;
	for (i = 0; i < mcount; i++) {
		bzxyt_p->xcol[i] = pfmd->x_coord;
		bzxyt_p->ycol[i] = pfmd->y_coord;
		bzxyt_p->ycol[i] = fmr->y_image_size - pfmd->y_coord - 1;
		/* Angle is opposite M1, and points into minutiae */
		bzxyt_p->thetacol[i] = ((pfmd->angle*2) + 180) % 360;
		pfmd++;
	}
	free_fmr(fmr);

	status = MINEX_RET_FAILURE_BAD_ENROLLMENT_TEMPLATE;
	INIT_BDB(&bdb, gallery_template, FMR_ANSI_MAX_SHORT_LENGTH);
	new_fmr(FMR_STD_ANSI, &fmr);
	if (scan_fmr(&bdb, fmr) != READ_OK) {
		free_fmr(fmr);
		goto err_out;
	}
	/*
	 * Get the minutiae from the first view in the gallery template
	 */
	bufsz = FMR_MAX_NUM_MINUTIAE * sizeof(struct raw_fmd);
	ret = getFingerMinutiae(1, gallery_template, buf, &bufsz, &mcount);
	if (ret != 0)
		goto err_out;
	if (mcount < MIN_COMPUTABLE_BOZORTH_MINUTIAE)
		goto err_out;
	if (mcount > MAX_BOZORTH_MINUTIAE)
		mcount = MAX_BOZORTH_MINUTIAE;

	bzxyt_g = (struct xyt_struct *)malloc(sizeof(struct xyt_struct));
	if (bzxyt_g == NULL)
		goto err_out;
	bzxyt_g->nrows = mcount;
	pfmd = (struct raw_fmd *)buf;
	for (i = 0; i < mcount; i++) {
		bzxyt_g->xcol[i] = pfmd->x_coord;
		bzxyt_g->ycol[i] = pfmd->y_coord;
		bzxyt_g->ycol[i] = fmr->y_image_size - pfmd->y_coord - 1;
		bzxyt_g->thetacol[i] = ((pfmd->angle*2) + 180) % 360;
		pfmd++;
	}
	free_fmr(fmr);
	*score = (float)bozorth_main(bzxyt_p, bzxyt_g);

	status = MINEX_RET_SUCCESS;

err_out:
	if (buf != NULL)
		free(buf);
	if (bzxyt_p != NULL)
		free(bzxyt_p);
	if (bzxyt_g != NULL)
		free(bzxyt_g);

	if (status != MINEX_RET_SUCCESS)
		*score = -1.0;
	return (status);
}

int32_t
get_pids(
    uint32_t *template_generator,
    uint32_t *template_matcher)
{
	*template_generator = (((uint32_t)NBIS_CBEFF_PRODUCT_ID_TMPL << 16) | 
	    NBIS_VERSION_TMPL);
	*template_matcher = (((uint32_t)NBIS_CBEFF_PRODUCT_ID_MATCH << 16) | 
	    NBIS_VERSION_MATCH);
	
	return (MINEX_RET_SUCCESS);
}

