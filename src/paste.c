
/*
 *  paste -- merge corresponding or subsequent lines of files
 *  Copyright (c) 2001-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: paste long-option\n"
   "       paste [-s] [-d list] [file ...]\n"
   "Merge corresponding or subsequent lines of files.\n"
   "\n"
   "  -d list    Use charaters from `list' instead of tabs\n"
   "  -s         Paste one file at a time instead of in parallel\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "https://github.com/bproctor/utils"
};

static FILE *in;
static char flag_serial = 0;
static char *flag_delimiter = "\t";

static void do_paste (void)
{
}

/*
 * Main
 */
 
int command_paste (int argc, char **argv)
{
   int i = 1;

   progname = argv[0];

   if (argc == 1)
   {
      in = stdin;
      do_paste ();
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
         puts ("paste: version "VERSION);
         return (EXIT_SUCCESS);
      }
      else if (!strcmp (argv[1], "-s"))
      {
         ++i;
         flag_serial = 1;
      }
      else if (!strcmp (argv[1], "-d"))
      {
         ++i;
         if (argc > 2)
            flag_delimiter = argv[2];
         else
            terror ("`-d' option requires an argument");
      }
   }

   for (; i < argc; ++i)
   {
      in = xfopen (argv[i], "r");
      do_paste ();
      fclose (in);
   }

   return (EXIT_SUCCESS);
}
