
/*
 *  split -- split files into pieces
 *  Copyright (c) 1999-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include "utils.h"

#define BUF_SIZE  65536

static const char help_text[] =
{
   "Usage: split long-option\n"
   "       split [-a n][-b n][-l n][-line_count] file ...\n"
   "Split files into pieces.\n"
   "\n"
   "  -a suffix_length  Use suffix_length letters for the split filenames\n"
   "  -b n              Split a file into n bytes in size\n"
   "  -b nk             Split a file into n*1024 bytes in size\n"
   "  -b nm             Split a file into n*1048576 bytes in size\n"
   "  -l line_count     Number of lines in each file\n"
   "  -line_count       Number of lines in each file\n"
   "\n"
   "Long Options:\n"
   "  --help            Print this help message\n"
   "  --version         Print the program version\n"
   "\n"
   "Report bugs to <bugs@plural-os.org>"
};

static char buf[BUF_SIZE];
static char *inname = "x";
static FILE *in;
static FILE *out = 0;
static char flag_lines = 0;
static int suffix_length = 2;

static FILE *file_open (char *name, int num)
{
   char sname[1024];
   if (num > 676)
      error ("exceeded maximum number of split files");
   else
      sprintf (sname, "%s%c%c", name, num / 26 + 'a', num % 26 + 'a');
   return (fopen (sname, "wb"));
}

/*
 * Main
 */

int command_split (int argc, char **argv)
{
   int i;
   unsigned long long size = 0;
   unsigned long num_lines = 1000;
   char *ptr;

   in = stdin;
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
                  puts ("split: version "VERSION);
               else
                  terror ("invalid option `%s'", argv[i]);
               return (EXIT_SUCCESS);
            case 'a':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-a' option");
               suffix_length = getnum_ul (ptr);
               break;
            case 'b':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-b' option");
               size = getnum_ul (ptr);
               break;
            case 'l':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-l' option");
               num_lines = getnum_ul (ptr);
               flag_lines = 1;
               break;
            case '0':   case '1':   case '2':
            case '3':   case '4':   case '5':
            case '6':   case '7':   case '8':
            case '9':
               num_lines = getnum_ul (ptr);
               flag_lines = 1;
               ptr = " ";
               break;
            default:
               in = xfopen (argv[i], "r");
               inname = argv[i];
         }
      }
      else if (in == stdin)
         in = xfopen (argv[i], "r");
      else
         inname = argv[i];
   }
   if (!flag_lines)
   {
      int number = 0;
      unsigned long bytes_left = size;
      unsigned long bytes_read = 0;      
      out = file_open (inname, number);
      
      while (1)
      {
         if (bytes_left >= BUF_SIZE)
            bytes_read = fread (buf, 1, BUF_SIZE, in);
         else
            bytes_read = fread (buf, 1, bytes_left, in);

         if (bytes_read <= 0)
         {
            fclose (out);
            fclose (in);
            return (EXIT_SUCCESS);
         }
         fwrite (buf, 1, bytes_read, out);
         bytes_left -= bytes_read;
         if (bytes_left == 0)
         {
            fclose (out);
            number++;
            out = file_open (inname, number);
            bytes_left = size;
         }
      }
   }
   else
   {
      int number = 0, lines = 0;
      out = file_open (inname, number);
      while (fgets (buf, BUF_SIZE, in) != NULL)
      {
         if (lines >= num_lines)
         {
            fclose (out);
            out = file_open (inname, number);
            lines = 0;
            ++number;
         }
         fputs (buf, out);
         ++lines;
      }
   }
   return (EXIT_SUCCESS);
}
