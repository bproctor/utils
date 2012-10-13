
/*
 *  sum -- calculate a checksum
 *  Copyright (c) 1999-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: sum long-option\n"
   "       sum [-r|-s][-b n] file ...\n"
   "Calculate a checksum.\n"
   "\n"
   "  -r         Use the BSD algorithm, defaults to 1024-byte blocks\n"
   "  -s         Use the SYSV algorithm, defaults to 512-byte blocks\n"
   "  -b n       Set the block size to 2^n\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "https://github.com/bproctor/utils"
};

static char flag_bsd = 0;
static char buf[512];
static int block_size = 0;
static FILE *in;

static void do_bsd_sum (const char *name, FILE *in)
{
   int c, sum = 0;
   unsigned long count = 0;
   
   while ((c = getc (in)) != EOF)
      sum = ((sum >> 1) + ((sum & 1) << 15) + c) & 0xFFFF, ++count;
   count = ((count + (block_size - 1)) & ~(block_size - 1)) / block_size;
   if (in == stdin)
      printf ("%05d %5lu\n", sum, count);
   else
      printf ("%05d %5lu %s\n", sum, count, name);
}

static void do_sysv_sum (const char *name, FILE *in)
{
   unsigned long j, k;
   unsigned long sum = 0;
   unsigned long count = 0;

   while ((k = fread (buf, 1, block_size, in)) > 0)
   {
      j = 0;
      while (j < k)
         sum += buf[j++];
      count += k;
      j = (sum & 0xFFFF) + ((sum & 0xFFFFFFFF) >> 16);
      sum = (j & 0xFFFF) + (j >> 16);
   }
   count = ((count + (block_size - 1)) & ~(block_size - 1)) / block_size;

   if (in == stdin)
      printf ("%lu %lu\n", sum, count);
   else
      printf ("%lu %lu %s\n", sum, count, name);
}

/*
 * Main
 */

int command_sum (int argc, char **argv)
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
               break;
            case '-':
               if (!strcmp (ptr, "-help"))
                  puts (help_text);
               else if (!strcmp (ptr, "-version"))
                  puts ("sum: version "VERSION);
               else
                  terror ("invalid option `%s'", argv[i]);
               return (EXIT_SUCCESS);
            case 'r':
               flag_bsd = 1;
               continue;
            case 's':
               flag_bsd = 0;
               continue;
            case 'b':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-b' option");
               
               block_size = getnum_ul (ptr);
               if (block_size < 1 || block_size > 31)
                  terror ("invalid block size, must be 1-31");
               block_size = 2 << block_size;
               break;
            default:
               in = xfopen (argv[i], "r");
         }
      }   
      else
      {
         if (in != stdin)
            fclose (in);
         in = xfopen (argv[i], "r");
      }

      if (block_size == 0)
         block_size = flag_bsd ? 1024 : 512;

      if (flag_bsd)
         do_bsd_sum (argv[i], in);
      else
         do_sysv_sum (argv[i], in);
   }
   return (EXIT_SUCCESS);
}
