
/*
 *  chmod -- change the file modes
 *  Copyright (c) 1999-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: chmod long-option\n"
   "       chmod [-R] mode file ...\n"
   "Change file permissions.\n"
   "\n"
   "  -R         Recursively change file permission bits\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "https://github.com/bproctor/utils"
};

static char flag_recursive = 0;
static char flag_user = 0;
static char flag_owner = 0;
static char flag_group = 0;
static char flag_read = 0;
static char flag_write = 0;
static char flag_exec = 0;
static char flag_add = 0;
static char flag_remove = 0;
static char flag_equals = 0;

/*
 * Main
 */

int command_chmod (int argc, char **argv)
{
   int i = 1, mode, newmode;
   char *ptr;
   
   progname = argv[0];
   
   if (argc == 1)
      terror ("too few arguments");
   else if (argc == 2)
   {
      if (!strcmp (argv[1], "--help"))
         puts (help_text);
      else if (!strcmp (argv[1], "--version"))
         puts ("chmod: version "VERSION);
      else
         terror ("");
      return (EXIT_SUCCESS);
   }

   if (!strcmp (argv[1], "-R"))
      flag_recursive = 1, ++i;


   ptr = argv[i];
   if (isdigit (*ptr))
      mode = getnum_octal (argv[i]);

   else
   {
      int done = 0;
      while (!done)
      {
         switch (*ptr)
         {
            case 'g': flag_group = 1; break;
            case 'o': flag_owner = 1; break;
            case 'u': flag_user = 1; break;
            default:
               done = 1;
         }
         ++ptr;
      }
      if (*ptr == '-')
         flag_remove = 1;
      else if (*ptr == '+')
         flag_add = 1;
      else if (*ptr == '=')
         flag_equals = 1;
      else
         terror ("invalid character in mode field `%c'", *ptr);
      
      ++ptr, done = 0;
      while (!done)
      {
         switch (*ptr++)
         {
            case 'r': flag_read = 1; break;
            case 'w': flag_write = 1; break;
            case 'x': flag_exec = 1; break;
            default:
               done = 1;
         }
      }
      newmode = 0;
      mode = (flag_read << 2) | (flag_write << 1) | flag_exec;
      if (flag_user)
         newmode |= mode << 6;
      if (flag_group)
         newmode |= mode << 3;
      if (flag_owner)
         newmode |= mode;

      if (flag_equals)
      {
         if (chmod (argv[i+1], newmode))
            error ("cannot change permissions of `%s': %s",
               argv[i+1], strerror (errno));
         return (EXIT_SUCCESS);
      }
      
      if (flag_add)
         ;
      else if (flag_remove)
         ;
      
   }
   return (EXIT_SUCCESS);
}
