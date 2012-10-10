#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "utils.h"

static char *command_name;

void usage(char *msg)
{
	puts(msg);
	exit(EXIT_SUCCESS);
}

void version(void)
{
	printf ("%s, version %s", command_name, VERSION);
	exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
	int ret = EXIT_FAILURE;

	command_name = basename(argv[0]);
	switch(*command_name) {
		case 'c':
			if (strcmp(command_name, "cat") == 0) {
				ret = command_cat(argc, argv);
			}
			break;
		case 'f':
			if (strcmp(command_name, "false") == 0) {
				ret = EXIT_FAILURE;
			}
			break;
		case 'h':
			if (strcmp(command_name, "head") == 0) {
				ret = command_head(argc, argv);
			}
			break;
		case 'o':
			if (strcmp(command_name, "od") == 0) {
				ret = command_od(argc, argv);
			}
			break;
		case 't':
			if (strcmp(command_name, "tac") == 0) {
				ret = command_tac(argc, argv);
			} else if (strcmp(command_name, "tail") == 0) {
				ret = command_tail(argc, argv);
			} else if (strcmp(command_name, "true")) {
				ret = EXIT_SUCCESS;
			}
			break;
		case 'w':
			if (strcmp(command_name, "wc") == 0) {
				ret = command_wc(argc, argv);
			}
			break;
		case 'y':
			if (strcmp(command_name, "yes") == 0) {
				ret = command_yes(argc, argv);
			}
			break;
		default:
			ret = EXIT_FAILURE;
	}
	return ret;
}
