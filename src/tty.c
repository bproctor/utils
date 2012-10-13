
/*
 *  tty -- return user's terminal name
 *  Copyright (c) 2001-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: tty long-option\n"
   "       tty [-s]\n"
   "Return users terminal name.\n"
   "\n"
   "  -s         Silent\n"
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
 
int command_tty (int argc, char **argv)
{
   char *tty;
   int silent = 0;

   progname = argv[0];

   if (argc == 2)
   {
      if (!strcmp (argv[1], "--help"))
      {
         puts (help_text);
         return (EXIT_SUCCESS);
      }
      else if (!strcmp (argv[1], "--version"))
      {
         puts ("tty: version "VERSION);
         return (EXIT_SUCCESS);
      }
      else if (!strcmp (argv[1], "-s"))
         silent = 1;
      else
         terror ("invalid option `%s'", argv[1]);
   }
   
   tty = ttyname (0);
   if (!silent)
   {
      if (tty)
         puts (tty);
      else
         puts ("not a tty");
   }   
   return (isatty (0) ? EXIT_SUCCESS : EXIT_FAILURE);
}
