
/*
 *  cmp -- compare two files
 *  Copyright (c) 1999-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */
 
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

#define BUF_SIZE  32768

static const char help_text[] =
{
   "Usage: cmp long-option\n"
   "       cmp [-l | -s] file1 file2\n"
   "Compare files.\n"
   "\n"
   "  -i n[:m]   Skip the first n bytes, or skip the n - m bytes\n"
   "  -l         Write the byte number and differing bytes\n"
   "  -s         Suppress output, exit status only\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "  Suffixes can be used for the values of the -i option,\n"
   "  the values are then multiplied by the following:\n"
   "    k = 1,000          kb = 1,024\n"
   "    m = 1,000,000      mb = 1,048,576\n"
   "    g = 1,000,000,000  gb = 1,073,741,824\n"
   "\n"
   "https://github.com/bproctor/utils"
};

static char buf1[BUF_SIZE];
static char buf2[BUF_SIZE];
static char flag_quiet = 0;
static char flag_l = 0;
static unsigned long bytes = 1;
static unsigned long lines = 1;
static unsigned long long first = 0;
static unsigned long long last  = 0;
static FILE *file1 = 0;
static FILE *file2 = 0;
static char *name1 = 0;
static char *name2 = 0;

/*
 * Main
 */

int command_cmp (int argc, char **argv)
{
   int i;
   char *ptr;

   progname = argv[0];
   
   for (i = 1; i < argc; ++i)
   {
      ptr = argv[i];
      if (*ptr == '-')
      {
         while (*++ptr)
         switch (*ptr)
         {
            case '\0':
               file1 = stdin;
               break;
            case '-':
               if (!strcmp (ptr, "-help"))
                  puts (help_text);
               else if (!strcmp (ptr, "-version"))
                  puts ("cmp: version "VERSION);
               else
                  terror ("invalid option `%s'", argv[i]);
               return (EXIT_SUCCESS);
            case 'i':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-i' option");

               first = getnum_ul (ptr);
               ptr = strchr (ptr, ':');
               if (ptr)
                  last = getnum_ul (++ptr);
               break;
            case 'l': flag_l = 1; continue;
            case 's': flag_quiet = 1; continue;
         }
      }
      else if (!file1)
      {
         file1 = xfopen (argv[i], "r");
         name1 = argv[i];
      }
      else if (!file2)
      {
         file2 = xfopen (argv[i], "r");
         name2 = argv[i];
      }
   }
   if(!file1 && !file2)
      terror ("too few arguments");
      
   if (file1 == file2)
      return (EXIT_SUCCESS);

   if (!last)
   {
      fseek (file1, 0, SEEK_END);
      last = ftell (file1);
   }
   if (first)
   {
      fseek (file1, first, SEEK_SET);
      fseek (file2, first, SEEK_SET);
      bytes = first;
   }
   else
      fseek (file1, 0, SEEK_SET);

   if (flag_quiet)
   {
      if (last)
      {
         while (ftell (file1) <= last)
         {
            int b = getc (file1);
            int c = getc (file2);
            if (b != c)
               return (EXIT_FAILURE);
            else if (b == EOF || c == EOF)
               break;
         }
      }
      else
      {
         while (1)
         {
            fread (buf1, 1, BUF_SIZE, file1);
            fread (buf2, 1, BUF_SIZE, file2);
            if (memcmp (buf1, buf2, BUF_SIZE))
               return (EXIT_FAILURE);
         }
      }
   }
   else if (flag_l)
   {
      int b, c;
      while (ftell (file1) <= last)
      {
         b = getc (file1);
         c = getc (file2);
         if (b == EOF)
         {
            printf ("cmp: EOF on %s\n", name1);
            break;
         }
         else if (c == EOF)
         {
            printf ("cmp: EOF on %s\n", name2);
            break;
         }
         else if (c != b)
            printf ("%7ld %3o %3o\n", bytes, b, c);
         ++bytes;
      }
   }
   else
   {
      int b, c;
      while (ftell (file1) <= last)
      {
         b = getc (file1);
         c = getc (file2);
         if (c != b)
            break;
         if (c == '\n')
            ++lines;
         else if (c == EOF)
            return (EXIT_SUCCESS);
         ++bytes;
      }

      printf ("%s %s differ: char %ld, line %ld\n", name1, name2, bytes,
         lines);
   }
   return (EXIT_SUCCESS);
}
