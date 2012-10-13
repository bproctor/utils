
/*
 *  mesg -- allow or deny messages
 *  Copyright (c) 2003-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: mesg long-option\n"
   "       mesg [y | n]\n"
   "Allow or deny messages.\n"
   "\n"
   " y           Allow write access to your terminal\n"
   " n           Deny write access to your terminal\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "If neither y or n is specified, mesg will display the current\n"
   "write state of your terminal\n"
   "\n"
   "https://github.com/bproctor/utils"
};

/*
 * Main
 */
 
int command_mesg (int argc, char **argv)
{
   struct stat s;
   char *tty;
   
   progname = argv[0];

   tty = ttyname (STDERR_FILENO);
   if (tty == NULL)
      error ("ttyname failed: %s", strerror (errno));
   if (stat (tty, &s) < 0)
      error ("cannot stat tty: %s", strerror (errno));
   

   if (argc == 1)
   {
      if (s.st_mode & (S_IWGRP | S_IWOTH))
      {
         puts ("is y");
         return (EXIT_SUCCESS);
      }
      else
      {
         puts ("is n");
         return (EXIT_FAILURE);
      }
   }
   else if (argc == 2)
   {
      if (!strcmp (argv[1], "--help"))
         puts (help_text);
      else if (!strcmp (argv[1], "--version"))
         puts ("mesg: version "VERSION);
      else if (!strcmp (argv[1], "y"))
      {
         if (chmod (tty, s.st_mode | S_IWGRP) < 0)
            error ("cannot change mode: %s", strerror (errno));
      }
      else if (!strcmp (argv[1], "n"))
      {
         if (chmod (tty, s.st_mode & ~S_IWGRP) < 0)
            error ("cannot change mode: %s", strerror (errno));
      }
      else
         terror ("invalid option `%s'", argv[1]);
   }
   else
      terror ("too many arguments");

   return (EXIT_SUCCESS);
}
