
/*
 *  rmdir -- remove directories
 *  Copyright (c) 1999-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: rmdir long-option\n"
   "       rmdir [-p] dir...\n"
   "Remove directories.\n"
   "\n"
   "  -p         Remove all directories in a pathname\n"
   "  -v         Verbose\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "https://github.com/bproctor/utils"
};

static char flag_verbose = 0;
static char flag_parent = 0;

static void do_rmdir (const char *dir)
{
   if (flag_parent)
   {
      while (1)
      {
         char *s;
         if (rmdir (dir) != 0)
            error ("cannot remove directory `%s': %s", dir, strerror (errno));
         else if (flag_verbose)
            printf ("removed: `%s'\n", dir);
         if ((s = strrchr (dir, '/')) == NULL)
            break;
         while (s > dir && *s == '/')
            --s;
         s[1] = 0;
      }
   }
   else
   {
      if (rmdir (dir) != 0)
         error ("cannot remove directory `%s': %s", dir, strerror (errno));
      else if (flag_verbose)
         printf ("removed: `%s'\n", dir);
   }
}

/*
 * Main
 */

int command_rmdir (int argc, char **argv)
{
   int i;
   char *ptr;

   progname = argv[0];

   if (argc == 1)
      do_rmdir (argv[1]);

   for (i = 1; i < argc; ++i)
   {
      ptr = argv[i];
      if (*ptr == '-')
      {
         while (*++ptr)
         switch (*ptr)
         {
            case '-':   
               if (!strcmp (ptr, "-help"))
                  puts (help_text);
               else if (!strcmp (ptr, "-version"))
                  puts ("rmdir: version "VERSION);
               else
                  terror ("invalid option `%s'", argv[i]);
               return (EXIT_SUCCESS);
            case 'p':
               flag_parent = 1;
               continue;
            case 'v':
               flag_verbose = 1;
               continue;
         }
      }
      else
         do_rmdir (argv[i]);
   }
   return (EXIT_SUCCESS);
}
