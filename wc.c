#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

static char *str_usage =
	"Usage: wc [options] file\n"
	"   -h    Print this help message\n"
	"   -v    Version of this command";

int command_wc(int argc, char **argv)
{
	int i, count;
	int chars = 0, words = 0, lines = 0;
	FILE *fp;
	char *buf = malloc(4096);

	if (argc > 1) {
		if (strcmp(argv[1], "--help") == 0) {
			usage(str_usage);
		} else {
			version();
		}
	} else {
		usage(str_usage);
	}
	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		printf("wc: %s: file does not exist\n", argv[1]);
		return EXIT_FAILURE;
	}

	while (!feof(fp)) {
		count = fread(buf, 4096, 1, fp);
		chars += count;
		for (i = 0; i < count; i++) {
			if (buf[i] == '\n') {
				lines++;
			}
		}
	}

	printf ("%d %d %d\n", chars, words, lines);

	fclose(fp);
	return EXIT_SUCCESS;
}
