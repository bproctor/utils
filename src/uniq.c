
/*
 *  uniq -- Remove duplicate lines from sorted files
 *  Copyright (c) 2003-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: uniq long-option\n"
   "Remove duplicate lines from sorted files.\n"
   "  -c         \n"
   "  -d         Suppress lines that are not repeated\n"
   "  -f         \n"
   "  -s         \n"
   "  -u         \n"
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
 
int command_uniq (int argc, char **argv)
{
   int i;
   char *ptr;
   
   progname = argv[0];
   
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
                  puts ("uniq: version "VERSION);
               else
                  terror ("invalid option `%s'", argv[i]);
               return (EXIT_SUCCESS);
            case 'c':
               continue;
            case 'd':
               continue;
            case 'f':
               continue;
            case 's':
               continue;
            case 'u':
               continue;
         }
      }
   }
   return (EXIT_SUCCESS);
}
