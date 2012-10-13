
/*
 *  tac -- output files to the standard output in reverse
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
   "Usage: tac long-option\n"
   "       tac [-b][-s sep] file ...\n"
   "Output files to the standard output in reverse.\n"
   "\n"
   "  -b         \n"
   "  -s sep     Use `sep' as the seperator instead of newline characters\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "https://github.com/bproctor/utils"
};

static FILE *in;
static char buf[1025];
static char flag_seperator = '\n';

/*
 *  This function needs some work.  It is almost there, but it outputs duplicates
 * of some of the first part of the input file at the end of the file.  Try it out
 * to see what I mean.
 */

static void do_tac (void)
{
   int pos;
   char *ptr;
   
   fseek (in, 0, SEEK_END);
   pos = ftell (in);
   if (pos <= 0)
      return;

   while (1)
   {
      pos = (pos <= 1024) ? 0 : pos - 1024;
      fseek (in, pos, SEEK_SET);
      fread (buf, 1, 1024, in);
      while ((ptr = strrchr (buf, flag_seperator)) != NULL)
      {
         if (ptr)
            *ptr = '\0', ++ptr;
         else
            break;
         puts (ptr);
      }
      if (pos <= 0)
         break;
   }
}

/*
 * Main
 */
 
int command_tac (int argc, char **argv)
{
   int i;
   char *ptr;
   
   progname = argv[0];
   
   for (i = 1; i < argc; ++i)
   {
      ptr = argv[i];
      if (*ptr == '-')
      {
         switch (*++ptr)
         {
            case '-':
               if (!strcmp (ptr, "-help"))
                  puts (help_text);
               else if (!strcmp (ptr, "-version"))
                  puts ("tac: version "VERSION);
               else
                  terror ("invalid option `%s'", argv[i]);
               return (EXIT_SUCCESS);
            case 'b':
               continue;
            case 's':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-s' option");
               break;
         }
      }
      else
      {
         in = xfopen (argv[1], "r");
         do_tac ();
         fclose (in);
      }
   }
      
   if (!in)
   {
      in = stdin;
      do_tac ();
   }
         
   return (EXIT_SUCCESS);
}
