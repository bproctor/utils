
/*
 *  dirname -- return directory portion of a pathname
 *  Copyright (c) 2002-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include "utils.h"

char *dirname (const char *);

static const char help_text[] =
{
   "Usage: dirname long-option\n"
   "       dirname string\n"
   "Return the directory compontents from a pathname.\n"
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

int command_dirname (int argc, char **argv)
{
   char *ptr, *ptr1;

   progname = argv[0];

   if (argc == 1)
      terror ("too few arguments");
   else if (argc == 2)
   {
      if (!strcmp(argv[1], "--help"))
      {
         puts (help_text);
         return (EXIT_SUCCESS);
      }
      else if (!strcmp (argv[1], "--version"))
      {
         puts ("dirname: version "VERSION);
         return (EXIT_SUCCESS);
      }
   }
   else
      terror ("too many arguments");

   ptr = argv[1];
   while (*ptr == '/')
   {
      if (*(ptr + 1) == '\0')
      {
         puts ("/");
         return (EXIT_SUCCESS);
      }
      if (*(ptr + 1) != '/')
         break;
      ++ptr;
   }
   
   ptr1 = strrchr (ptr, '/');
   if (ptr1 == NULL)
   {
      puts (".");
      return (EXIT_SUCCESS);
   }

   while (*(ptr1 + 1) == '\0')
   {
      *ptr1 = '\0';
      ptr1 = strrchr (ptr, '/');
      if (ptr1 == NULL)
         break;
   }
   *(ptr1+1) = '\0';
   if (ptr1 > ptr)
   {
      while (*(ptr1 - 1) == '/')
         --ptr1;
      *ptr1 = '\0';
   }
   puts (ptr);
   
   return (EXIT_SUCCESS);
}
