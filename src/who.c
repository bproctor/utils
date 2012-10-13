
/*
 *  who -- List who is on the system
 *  Copyright (c) 2003-2012 Brad Proctor <brad@bradleyproctor.com>
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
   "Usage: who long-option\n"
   "       who [am i]\n"
   "List who is on the system.\n"
   "\n"
   "  am i       Print current user name\n"
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
 
int command_who (int argc, char **argv)
{
   int i;
   char *ptr;
   struct passwd *p;
   uid_t uid;
   
   progname = argv[0];
   
   if (argc == 3)
   {
      if (!strcmp (argv[1], "am") && !strcmp (argv[2], "i"))
      {
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
   }

   for (i = 1; i < argc; ++i)
   {
      ptr = argv[i];
      if (*ptr == '-')
      {
         switch (*++ptr)
         {
            case '-':
               if (!strcmp (ptr, "-help"))
                  puts (help_text);
               else if (!strcmp (ptr, "-version"))
                  puts ("who: version "VERSION);
               else
                  terror ("invalid option `%s'", argv[i]);
               return (EXIT_SUCCESS);
         }
      }
   }
   return (EXIT_SUCCESS);
}
