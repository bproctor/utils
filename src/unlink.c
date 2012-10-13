
/*
 *  unlink -- call the unlink function to unlink a file
 *  Copyright (c) 2003-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: unlink long-option\n"
   "       unlink file\n"
   "Call the unlink function to unlink a file.\n"
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
 
int command_unlink (int argc, char **argv)
{
   progname = argv[0];
   
   if (argc == 1)
      terror ("too few arguments");
   else if (argc == 2)
   {
      if (!strcmp (argv[1], "--help"))
         puts (help_text);
      else if (!strcmp (argv[1], "--version"))
         puts ("unlink: version "VERSION);
      else
      {
         if (unlink (argv[1]))
            error ("cannot unlink `%s': %s", argv[1], strerror (errno));
      }
   }
   else
      terror ("too many arguments");
      
   return (EXIT_SUCCESS);
}
