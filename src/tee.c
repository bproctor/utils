
/*
 *  tee -- copy standard input to standard output
 *  Copyright (c) 2001-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: tee long-option\n"
   "       tee [-ai] [file ...]\n"
   "Copy standard input to standard output.\n"
   "\n"
   "  -a         Append to the given file, do not overwrite\n"
   "  -i         Ignore signals\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "https://github.com/bproctor/utils"
};

static char flag_append = 0;
static char flag_ignore = 0;

static void do_tee (int in, int out)
{
   int c;
   char *ptr;
   struct stat s;
   
   fstat (STDIN_FILENO, &s);
   ptr = xmalloc (s.st_blksize);
   while ((c = read (in, ptr, s.st_blksize)) > 0)
      write (out, ptr, c);
}


/*
 * Main
 */
 
int command_tee (int argc, char **argv)
{
   int i, in, out;
   char *ptr;

   in = STDIN_FILENO;
   out = STDOUT_FILENO;
   progname = argv[0];

   for (i = 1; i < argc; ++i)
   {
      ptr = argv[i];
      if (*ptr == '-')
      {
         while (*++ptr)
         switch (*ptr)
         {
            case '-':
               if (!strcmp (ptr, "-help"))
                  puts (help_text);
               else if (!strcmp (ptr, "-version"))
                  puts ("tee: version "VERSION);
               else
                  terror ("invalid option `%s'", argv[i]);
               return (EXIT_SUCCESS);
            case 'a':
               flag_append = 1;
               continue;
            case 'i':
               flag_ignore = 1;
               continue;
         }
      }
      else
      {
         
      }
   }
   do_tee (in, out);
   return (EXIT_SUCCESS);
}
