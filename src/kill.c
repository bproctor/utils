
/*
 *  kill -- terminate the specified process
 *  Copyright (c) 2003-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: kill long-option\n"
   "       kill [-9] pid\n"
   "Terminate the specified process using SIGTERM.\n"
   "\n"
   "  -9         Terminate the process immediately (SIGKILL)\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "https://github.com/bproctor/utils"
};

static char flag_kill9 = 0;

static void do_kill (char *s)
{
   int pid = 0, err;
   char *ptr = s;
   
   while (isdigit (*ptr))
      pid = pid * 10 + *ptr++ - '0';
      
   if (*ptr != '\0')
      error ("argument must be a valid PID");
   
   if (flag_kill9)
      err = kill (pid, SIGKILL);
   else
      err = kill (pid, SIGTERM);
      
   if (err)
      error ("cannot kill %d: %s", pid, strerror (errno));
}

/*
 * Main
 */
 
int command_kill (int argc, char **argv)
{
   int i;
   
   progname = argv[0];

   if (argc == 1)
      terror ("too few arguments");   
   else if (argc == 2)
   {
      if (!strcmp (argv[1], "--help"))
         puts (help_text);
      else if (!strcmp (argv[1], "--version"))
         puts ("kill: version "VERSION);
      else
         do_kill (argv[1]);
   }
   else if (argc >= 3)
   {
      if (!strcmp (argv[1], "-9"))
         flag_kill9 = 1;
         
      for (i = flag_kill9 ? 2 : 1; i < argc; ++i)
         do_kill (argv[i]);
   }
         
   return (EXIT_SUCCESS);
}
