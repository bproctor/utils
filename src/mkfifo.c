
/*
 *  mkfifo -- make FIFO special files
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
   "Usage: mkfifo long-option\n"
   "       mkfifo [-m mode] name ...\n"
   "Create FIFO special files.\n"
   "\n"
   "  -m mode    Set permission bits\n"
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

int command_mkfifo (int argc, char **argv)
{
   int i = 1, mode = 0666;
   
   progname = argv[0];
   
   if (argc > 1)
   {
      if (argv[1][0] == '-')
      {
         if (!strcmp (argv[1], "--help"))
         {
            puts (help_text);
            return (EXIT_SUCCESS);
         }
         else if (!strcmp (argv[1], "--version"))
         {
            puts ("mkfifo: version "VERSION);
            return (EXIT_SUCCESS);
         }
         else if (!strcmp (argv[1], "-m"))
         {
            mode = getnum_octal (argv[2]);
            i = 3;
         }
      }
      else
      {
         mode &= ~umask (0);
         for (; i < argc; ++i)
            if (mkfifo (argv[i], mode))
               error ("cannot create FIFO `%s': %s",
                  argv[i], strerror (errno));
      }
   }
   return (EXIT_SUCCESS);
}
