
/*
 *  whoami -- Print the current user name
 *  Copyright (c) 2001-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: whoami [long-option]\n"
   "Print the current user name.\n"
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
 
int command_whoami (int argc, char **argv)
{
   struct passwd *p;
   uid_t uid;

   progname = argv[0];
   
   if (argc == 2)
   {
      if (!strcmp (argv[1], "--help"))
         puts (help_text);
      else if (!strcmp (argv[1], "--version"))
         puts ("whoami: version "VERSION);
      else
         terror ("invalid option `%s'", argv[1]);
      return (EXIT_SUCCESS);
   }
   uid = geteuid ();
   if (uid == -1)
      error ("cannot get effective UID: %s", strerror (errno));
   p = getpwuid (uid);
   if (!p)
      error ("cannot find username for UID %u: %s",
         (unsigned int)uid, strerror (errno));
   puts (p->pw_name);

   return (EXIT_SUCCESS);
}
