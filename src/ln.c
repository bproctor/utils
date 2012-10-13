
/*
 *  ln -- link files
 *  Copyright (c) 1999-2012 Brad Proctor <brad@bradleyproctor.com>
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
   "Usage: ln long-option\n"
   "       ln [-fs] source dest\n"
   "       ln [-fs] source ... dir\n"
   "Create file links.\n"
   "\n"
   "  -f         Force the link.\n"
   "  -s         Create a symbolic link\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "https://github.com/bproctor/utils"
};

static char flag_force = 0;      /* Force the link */
static char flag_sym = 0;        /* Create a symbolic link */

/*
 * Main
 */

int command_ln (int argc, char **argv)
{
   int i;
   char *ptr;
   
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
                  puts ("ln: version "VERSION);
               else
                  terror ("invalid option `%s'", argv[i]);
               return (EXIT_SUCCESS);
            case 'f':
               flag_force = 1;
               continue;
            case 's':
               flag_sym = 1;
               continue;
         }
      }   
   }
   if (flag_force || flag_sym)
   {
      if (!argv[2] || !argv[3])
         terror ("missing argument");
      if (flag_force)
      {
         if (unlink (argv[3]))
            error ("cannot unlink `%s': %s", argv[3], strerror (errno));
      }
      if (flag_sym)
      {
         if (symlink (argv[2], argv[3]))
            error ("cannot create link `%s' to `%s': %s",
               argv[3], argv[2], strerror (errno));
      }
      else
      {
         if (link (argv[2], argv[3]))
            error ("cannot create link `%s' to `%s': %s",
               argv[3], argv[2], strerror (errno));
      }
   }
   else 
   {
      if (!argv[1] || !argv[2])
         terror ("missing argument");
      if (flag_force)
         unlink (argv[2]);
      if (flag_sym)
         symlink (argv[1], argv[2]);
      else
         link (argv[1], argv[2]);
   }
   return (EXIT_SUCCESS);
}
