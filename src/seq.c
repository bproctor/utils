
/*
 *  seq -- print numbers in sequence
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
   "Usage: seq long-option\n"
   "       seq [-f fmt] [-s sep] last\n"
   "       seq [-f fmt] [-s sep] first last\n"
   "       seq [-f fmt] [-s sep] first inc last\n"
   "Print numbers in sequence.\n"
   "\n"
   " -f fmt      Use printf style format 'fmt' for outputing numbers\n"
   " -s sep      Use seperator 'sep' between numbers instead of newline\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "https://github.com/bproctor/utils"
};

static char *seperator = "\n";
static char have_first = 0;
static char have_last = 0;
static char have_inc = 0;
static long double first = 1;
static long double last = 0;
static long double inc = 1;

static void do_seq (void)
{
   long double i;

   if (first < last)
   {
      if (have_inc)
         for (i = first; i <= last; i += inc)
            printf ("%Lf%s", i, seperator);
      else
         for (i = first; i <= last; ++i)
            printf ("%Lf%s", i, seperator);
   }
   else
   {
      if (have_inc)
         for (i = first; i >= last; i -= inc)
            printf ("%Lf%s", i, seperator);
      else
         for (i = first; i >= last; --i)
            printf ("%Lf%s", i, seperator);
   }
}

/*
 * Main
 */
 
int command_seq (int argc, char **argv)
{
   int i;
   char *ptr;
   
   progname = argv[0];

   if (argc == 1)
      terror ("too few arguments");

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
                  puts ("seq: version "VERSION);
               else
                  terror ("invalid option `%s'", argv[i]);
               return (EXIT_SUCCESS);
            case 'f':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operator to `-%c' option", *(ptr-1));
            case 's':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-%c' option", *(ptr-1));
               seperator = ptr;
               break;
         }
      }
      else if (!have_last)
         last = getnum_ld (argv[i]), have_last = 1;
      else if (!have_first)
      {
         long double temp;
         first = getnum_ld (argv[i]);
         have_first = 1;
         temp = first, first = last, last = temp;
      }
      else if (!have_inc)
      {
         long double temp;
         inc = getnum_ld (argv[i]);
         have_inc = 1;
         temp = inc, inc = last, last = temp;
      }
      do_seq ();
      return (EXIT_SUCCESS);
   }
   
   do_seq ();
      
   return (EXIT_SUCCESS);
}
