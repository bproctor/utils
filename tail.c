#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

static char *str_usage =
	"Usage: tail [options] [file]\n"
	"\n"
	"Display the last ten lines of the file, unless otherwise stated."
	"\n"
	"   -num        The number of lines to print\n"
	"   -n num      The number of lines to print\n"
	"   -f          \n"
	"   --help      Print this help message and exit\n"
	"   --version	Print version information and exit\n"
	"\n"
	"When no file is given, or when file is -, stdin will be used";


int command_tail(int argc, char **argv)
{
	char *buf, *ptr;
	int i, count;
	int lines = 10, line_number = 0;
	FILE *fp;

	if (argc > 1) {
		if (strcmp(argv[1], "--help") == 0) {
			usage(str_usage);
		} else if (strcmp(argv[1], "--version") == 0) {
			version();
		} else {
			fp = fopen(argv[1], "r");
			if (fp == NULL) {
				printf("tail: %s: file does not exist", argv[1]);
				return EXIT_FAILURE;
			}
		}
	} else {
		fp = stdin;
	}

	buf = malloc(4096);
	while (!feof(fp) || line_number < lines) {
		count = fread(buf, 4096, 1, fp);
		ptr = buf;
		while (*ptr != '\0') {
			if (*ptr++ == '\n') {
				if (++line_number == lines) {
					*ptr = '\0';
					break;
				}
			}
		}
		puts(buf);
	}

	return EXIT_SUCCESS;
}
