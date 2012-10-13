
/*
 *  tr -- translate characters
 *  Copyright (c) 2002-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: tr long-option\n"
   "       tr [-d] str1\n"
   "       tr str1 str2\n"
   "Translate characters\n"
   "\n"
   "  -d         Delete characters\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "https://github.com/bproctor/utils"
};

static char *fstr = 0;  /* String to search for */
static char *rstr = 0;  /* String to replace with */

/*
 * Main
 */
 
int command_tr (int argc, char **argv)
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
                  puts ("tr: version "VERSION);
               else
                  terror ("invalid option `%s'", argv[i]);
               return (EXIT_SUCCESS);
            case 'd':
               break;
         }
      }
      else if (!fstr)
      {
      }
      else if (!rstr)
      {
      }
   }
   return (EXIT_SUCCESS);
}
