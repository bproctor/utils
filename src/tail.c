
/*
 *  tail -- Print the last part of a file
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
   "Usage: tail long-option\n"
   "       tail file...\n"
   "Print the last part of a file.\n"
   "\n"
   "  -c num     Print num characters\n"
   "  -n num     Print num lines\n"
   "  -num       Print num lines\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "https://github.com/bproctor/utils"
};

static char flag_lines = 1;
static int bytes = 800;
static int lines = 10;
static char *buf[1024];
static char string[1024];

static void do_tail (FILE *in)
{
   int i, count = 0;
   unsigned long pos;

   fseek (in, 0, SEEK_END);
   pos = ftell (in);
   
   while (count < lines)
   {
      pos = pos > 1024 ? pos - 1024 : 0;
      fseek (in, pos, SEEK_SET);
      fread (in, 0, 1024, (void *)string);
      while (count < lines)
      {
         buf[count] = strrchr (string, '\n');
         if (buf[count])
            buf[count++] = '\0';
         else
            break;
      }
   }   
   for (i = 0; i < count; ++i)
      puts (buf[i]);
}

/*
 * Main
 */
 
int command_tail (int argc, char **argv)
{
   int i, done = 0;
   char *ptr;
   
   progname = argv[0];
   
   for (i = 1; i < argc; ++i)
   {
      ptr = argv[0];
      if (*ptr == '-')
      {
         switch (*++ptr)
         {
            case '-':
               if (!strcmp (ptr, "-help"))
                  puts (help_text);
               else if (!strcmp (ptr, "-version"))
                  puts ("tail: version "VERSION);
               else
                  terror ("invalid option `%s'", argv[i]);
               return (EXIT_SUCCESS);
            case 'c':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
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
               /* Fall through */
            case '0': case '1': case '2': case '3':
            case '4': case '5': case '6': case '7':
            case '8': case '9':
               lines = getnum_ul (ptr);
               flag_lines = 1;
               break;
         }
      }
      else
      {
         FILE *in = xfopen (argv[i], "r");
         do_tail (in);
         done = 1;
      }
   }
   
   /* In case no input files were on the command line, use stdin */
   
   if (!done)
      do_tail (stdin);
      
   return (EXIT_SUCCESS);
}
