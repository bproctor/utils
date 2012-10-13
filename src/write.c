
/*
 *  write -- write to another user
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
   "Usage: write long-option\n"
   "       write user [terminal]\n"
   "Write to another user.\n"
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
 
int command_write (int argc, char **argv)
{
   char *user, *term;
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
         puts ("write: version "VERSION);
         return (EXIT_SUCCESS);
      }
      else
         user = argv[1];
   }
   else if (argc == 3)
   {
      user = argv[1];
      term = argv[2];
   }
   else
      terror ("too many arguments");

   return (EXIT_SUCCESS);
}
