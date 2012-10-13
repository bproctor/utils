
/*
 *  link -- link files
 *  Copyright (c) 2003-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: link long-option\n"
   "       link file1 file2\n"
   "Use the link function to link files together.\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "https://github.com/bproctor/utils"
};

/*
 * Main
 */
 
int command_link (int argc, char **argv)
{
   progname = argv[0];

   if (argc == 1)
      terror ("too few arguments");
   else if (argc == 2)
   {
      if (!strcmp (argv[1], "--help"))
         puts (help_text);
      else if (!strcmp (argv[1], "--version"))
         puts ("link: version "VERSION);
      else
         terror ("invalid option `%s'", argv[1]);
   }
   else if (argc == 3)
   {
      if (link (argv[1], argv[2]))
         error ("cannot create link `%s' to `%s': %s", argv[2], argv[1], strerror (errno));
   }
   else
      terror ("too many arguments");

   return (EXIT_SUCCESS);
}
