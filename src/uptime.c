
/*
 *  uptime -- print how long the system has been running
 *  Copyright (c) 2001-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: uptime [long-option]\n"
   "Print how long the system has been running.\n"
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
 
int command_uptime (int argc, char **argv)
{
   double uptime;
   unsigned long i, x, d, h, m, s;
   FILE *f;
   
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
         puts ("uptime: version "VERSION);
         return (EXIT_SUCCESS);
      }
   }

   f = xfopen ("/proc/uptime", "r");
   fscanf (f, "%f", &uptime);
   i = (unsigned long)uptime;
   x = (unsigned long)((double)(uptime * 100) - (double)(i * 100));
   d = i / 86400;
   h = (i - (d * 86400)) / 3600;
   m = (i - (d * 86400) - (h * 3600)) / 60;
   s = i % 60;
   
   if (i < 60)
      printf ("%lu.%lu seconds\n", s, x);
   else if (i < 3600)
      printf ("%lu minutes, %lu.%lu seconds\n", m, s, x);
   else if (i < 86400)
      printf ("%lu hours, %lu minutes, %lu.%lu seconds\n", h, m, s, x);
   else
      printf ("%lu days, %lu hours, %lu minutes, %lu.%lu seconds\n",
         d, h, m, s, x);
               
   return (EXIT_SUCCESS);
}
