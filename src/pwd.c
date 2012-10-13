
/*
 *  pwd -- print the working directory
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
   "Usage: pwd long-option\n"
   "Print the working directory.\n"
   "\n"
   "  -L         \n"
   "  -P         \n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "https://github.com/bproctor/utils"
};

static char flag_L = 0;
static char flag_P = 0;

/*
 * Main
 */
 
int command_pwd (int argc, char **argv)
{
   int i;
   char *ptr, *d;

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
                  puts ("pwd: version "VERSION);
               else
                  terror ("invalid option `%s'", argv[i]);
               return (EXIT_SUCCESS);
            case 'L':
               flag_L = 1, flag_P = 0;
               continue;
            case 'P':
               flag_P = 1, flag_L = 0;
               continue;
            default:
               terror ("invalid option `%s'", argv[i]);
         }
      }
   }
   if (!flag_L && !flag_P)
      flag_L = 1;
      
   d = getcwd (NULL, 0);
   if (d == NULL)
      error ("cannot get directory: %s", strerror (errno));
   puts (d);

   return (EXIT_SUCCESS);
}
