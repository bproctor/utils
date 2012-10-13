
/*
 *  uudecode -- decode a UU encoded file
 *  Copyright (c) 2003-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: uudecode long-option\n"
   "       uudecode file\n"
   "Decode a UU encoded file.\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "https://github.com/bproctor/utils"
};

static void do_uudecode (void)
{
}

/*
 * Main
 */
 
int command_uudecode (int argc, char **argv)
{
   progname = argv[0];

   if (argc == 1)
      terror ("too few arguments");
   else if (argc == 2)
   {
      if (!strcmp (argv[1], "--help"))
      {
         puts (help_text);
         return (EXIT_SUCCESS);
      }
      else if (!strcmp (argv[1], "--version"))
      {
         puts ("uudecode: version "VERSION);
         return (EXIT_SUCCESS);
      }
   }
   else
      terror ("too many arguments");
      
   do_uudecode ();
   return (EXIT_SUCCESS);
}
