#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

static char *str_usage =
	"Usage: head [options] [file]\n"
	"\n"
	"Display the first ten lines of the file, unless otherwise stated."
	"\n"
	"   -num        The number of lines to print\n"
	"   -n num      The number of lines to print\n"
	"   --help      Print this help message and exit\n"
	"   --version	Print version information and exit\n"
	"\n"
	"When no file is given, or when file is -, stdin will be used";


int command_head(int argc, char **argv)
{
	FILE *fp;
	char *buf;

	if (argc > 1) {
		if (strcmp(argv[1], "--help")) {
			usage(str_usage);
		} else if (strcmp(argv[1], "--version")) {
			version();
		} else {
			fp = fopen(argv[1], "r");
			if (fp == NULL) {
				puts("");
				return EXIT_FAILURE;
			}
		}
	} else {
		fp = stdin;
	}

	while (!feof(fp)) {
		fgets(buf, 1024, fp);
		puts(buf);
	}

	fclose(fp);

	return EXIT_SUCCESS;
}
