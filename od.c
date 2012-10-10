
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

static char *str_usage =
	"Usage: od [options] file\n"
	"\n"
	"description"
	"\n"
	"   --help      Print this help message and exit\n"
	"   --version	Print version information and exit\n";

int command_od(int argc, char **argv)
{
	char *buf;
	FILE *fp;

	if (argc > 1) {
		if (strcmp(argv[1], "--help")) {
			usage(str_usage);
		} else (strcmp(argv[1], "--version")) {
			version();
		} else {
			fp = fopen(argv[1], "r");
			if (fp == NULL) {
				printf("od: %s: file does not exist\n", argv[1]);
				return EXIT_FAILURE;
			}
		}
	} else {
		fp = stdin;
	}

	buf = malloc(4096);
	while (!feof(fp)) {
		count = fread(buf, 4096, 1, fp);
		for (i = 0; i < count; i++) {
			printf ("%x ", buf[i]);
		}
	}

	return EXIT_SUCCESS;
}
