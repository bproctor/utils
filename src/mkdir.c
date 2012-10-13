
/*
 *  mkdir -- make directories
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
   "Usage: mkdir long-option\n"
   "       mkdir [-p][-m mode] dir...\n"
   "Create directories.\n"
   "\n"
   "  -m mode    Set permission bits\n"
   "  -p         Create missing pathname components\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "`mode' can be any of the following\n"
   "  b   block (buffered) special file\n"
   "  c   character (unbuffered) special file\n"
   "  p   FIFO special file\n"
   "\n"
   "https://github.com/bproctor/utils"
};

/*
 * Main
 */

int command_mkdir (int argc, char **argv)
{
   int i = 1, mode = 0775, err;

   progname = argv[0];
   
   if (argc > 1)
   {
      if (argv[1][0] == '-')
      {
         if (!strcmp(argv[1], "--help"))
         {
            puts (help_text);
            return (EXIT_SUCCESS);
         }
         else if (!strcmp (argv[1], "--version"))
         {
            puts ("mkdir: version "VERSION);
            return (EXIT_SUCCESS);
         }
         else if (!strcmp (argv[1], "-p"))
            i = 2;
         else if (!strcmp (argv[1], "-m"))
         {
            if (argv[2] == NULL)
               terror ("missing operand to `-m' option");
            mode = getnum_octal (argv[2]);
            i = 3;
         }
      }
      for (; i < argc; ++i)
      {
         err = mkdir (argv[i], mode);
         if (err)
            error ("cannot create directory `%s': %s", argv[i], strerror (errno));
      }
   }
   return (EXIT_SUCCESS);
}
