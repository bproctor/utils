
/*
 *  mknod -- create a node special file
 *  Copyright (c) 1999-2012 Brad Proctor. <brad@bradleyproctor.com>
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
   "Usage: mknod long-option\n"
   "       mknod [-p][-m mode] name ...\n"
   "Create a node special file.\n"
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
};

/*
 * Main
 */

int command_mknod (int argc, char **argv)
{
   int i = 1, mode = 0775;

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
            puts ("mknod: version "VERSION);
            return (EXIT_SUCCESS);
         }
         else if (!strcmp (argv[1], "-m"))
         {
            if (argv[2] == NULL)
               terror ("missing operand to `-m' option");
            mode = getnum_octal (argv[2]);
            i = 3;
         }
      }
      for (; i < argc; ++i)
         if (mknod (argv[i], mode, 0))
            error ("cannot create `%s': %s",
               argv[i], strerror (errno));
   }
   return (EXIT_SUCCESS);
}
