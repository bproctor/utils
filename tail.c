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
	usage(str_usage);
	return EXIT_SUCCESS;
}
