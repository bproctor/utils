
/*
 *  more -- Print files on page at a time
 *  Copyright (c) 2003-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: more long-option\n"
   "Print files one page at a time.\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "https://github.com/bproctor/utils"
};

static int max_lines = 24;

static void do_more (FILE *in)
{
   int c, lines = 0;
   
   while (1)
   {
      c = getc (in);
      if (c == EOF)
         break;
      else if (c == '\n')
      {
         if (++lines >= max_lines)
         {
            lines = 0;
            fputs ("\n--MORE--", stdout);
            getc (stdin);
         }
         else
            putchar (c);
      }
      else
         putchar (c);
   }
}

/*
 * Main
 */
 
int command_more (int argc, char **argv)
{
   int i;
   FILE *in;
   
   if (argc == 1)
      do_more (stdin);
   else if (argc == 2)
   {
      if (!strcmp (argv[1], "--help"))
      {
         puts (help_text);
         return (EXIT_SUCCESS);
      }
      else if (!strcmp (argv[1], "--version"))
      {
         puts ("more: version "VERSION);
         return (EXIT_SUCCESS);
      }
      else if (!strcmp (argv[1], "-"))
      {
         do_more (stdin);
         return (EXIT_SUCCESS);
      }
   }

   for (i = 1; i < argc; ++i)
   {
      in = xfopen (argv[i], "r");
      do_more (in);
      fclose (in);
   }

   return (EXIT_SUCCESS);
}
