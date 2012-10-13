
/*
 *  printf.c -- write formatted output
 *  Copyright (c) 2001-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: printf long-option\n"
   "       printf format...\n"
   "Write formatted output.\n"
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
 
int command_printf (int argc, char **argv)
{
   int count = 2;
   long long i;
   long double f;
   char *ptr;
   
   progname = argv[0];

   if (argc == 2)
   {
      if (!strcmp (argv[1], "--help"))
      {
         puts (help_text);
         return (EXIT_SUCCESS);
      }
      else if (!strcmp (argv[1], "--version"))
      {
         puts ("printf: version "VERSION);
         return (EXIT_SUCCESS);
      }
   }
   ptr = argv[1];
   while (*ptr != '\0')
   {
      if (*ptr == '%')
      {
         if (isdigit (*++ptr))
         {
            if (*ptr == '0')
               ;
         }
         switch (*ptr)
         {
            case 'd':
               i = getnum_ll (argv[count++]);
               break;
            case 'f':
               f = getnum_ld (argv[count++]);
               break;
            case 'i':
               i = getnum_ll (argv[count++]);
               break;
            case 'l':
               break;
            case 's':
               fputs (argv[count++], stdout);
               break;
            case 'u':
               i = getnum_ll (argv[count++]);
               break;
            case 'x':
               i = getnum_ll (argv[count++]);
               break;
         }
         ++ptr;
      }
      else if (*ptr == '\\')
      {
         switch (*++ptr)
         {
            case 'a':   putchar (7);      break;
            case 'b':   putchar (8);      break;
            case 'c':   exit (0);
            case 'f':   putchar (12);     break;
            case 'n':   putchar (10);     break;
            case 'r':   putchar (13);     break;
            case 't':   putchar (9);      break;
            case 'v':   putchar (11);     break;
            default:    putchar (*ptr);   break;
         }
         ++ptr;
      }
      else
         putchar (*ptr++);
   }
   
   return (EXIT_SUCCESS);
}
