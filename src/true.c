
/*
 *  true -- return true value
 *  Copyright (c) 2001-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: true [long-option]\n"
   "Exits with a successful status code.  Any arguments except those\n"
   "given below are ignored.\n"
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
 
int command_true (int argc, char **argv)
{
   if (argc == 2)
   {
      if (!strcmp (argv[1], "--help"))
         puts (help_text);
      else if (!strcmp (argv[1], "--version"))
         puts ("true: version "VERSION);
   }

   return (EXIT_SUCCESS);
}
