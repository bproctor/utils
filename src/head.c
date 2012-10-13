
/*
 *  head -- copy the first part of files
 *  Copyright (c) 1999-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include "utils.h"

#define  LINE_MAX    1024

static const char help_text[] =
{
   "Usage: head long-option\n"
   "       head [-c n | -n n | -number][file ...]\n"
   "Print the first part of a file.\n"
   "\n"
   "  -c n        Print the first `n' bytes\n"
   "  -n n        Print the first `n' lines\n"
   "  -number     Print the first `number' lines\n"
   "\n"
   "Long Options:\n"
   "  --help      Print this help message\n"
   "  --version   Print the prgram version\n"
   "\n"
   "https://github.com/bproctor/utils"
};

static char flag_lines = 1;
static char flag_done = 0;
static char buf[LINE_MAX];
static FILE *in;
static int bytes = 800;
static int lines = 10;

static void do_head (void)
{
   if (flag_lines)
      while (lines-- && fgets (buf, LINE_MAX, in))
         fputs (buf, stdout);
   else
   {
      int c;
      while (bytes-- && (c = fgetc (in)) != EOF)
         putchar (c);
   }      
   flag_done = 1;
}

/*
 * Main
 */

int command_head (int argc, char **argv)
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
         switch (*++ptr)
         {
            case '\0':
               in = stdin;
               continue;
            case '-':
               if (!strcmp (ptr, "-help"))
                  puts (help_text);
               else if (!strcmp (ptr, "-version"))
                  puts ("head: version "VERSION);
               else
                  terror ("invalid option `%s'", argv[i]);
               return (EXIT_SUCCESS);
            case 'c':
               if (*++ptr != '\0')
                  ;
               else if (++i >= argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-c' option");
               bytes = getnum_ul (ptr);
               flag_lines = 0;
               continue;
            case 'n':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-n' option");
               /* Fall through to get the number */
            case '0':   case '1':   case '2':
            case '3':   case '4':   case '5':
            case '6':   case '7':   case '8':
            case '9':
               lines = getnum_ul (ptr);
               flag_lines = 1;
               continue;
         }
      }
      else
         in = xfopen (argv[i], "r");
      do_head ();
      fclose (in);
   }
   
   if (flag_done == 0)
      do_head ();
         
   return (EXIT_SUCCESS);
}
