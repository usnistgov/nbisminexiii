#include <sys/queue.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <biomdimacro.h>
#include <fmr.h>
#include <minexiii.h>

#define P 0
#define G 1
int
main(int argc, char** argv)
{
	int i, ret;
	struct stat sb;
	uint8_t *buf;
	uint8_t *template[2];
	FILE *fp;
	uint16_t width[2], height[2];
	char fn[256];
	float score;
	int n, w, h;

	if (argc != 7) {
		fprintf(stderr, "Usage: %s <probe img> <width> <height>"
		    " <gallery img <width> <height>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	n = 1; w = 2; h = 3;
	for (i = 0; i < 2; i++) {
		ret = stat(argv[n], &sb);
		if (ret != 0) {
			fprintf(stderr, "Could not stat file %s.\n", argv[n]);
			exit(EXIT_FAILURE);
		}
		width[i] = (uint16_t)strtol(argv[w], NULL, 10);
		height[i] = (uint16_t)strtol(argv[h], NULL, 10);
		buf = malloc(sb.st_size);
		fp = fopen(argv[n], "rb");
		if (fp == NULL) {
			fprintf(stderr, "Could not open file %s.\n", argv[n]);
			exit(EXIT_FAILURE);
		}
		ret = fread(buf, 1, sb.st_size, fp);
		if (ret < 0) {
			fprintf(stderr, "Could not read file %s.\n", argv[n]);
			exit(EXIT_FAILURE);
		}	
		fclose(fp);
		template[i] = malloc(16384);

		ret = create_template(buf, MINEX_QUALITY_EXCELLENT,
		    MINEX_FINGER_UNKNOWN, MINEX_IMP_LIVESCAN_PLAIN,
		    height[i], width[i], template[i]);
		if (ret != 0) {
			fprintf(stderr, "Failed to create template.\n");
			exit(EXIT_FAILURE);
		}
		free(buf);
		n += 3; w += 3; h += 3;
	}
	printf("Calling match_templates()...");
	ret = match_templates(template[P], template[G], &score);
	if (ret != MINEX_RET_SUCCESS)
		printf(" failed!\n");
	else
		printf(" score returned is %f\n", score);
	exit(EXIT_SUCCESS);
}
