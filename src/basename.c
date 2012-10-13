
/*
 *  basename -- return non-directory portion of a pathname
 *  Copyright (c) 1999-2012 Brad Proctor
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
   "Usage: basename long-option\n"
   "       basename string [suffix]\n"
   "Remove directory compontents from a pathname.\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
};

/*
 * Main
 */

int command_basename (int argc, char **argv)
{
   char *ptr, *ptr1;

   progname = argv[0];

   if (argc == 1)
      terror ("too few arguments");

   ptr = argv[1];
   if (!strcmp(ptr, "--help"))
   {
      puts (help_text);
      return (EXIT_SUCCESS);
   }
   else if (!strcmp (ptr, "--version"))
   {
      puts ("basename: version "VERSION);
      return (EXIT_SUCCESS);
   }

   /* 
    * There is probably a faster simpler way to do all this...
    *
    *       -- Brad
    */

   while (*ptr == '/')
      ++ptr;

   if (*ptr == '\0')
   {
      if (ptr > argv[1])
         putchar ('/');
      putchar ('\n');
      return (EXIT_SUCCESS);
   }

   ptr1 = strrchr (ptr, '/');
   if (ptr1)
      ptr = ptr1 + 1;

   while (*ptr == '\0')
   {
      *--ptr = '\0';
      ptr = strrchr (argv[1], '/');
      ++ptr;
   }
   
   if (argc > 2)
   {
      if (strcmp (argv[1], argv[2]) != 0)
      {
         ptr1 = strstr (ptr, argv[2]);
         if (strlen (ptr1) == strlen (argv[2]))
            *ptr1 = '\0';
      }
   }
   puts (ptr);
   return (EXIT_SUCCESS);
}
