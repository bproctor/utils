
/*
 *  yes -- print y or a message forever
 *  Copyright (c) 2001-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: yes long-option\n"
   "       yes [message]\n"
   "Print 'y' or a message forever.\n"
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
 
int command_yes (int argc, char **argv)
{
   if (argc == 1)
   {
      while (1)
         puts ("y");
      return (EXIT_SUCCESS);
   }

   else if (argc == 2)
   {
      if (!strcmp (argv[1], "--help"))
      {
         puts (help_text);
         return (EXIT_SUCCESS);
      }
      else if (!strcmp (argv[1], "--version"))
      {
         puts ("yes: version "VERSION);
         return (EXIT_SUCCESS);
      }
   }

   while (1)
   {
      int i;
      for (i = 1; i < argc; ++i)
      {
         fputs (argv[i], stdout);
         if (i == argc - 1)
            putchar ('\n');
         else
           putchar (' ');
      }
   }

   return (EXIT_SUCCESS);
}
