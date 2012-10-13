
/*
 *  cut -- cut out selected fields of each line of a file
 *  Copyright (c) 2002-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: cut long-option\n"
   "       cut -b list [-n] [file ...]\n"
   "       cut -c list [file ...]\n"
   "       cut -f list [-d delim][-s][file ...]\n"
   "Cut out selected fields of each line of a file.\n"
   "\n"
   "  -b         Cut based on list of bytes\n"
   "  -c         Cut based on list of characters\n"
   "  -d delim   Set the delimiter to 'delim'\n"
   "  -f         Cut based on list of fields\n"
   "  -n         Do not split characters\n"
   "  -s         Suppress lines with no delimiter\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "Ranges:\n"
   "  x     Output the x'th character or field\n"
   "  x-    Output from the x'th character or field on\n"
   "  -x    Output up to the x'th character or field\n"
   "  x-y   Output from x'th to the y'th character or field\n"
   "\n"
   "https://github.com/bproctor/utils"
};

static char flag_character = 0;
static char flag_field = 0;

#define RANGE_ONE    1     /* x   */
#define RANGE_FROM   2     /* x-  */
#define RANGE_TO     3     /* -y  */
#define RANGE_SOME   4     /* x-y */

static int range_type;
static int range_x, range_y;
static char buffer[4096];
static FILE *in;

static void get_range (const char *arg)
{
   const char *ptr = arg;
   if (isdigit (*ptr))
   {
      do
         range_x = range_x * 10 + *ptr - '0';
      while (isdigit (*++ptr));
      range_type = RANGE_ONE;
   }
   if (*ptr == '-')
   {
      range_type = (range_type == RANGE_ONE) ? RANGE_FROM : RANGE_TO;
      ++ptr;
   }
      
   if (isdigit (*ptr))
   {
      do
         range_y = range_y * 10 + *ptr - '0';
      while (isdigit (*++ptr));
      if (range_type == RANGE_FROM)
         range_type = RANGE_SOME;
   }
}

static int read_line (char *buf, FILE *file)
{
   char *ptr = buf;
   int c;
   
   while (1)
   {
      c = getc (file);
      if (c == '\n')
      {
         *ptr++ = '\n';
         break;
      }
      else if (c == EOF)
         break;
      *ptr++ = c;
   }
   *ptr = '\0';
   if (c == '\n')
      return (0);
   return (1);
}


static void do_cut (void)
{
   char *ptr;

   
   while (1)
   {
      read_line (buffer, in);
      ptr = buffer;
      if (*ptr == '\0')
         break;
      else if (*ptr == '\n')
      {
         putchar ('\n');
         continue;
      }
         
      switch (range_type)
      {
         case RANGE_ONE:   
            putchar (ptr[range_x]);
            break;
         case RANGE_FROM:
            ptr += range_x - 1;
            while (*ptr != '\0')
               putchar (*ptr++);
            break;
         case RANGE_TO:
            while (*ptr != '\0' && range_y > (ptr - buffer))
               putchar (*ptr++);
            break;
         case RANGE_SOME:
            ptr += range_x - 1;
            while (*ptr != '\0' && range_y > (ptr - buffer))
               putchar (*ptr++);
            break;
      }
      if (*(ptr - 1) != '\n')
         putchar ('\n');
   }
}

/*
 * Main
 */
 
int command_cut (int argc, char **argv)
{
   int i;
   char *ptr;

   progname = argv[0];
   
   if (argc == 1)
      terror ("no source file specified");
   
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
                  puts ("cut: version "VERSION);
               else
                  terror ("invalid option `%s'", argv[i]);
               return (EXIT_SUCCESS);
            case 'b':
               /* Fall through */
            case 'c':
               flag_character = 1;
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-c' option");
               get_range (argv[i]);
               break;
            case 'f':
               flag_field = 1;
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-f' option");
               get_range (argv[i]);
               break;
            case 'd':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-d' option");
               break;
            case 'n':
               continue;
            case 's':
               continue;
         }
      }
      if (in == NULL)
         in = xfopen (ptr, "r");
   }
   if (flag_character && flag_field)
      error ("use only one of `-b', `-c', or `-f'", stderr);

   do_cut ();

   return (EXIT_SUCCESS);
}
