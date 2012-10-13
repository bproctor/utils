
/*
 *  chroot -- Execute a command or shell on a different directory
 *  Copyright (c) 2001-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: chroot long-option\n"
   "       chroot dir [command]\n"
   "Execute a command or shell on a different directory.\n"
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
 
int command_chroot (int argc, char **argv)
{
   char *sh;

   progname = argv[0];
   
   if (argc == 1)
      terror ("too few arguments");
   else if (argc == 2)
   {
      if (!strcmp (argv[1], "--help"))
      {
         puts (help_text);
         return (EXIT_SUCCESS);
      }
      else if (!strcmp (argv[1], "--version"))
      {
         puts ("chroot: version "VERSION);
         return (EXIT_SUCCESS);
      }
   }
   
   if (chroot (argv[1]))
      error ("cannot change to directory `%s': %s", argv[1], strerror (errno));
   
   if (argc == 2)
   {
      sh = getenv ("SHELL");
      argv[0] = (sh == NULL) ? "/bin/sh" : sh;
      argv[1] = "-i";      
   }
   else
      argv += 2;
   
   execvp (argv[0], argv);
   error ("cannot execute `%s': %s", argv[0], strerror (errno));
   return (EXIT_FAILURE);
}
