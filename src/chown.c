
/*
 *  chown -- change the file ownership
 *  Copyright (c) 1999-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: chown long-option\n"
   "       chown [-hR] owner[:group] file ...\n"
   "       chown -R [-H | -L | -P] file ...\n"
   "Change file owner.\n"
   "\n"
   "  -h        \n"
   "  -H        \n"
   "  -L        \n"
   "  -P        \n"
   "  -R         Recursively change file owner IDs\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "https://github.com/bproctor/utils"
};

static char flag_recursive = 1;
static char *owner = 0;
static char *group = 0;

/*
 * Main
 */

int command_chown (int argc, char **argv)
{
   int i;
   char *ptr;
   
   progname = argv[0];

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
                  puts ("chown: version "VERSION);
               else
                  terror ("invalid option `%s'", argv[i]);
               return (EXIT_SUCCESS);
            case 'h':
               continue;
            case 'H':
               continue;
            case 'L':
               continue;
            case 'P':
               continue;
            case 'R':
               flag_recursive = 1;
               continue;
         }
      }
      else if (!owner)
      {
         char *p;
         owner = argv[i];
         p = strchr (owner, ':');
         if (p)
         {
            *p = '\0';
            group = p + 1;
         }
      }
      else
      {
         
      }
   }
   return (EXIT_SUCCESS);
}
