
/*
 *  cal -- print a calender
 *  Copyright (c) 2001-2012 Brad Proctor <brad@bradleyproctor.com>
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
#include <time.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: cal long-option\n"
   "       cal [[month] year]\n"
   "Prints a calender.\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
};

static const char *months[] =
{
   "", "January", "February", "March", "April", "May",
   "June", "July", "August", "September", "October",
   "November", "December"
};

static const unsigned days[] =
   {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
   
static const unsigned ldays[] =
   {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static const unsigned days1[] =
   {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

static const unsigned ldays1[] =
   {0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};

/*
 *   Before September 1752, leap years were calculated every four years.
 * This was slightly inaccurate so the formula was changed to every 4 years
 * except when the year is divisible by 100, unless that year is also
 * divisible by 400.  
 *
 * year 100: is a leap year, uses the old rule of (year % 4)
 * year 1800: not a leap year, the year is divisible by 4 & 100 but not by
 *            400 which makes it not a leap year.
 * year 2000: is a leap year, the year is divisible by 4, 100 and 400.
 * 
 *   Because of the past inaccuracy, in September 1752, 11 days are
 * missing from the calender. It was Wednesday, September 2, 1752 at 11:59pm
 * one minute later it was Thursday, September 14, 1752 at midnight.
 */

static unsigned isleapyear (unsigned year)
{
   if (year >= 1752)
   {
      if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
         return (1);
   }
   else
   {
      if (year % 4 == 0)
         return (1);
   }
   return (0);
}

static void print_cal (unsigned month, unsigned year)
{
   unsigned i, day;
   printf ("     %s %d\n Su Mo Tu We Th Fr Sa\n", months[month], year);
   
   /* January 1, 1 was a Saturday */
   for (i = 1, day = 6; i < year; ++i)
      day += isleapyear (i) == 0 ? 1 : 2;

   if (year > 1752 && month > 9)
      day -= 11;
      
   day = (day + (isleapyear (year) == 0 ? days1[month] : ldays1[month])) % 7;
   
   if (year == 1752 && month == 9)
   {
      fputs ("        1  2 14 15 16\n"
             " 17 18 19 20 21 22 23\n"
             " 24 25 26 27 28 29 30\n", stdout);
      return;
   }

   for (i = 0; i < day; ++i)
      fputs ("   ", stdout);

   for (i = 1; i <= (isleapyear (year) == 0 ? days[month] : ldays[month]); ++i)
   {
      if (++day > 7)
      {
         putchar ('\n');
         day = 1;
      }
      printf (" %2d", i);
   }
   putchar ('\n');
}

/*
 * Main
 */
 
int command_cal (int argc, char **argv)
{
   unsigned i, x, year = 2002, month = 12;
   unsigned char got_year = 0, got_month = 0;
   struct tm *t;
   time_t ti;
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
                  puts ("cal: version "VERSION);
               else
                  terror ("invalid option `%s'", argv[i]);
               return (EXIT_SUCCESS);
         }
      }
      else if (isdigit (*ptr))
      {
         x = getnum_ul (ptr);
         if (got_year == 0)
            year = x, got_year = 1;
         else if (got_month == 0)
         {
            month = year;
            year = x;
            got_month = 1;
            if (year < 1)
               terror ("invalid year value, must be 1-????\n");
            if (month > 12 || month < 1)
               terror ("invalid month value, must be 1-12\n");
         }
      }
   }

   if (got_year == 0 || got_month == 0)
   {
      t = xmalloc (sizeof (struct tm));
      ti = time (NULL);
      if (ti < 0)
         error ("cannot get current time: %s", strerror (errno));
      if (localtime_r (&ti, t) == NULL)
         error ("cannot get current time: %s", strerror (errno));
      if (got_year == 0)
         year = t->tm_year + 1900, got_year = 1;
      if (got_month == 0)
         month = t->tm_mon + 1, got_month = 1;
   }
         
   print_cal (month, year);
   return (EXIT_SUCCESS);
}
