
/*
 *  fold -- filter for folding lines
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
   "Usage: fold long-option\n"
   "       fold [-bs][-w width] file ...\n"
   "Filter for folding lines.\n"
   "\n"
   "  -b         count bytes instead of columns\n"
   "  -s         Break at spaces\n"
   "  -w width   Width of a line\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
};

static char flag_bytes = 0;
static char flag_spaces = 0;
static int width = 72;
static FILE *in;

static void do_fold (void)
{
   int c, count = 0;
   
   while (1)
   {
      c = fgetc (in);
      if (c == EOF)
         return;
      else if (c == '\t')
         count += 7;
      else if (c == '\n')
         count = 0;
      if (count >= width)
      {
         putchar ('\n');
         count = 0;
      }
      putchar (c);
      count++;
   }
}

/*
 * Main
 */
 
int command_fold (int argc, char **argv)
{
   int i;
   char *ptr;

   progname = argv[0];
   in = stdin;
   
   for (i = 1; i < argc; ++i)
   {
      ptr = argv[i];
      if (*ptr == '-')
      {
         while (*++ptr)
         switch (*ptr)
         {
            case '\0':
               in = stdin;
               break;
            case '-':
               if (!strcmp (ptr, "-help"))
                  puts (help_text);
               else if (!strcmp (ptr, "-version"))
                  puts ("fold: version "VERSION);
               else
                  terror ("invalid option `%s'", argv[i]);
               return (EXIT_SUCCESS);
            case 'b':
               flag_bytes = 1;
               continue;
            case 's':
               flag_spaces = 1;
               continue;
            case 'w':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-w' option");
               width = getnum_ul (ptr);
               break;
         }
      }
      else
         in = xfopen (argv[i], "r");
      do_fold ();
      fclose (in);
   }
   return (EXIT_SUCCESS);
}
