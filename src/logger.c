
/*
 *  logger -- log messages
 *  Copyright (c) 2003-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: logger long-option\n"
   "       logger [-f file] message\n"
   "Log messages.\n"
   "\n"
   "  -f file    Log messages to `file'\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "https://github.com/bproctor/utils"
};

static FILE *logfile = 0;

static void do_logger (const char *msg)
{
   char *t, *u;
   time_t now;
   
   u = getlogin ();
   if (!u)
      u = "<anonymous>";
   if (time (&now) < 0)
      error ("cannot get current time: %s", strerror (errno));
   t = ctime (&now);
   if (!t)
      error ("cannot get current time: %s", strerror (errno));
   fprintf (logfile, "%.24s:%s: %s\n", t, u, msg);
}

/*
 * Main
 */
 
int command_logger (int argc, char **argv)
{
   int i = 1;
   progname = argv[0];

   if (argc == 1)
      terror ("too few arguments");
   else if (argc >= 2)
   {
      if (!strcmp (argv[1], "--help"))
      {
         puts (help_text);
         return (EXIT_SUCCESS);
      }
      else if (!strcmp (argv[1], "--version"))
      {
         puts ("logger: version "VERSION);
         return (EXIT_SUCCESS);
      }
      else if (!strcmp (argv[1], "-f"))
      {
         if (argc >= 3)
            logfile = xfopen (argv[2], "w");
         i += 2;
      }
      
      if (logfile == NULL)
         logfile = xfopen ("/var/log/syslog", "w");
      fseek (logfile, 0, SEEK_END);
      
      for (; i < argc; ++i)
         do_logger (argv[i]);
   }
   return (EXIT_SUCCESS);
}
