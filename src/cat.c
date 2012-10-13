
/*
 *  cat -- concatenate and print files
 *  Copyright (c) 1999-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: cat long-option\n"
   "       cat [-u][file ...]\n"
   "Concatenate files.\n"
   "\n"
   "  -u         Print without buffering\n"
   "\n"
   "Long Option\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
};

static void do_cat (int in)
{
   int c;
   char *ptr;
   struct stat s;
   
   fstat (in, &s);
   ptr = xmalloc (s.st_blksize);
   while ((c = read (in, ptr, s.st_blksize)) > 0)
      write (STDOUT_FILENO, ptr, c);
   close (in);
}

/*
 * Main
 */

int command_cat (int argc, char **argv)
{
   int i = 1, in;
   
   if (argc == 1)
   {
      do_cat (STDIN_FILENO);
      return (EXIT_SUCCESS);
   }
   else if (argc >= 2)
   {
      if (!strcmp (argv[1], "--help"))
      {
         puts (help_text);
         return (EXIT_SUCCESS);
      }
      else if (!strcmp (argv[1], "--version"))
      {
         puts ("cat: version "VERSION);
         return (EXIT_SUCCESS);
      }
      else if (!strcmp (argv[1], "-u"))     /* Ignored */
         ++i;
   }

   for (; i < argc; ++i)
   {
      if (!strcmp (argv[i], "-"))
         in = STDIN_FILENO;
      else
      {
         in = open (argv[i], O_RDONLY);
         if (!in)
            error ("cannot open file `%s' for reading: %s",
               argv[i], strerror (errno));
         close (in);
      }
      do_cat (in);
   }

   return (EXIT_SUCCESS);
}
