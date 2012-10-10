#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

static char *str_usage =
	"Usage: cat [options] [file ...]\n"
	"\n"
	"concatenate files"
	"\n"
	"   -b          Number non empty output lines\n"
	"   -n          Number all output lines\n"
	"   -s          Suppress empty line output\n"
	"   --help      Print this help message and exit\n"
	"   --version	Print version information and exit\n"
	"\n"
	"When no file is given, or when file is -, stdin will be used";


int command_cat(int argc, char **argv)
{
	FILE *fp;
	char *buf = malloc(4096);
	if (argc > 1) {
		if (strcmp(argv[1], "--help")) {
			usage(str_usage);
		} else if (strcmp(argv[1], "--version")) {
			version();
		}
		fp = fopen(argv[1], "r");
		if (fp == NULL) {
			printf("cat: %s: File does not exist", argv[1]);
			return EXIT_SUCCESS;
		}
	} else {
		fp = stdin;
	}

	while (!feof(fp)) {
		fread(buf, 4096, 1, fp);
		fputs(buf, stdout);
	}
	fclose(fp);
	return EXIT_SUCCESS;
}
