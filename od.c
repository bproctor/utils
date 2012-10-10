
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

static char *str_usage =
	"Usage: od [options] file\n"
	"   -h    Print this help message\n"
	"   -v    Version of this command";

int command_od(int argc, char **argv)
{
	usage(str_usage);
	return EXIT_SUCCESS;
}
