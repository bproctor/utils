
/*
 *  date -- Print or change the time and date
 *  Copyright (c) 2003-2012 Brad Proctor <brad@bradleyproctor.com>
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
#include <time.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: date long-option\n"
   "       date [-u] [+format]\n"
   "       date [-u] mmddhhmm[[cc]yy]\n"
   "Print or change the time and date.\n"
   "\n"
   "  -u         Assume UTC time\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "  %a    Abbreviated weekday name (Sun..Sat)\n"
   "  %A    Full weekday name (Sunday..Saturday)\n"
   "  %b    Abbreviated month name (Jan..Dec)\n"
   "  %B    Full month name (January..December)\n"
   "  %c    Date and time (Tue Jan 28 18:32:43 EST 2003)\n"
   "  %d    Day of month (01..31)\n"
   "  %D    Date (mm/dd/yy)\n"
   "  %e    Day of month, padded ( 1..31)\n"
   "  %H    Hour, 24-hour clock (00..23)\n"
   "  %I    Hour, 12-hour clock (01..12)\n"
   "  %j    Day of year (001..366)\n"
   "  %k    Hour, padded, 24-hour clock ( 0..23)\n"
   "  %l    Hour, padded, 12-hour clock ( 1..12)\n"
   "  %m    Month (01..12)\n"
   "  %M    Minutes (01..59)\n"
   "  %n    Newline character\n"
   "  %p    AM or PM\n"
   "  %r    Time, 12-hour (06:32:43PM)\n"
   "  %s    Seconds since epoch, 00:00:00 January 1, 1970\n"
   "  %S    Seconds, (00..60)\n"
   "  %t    Horizontal tab\n"
   "  %T    Time, 24-hour (18:32:43PM)\n"
   "  %U    Weekday of year, with Sunday as first day of week (00..53)\n"
   "  %V    Weekday of year, with Monday as first day of week (01..52)\n"
   "  %w    Day of week (0..6)\n"
   "  %W    Weekday of year, with Monday as first day of week (00..53)\n"
   "  %x    \n"
   "  %X    \n"
   "  %y    Last two digits of year (00..99)\n"
   "  %Y    Year (1970...)\n"
   "  %z    Numeric timezone (-0500)\n"
   "  %Z    Timezone (EST)\n"
   "\n"
   "https://github.com/bproctor/utils"
};

static char flag_utc = 0;

static void print_date (const char *format, time_t t)
{
   size_t bufsize = 0;
   char *buf = NULL;
   struct tm *tm;
   tm = localtime (&t);

   if (format == NULL)
      format = flag_utc ? 
         "%a %b %d %H:%M:%S UTC %Y" :
         "%a %b %d %H:%M:%S %Z %Y";
   else if (*format == '\0')
   {
      putchar ('\n');
      return;
   }
   
   bufsize = 200;
   buf = xmalloc (200);
   do {
      bufsize += 200;
      buf = (char *)xrealloc (buf, bufsize);
      buf[0] = '\1';
   } while (strftime (buf, bufsize, format, tm) == 0 && buf[0] != '\0');
   
   puts (buf);
   free (buf);
}

static void set_date (const char *format)
{
}

/*
 * Main
 */
 
int command_date (int argc, char **argv)
{
   time_t now;
   
   time (&now);
   
   if (argc == 1)
   {
      print_date (NULL, now);
      return (EXIT_SUCCESS);
   }
   else if (argc >= 2)
   {
      if (!strcmp (argv[1], "--help"))
      {
         puts (help_text);
         return (EXIT_SUCCESS);
      }
      else if (!strcmp (argv[1], "--version"))
      {
         puts ("date: version "VERSION);
         return (EXIT_SUCCESS);
      }
      else if (!strcmp (argv[1], "-u"))
      {
         flag_utc = 1;
         if (argc > 2)
         {
            if (*argv[2] == '+')
               print_date (argv[2]+1, now);
            else
               set_date (argv[2]);
         }
         else
            print_date (NULL, now);
      }
      else if (*argv[1] == '+')
         print_date (argv[1]+1, now);
      else
         set_date (argv[1]);
   }
   
   return (EXIT_SUCCESS);
}
