
/*
 *  hostname -- print of set the system hostname
 *  Copyright (c) 2003-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/param.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: hostname long-option\n"
   "       hostname [name]\n"
   "Print or set the system hostname.\n"
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
 
int command_hostname (int argc, char **argv)
{
   char hostname[MAXHOSTNAMELEN];

   progname = argv[0];
   
   if (argc == 1)
   {
      if (gethostname (hostname, MAXHOSTNAMELEN) < 0)
         error ("cannot get hostname: %s", strerror (errno));
      puts (hostname);
      return (EXIT_SUCCESS);
   }
   else if (argc == 2)
   {
      if (!strcmp (argv[1], "--help"))
         puts (help_text);
      else if (!strcmp (argv[1], "--version"))
         puts ("hostname: version "VERSION);
      else
      {
         if (sethostname (argv[1], strlen (argv[1])) < 0)
            error ("cannot set hostname: %s", strerror (errno));
      }
   }
   else
      terror ("too many arguments");
      
   return (EXIT_SUCCESS);
}
