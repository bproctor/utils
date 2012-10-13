
/*
 *  logname.c -- return the user's login name
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
   "Usage: logname [long-option]\n"
   "Print the users login name.\n"
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
 
int command_logname (int argc, char **argv)
{
   progname = argv[0];
         
   if (argc == 1)
   {
      char *p = getlogin ();
      if (!p)
         error ("no login name");
      
      puts (p);
      return (EXIT_SUCCESS);
   }         
   else if (argc == 2)
   {
      if (!strcmp (argv[1], "--help"))
      {
         puts (help_text);
         return (EXIT_SUCCESS);
      }
      else if (!strcmp (argv[1], "--version"))
      {
         puts ("logname: version "VERSION);
         return (EXIT_SUCCESS);
      }
      else
         terror ("invalid option `%s'", argv[1]);
   }
   else
      terror ("too many arguments");

   return (EXIT_SUCCESS);
}
