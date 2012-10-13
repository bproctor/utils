
/*
 *  rm -- remove directory entries
 *  Copyright (c) 1999-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: rm long-option\n"
   "       rm [-frRi] file ...\n"
   "Remove files.\n"
   "\n"
   "  -f         Force the removal\n"
   "  -r, -R     Remove files recursively\n"
   "  -i         Prompt for comfirmation\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "https://github.com/bproctor/utils"
};

static char flag_force;
static char flag_recursive;
static char flag_prompt;

/*
 * Main
 */

int command_rm (int argc, char **argv)
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
                  puts("rm: version "VERSION);
               else
                  terror ("invalid option `%s'", ptr);
               return (EXIT_SUCCESS);
            case 'f':
               flag_force = 1;
               continue;
            case 'R':
            case 'r':
               flag_recursive = 1;
               continue;
            case 'i':
               flag_prompt = 1;
               continue;
         }
      }   
      else
      {
         if (!flag_force || flag_prompt)
         {
            printf ("rm: remove `%s'? ", ptr);
            if (getchar () == 'y')
               remove (ptr);
         }
         else
            remove (ptr);      
      }
   }
   return (EXIT_SUCCESS);
}
