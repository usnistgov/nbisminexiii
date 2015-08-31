/*
* This software was developed at the National Institute of Standards and
* Technology (NIST) by employees of the Federal Government in the course
* of their official duties. Pursuant to title 17 Section 105 of the
* United States Code, this software is not subject to copyright protection
* and is in the public domain. NIST assumes no responsibility  whatsoever for
* its use by other parties, and makes no guarantees, expressed or implied,
* about its quality, reliability, or any other characteristic.
*/

#include <sys/queue.h> 
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <biomdimacro.h>
#include <minexdata.h>
#include <fmr.h>	/* From BIOMDI, INCITS-378 Finger Minutiae */

/*
 * Helper function that either subsets an FMR and returns a new FMR with the
 * single view, or retrieves the minutiae from a single view and returns
 * them in a buffer.
 */
static int
subsetFMR(unsigned int view, uint8_t *rec, uint8_t *buffer, unsigned int *bufsz,
    unsigned int *count, FMR **newfmr)
{
	int status;
	int ret;
	FMR *fmr, *lnewfmr;
	FVMR **fvmrs, *newfvmr;
	FMD **fmds, *newfmd;
	BDB bdb;
	struct raw_fmd *pfmd;
	int vcount, mcount;
	int i;

	if ((view != 1) && (view != 2))
		return (MD_PARMERR);

	fmr = NULL;
	lnewfmr = NULL;
	fvmrs = NULL;
	fmds = NULL;
	ret = new_fmr(FMR_STD_ANSI, &fmr);
	if (ret != 0) 
		return (MD_MEMERR);

	//XXX Should check that the incoming FMR is actually a short record
	INIT_BDB(&bdb, rec, FMR_ANSI_MAX_SHORT_LENGTH);
	ret = scan_fmr(&bdb, fmr);
	if (ret != READ_OK) {
		status = MD_DATAERR;
		goto err_out;
	}
	vcount = get_fvmr_count(fmr);
	if (vcount == 0) {
		status = MD_DATAERR;
		goto err_out;
	}
	fvmrs = (FVMR **)malloc(vcount * sizeof(FVMR *));
	if (fvmrs == NULL) {
		status = MD_MEMERR;
		goto err_out;
	}
	ret = get_fvmrs(fmr, fvmrs);
	if (ret != vcount) {
		status = MD_DATAERR;
		goto err_out;
	}
	view = view - 1;

	mcount = get_fmd_count(fvmrs[view]);
	fmds = (FMD **)malloc(mcount * sizeof(FMD **));
	if (fmds == NULL) {
		status = MD_MEMERR;
		goto err_out;
	}
	ret = get_fmds(fvmrs[view], fmds);
	if (ret != mcount) {
		status = MD_DATAERR;
		goto err_out;
	}
	if (buffer != NULL) {
		*count = mcount;
		if (*bufsz < (mcount * sizeof(struct raw_fmd))) {
			status = MD_BUFSZERR;
			goto err_out;
		}
		*bufsz = mcount * sizeof(struct raw_fmd);
		pfmd = (struct raw_fmd *)buffer;
		for (i = 0; i < mcount; i++) {
			pfmd->type = fmds[i]->type;
			pfmd->x_coord = fmds[i]->x_coord;
			pfmd->y_coord = fmds[i]->y_coord;
			pfmd->angle = fmds[i]->angle;
			pfmd->quality = fmds[i]->quality;
			pfmd++;
		}
	}

	if (newfmr != NULL) {
		ret = new_fmr(FMR_STD_ANSI, &lnewfmr);
		if (ret != 0) {
			status = MD_MEMERR;
			goto err_out;
		}
		COPY_FMR(fmr, lnewfmr);
		lnewfmr->record_length = FMR_ANSI_SMALL_HEADER_LENGTH +
		    FEDB_HEADER_LENGTH;
		lnewfmr->num_views = 1;
		ret = new_fvmr(FMR_STD_ANSI, &newfvmr);
		if (ret != 0) {
			status = MD_MEMERR;
			goto err_out;
		}
		COPY_FVMR(fvmrs[view], newfvmr);
		for (i = 0; i < mcount; i++) {
			ret = new_fmd(FMR_STD_ANSI, &newfmd, i);
			if (ret != 0) {
				status = MD_MEMERR;
				goto err_out;
			}
			COPY_FMD(fmds[i], newfmd);
			add_fmd_to_fvmr(newfmd, newfvmr);
			lnewfmr->record_length += FMD_DATA_LENGTH;
		}
		add_fvmr_to_fmr(newfvmr, lnewfmr);
		lnewfmr->record_length += FVMR_HEADER_LENGTH;
		*newfmr = lnewfmr;
	}
	status = 0;
err_out:
	if (fmr != NULL)
		free_fmr(fmr);
	if (fvmrs != NULL)
		free(fvmrs);
	if (fmds != NULL)
		free(fmds);
	if (status != 0)
		if (lnewfmr != NULL)
			free_fmr(lnewfmr);
	return (status);
}

/*
 */
int
getFingerMinutiae(unsigned int view, uint8_t *rec, uint8_t *buffer,
    unsigned int *bufsz, unsigned int *count)
{
	return (subsetFMR(view, rec, buffer, bufsz, count, NULL));
}

/*
 */
int
subsetFingerMinutiaeRec(unsigned int view, uint8_t *rec, uint8_t *newrec,
    unsigned int *newrecsz)
{
	int status, ret;
	FMR *newfmr;
	BDB bdb;

	ret = subsetFMR(view, rec, NULL, NULL, NULL, &newfmr);
	if (ret != 0)
		return (ret);
	if (*newrecsz < newfmr->record_length) {
		status = MD_BUFSZERR;
		goto err_out;
	}
	INIT_BDB(&bdb, newrec, *newrecsz);
	ret = push_fmr(&bdb, newfmr);
	if (ret != WRITE_OK) {
		status = MD_DATAERR;
		goto err_out;
	}
	*newrecsz = newfmr->record_length;

	status = 0;
err_out:
	free_fmr(newfmr);
	return (status);
}
