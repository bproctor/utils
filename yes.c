#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

static char *str_usage =
	"Usage yes [option]\n"
	"      yes [string]\n"
	"\n"
	"yes prints out 'y' or the given string forever until terminated.\n"
	"\n"
	"    --help       Print this help message and exit\n"
	"    --version    Print version information and exit";


int command_yes(int argc, char **argv)
{
	char *str = "y";

	if (argc > 1) {
		if (strcmp(argv[1], "--help")) {
			usage(str_usage);
		} else if (strcmp(argv[1], "--version")) {
			version();
		}
		str = argv[1];
	}

	while (1) {
		puts (str);
	}

	return EXIT_SUCCESS;
}
