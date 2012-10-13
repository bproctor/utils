
/*
 *  domainname -- print of set the system domainname
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
   "Usage: domainname long-option\n"
   "       domainname [name]\n"
   "Print or set the system domainname.\n"
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
 
int command_domainname (int argc, char **argv)
{
   char domainname[MAXHOSTNAMELEN];

   progname = argv[0];
   
   if (argc == 1)
   {
      if (getdomainname (domainname, MAXHOSTNAMELEN) < 0)
         error ("cannot get domainname: %s", strerror (errno));
      puts (domainname);
      return (EXIT_SUCCESS);
   }
   else if (argc == 2)
   {
      if (!strcmp (argv[1], "--help"))
         puts (help_text);
      else if (!strcmp (argv[1], "--version"))
         puts ("domainname: version "VERSION);
      else
      {
         if (setdomainname (argv[1], strlen (argv[1])) < 0)
            error ("cannot set domainname: %s", strerror (errno));
      }
   }
   else
      terror ("too many arguments");
      
   return (EXIT_SUCCESS);
}
