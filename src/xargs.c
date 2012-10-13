
/*
 *  xargs -- construct argument lists and invoke utility
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
   "Usage: xargs long-option\n"
   "       xargs [-pt][-E eofstr][-I replstr][-L num][-n num[-x]][-s size][utility [arg ...]]\n"
   "Constuct argument lists and invoke utility.\n"
   "\n"
   "  -E eofstr  \n"
   "  -I replstr \n"
   "  -L num     \n"
   "  -n num     \n"
   "  -p         \n"
   "  -s size    \n"
   "  -t         \n"
   "  -x         \n"
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
 
int command_xargs (int argc, char **argv)
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
                  puts ("xargs: version "VERSION);
               else
                  terror ("invalid option `%s'", argv[i]);
               return (EXIT_SUCCESS);
            case 'E':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-%c' option", *(ptr-1));
               break;
            case 'I':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-%c' option", *(ptr-1));
               break;
            case 'L':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-%c' option", *(ptr-1));
               break;
            case 'n':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-%c' option", *(ptr-1));
               break;
            case 'p':
               continue;
            case 's':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-%c' option", *(ptr-1));
               break;
            case 't':
               continue;
            case 'x':
               continue;
         }
      }
   }
   return (EXIT_SUCCESS);
}
