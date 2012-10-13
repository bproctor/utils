
/*
 *  newgrp -- change to a new group
 *  Copyright (c) 2003-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: newgrp long-option\n"
   "       newgrp [-l][group]\n"
   "Change to a new group.\n"
   "\n"
   "  -l         \n"
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
 
int command_newgrp (int argc, char **argv)
{
   struct passwd *pw;
   struct group *g;
   
   progname = argv[0];

   pw = getpwuid (getuid ());
   if (!pw)
      error ("cannot figure out who you are???");
   
   if (argc == 1)
   {
      if (setgid (pw->pw_gid) < 0)
         error ("setgid() failed: %s", strerror (errno));
   }
   else if (argc >= 2)
   {
      if (!strcmp (argv[1], "--help"))
      {
         puts (help_text);
         return (EXIT_SUCCESS);
      }
      else if (!strcmp (argv[1], "--version"))
      {
         puts ("newgrp: version "VERSION);
         return (EXIT_SUCCESS);
      }
      else if (!strcmp (argv[1], "-l"))
         ;
   }
   g = getgrnam(argv[1]);
   if (!g)
      error ("`%s' is not a group", argv[1]);

   if (setgid (g->gr_gid) < 0)
      error ("cannot set new group: %s", strerror (errno));
      
   return (EXIT_SUCCESS);
}
