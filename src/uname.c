
/*
 *  uname -- print information about the system
 *  Copyright (c) 2003-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include <sys/utsname.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: uname long-option\n"
   "       uname [-amnrsv]\n"
   "Print info about the system.\n"
   "  -a         Behave as if all options (-mnrsv) where specified\n"
   "  -m         Print the machine type on which the system is running\n"
   "  -n         Print the name of this node\n"
   "  -r         Print the release level of the operating system\n"
   "  -s         Print the name of the implementation of the operating system\n"
   "  -v         Print the version of the operating system\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "https://github.com/bproctor/utils"
};

#define FLAG_M    0x01
#define FLAG_N    0x02
#define FLAG_R    0x04
#define FLAG_S    0x08
#define FLAG_V    0x10

static int flag = 0;

/*
 * Main
 */
 
int command_uname (int argc, char **argv)
{
   int i;
   char *ptr;
   struct utsname u;
   
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
                  puts ("uname: version "VERSION);
               else
                  terror ("invalid option `%s'", argv[i]);
               return (EXIT_SUCCESS);
            case 'a': flag |= FLAG_M|FLAG_N|FLAG_R|FLAG_S|FLAG_V; continue;
            case 'm': flag |= FLAG_M; continue;
            case 'n': flag |= FLAG_N; continue;
            case 'r': flag |= FLAG_R; continue;
            case 's': flag |= FLAG_S; continue;
            case 'v': flag |= FLAG_V; continue;
            default:
               terror ("invalid option `-%c'", *ptr);
         }
      }
   }
   if (uname (&u) == -1)
      error ("cannot get system information: %s", strerror (errno));

   if (flag & FLAG_S || flag == 0)
   {
      fputs (u.sysname, stdout);
      flag &= ~FLAG_S;
      if (flag)
         putchar (' ');
   }
   if (flag & FLAG_N)
   {
      fputs (u.nodename, stdout);
      flag &= ~FLAG_N;
      if (flag)
         putchar (' ');
   }
   if (flag & FLAG_R)
   {
      fputs (u.release, stdout);
      flag &= ~FLAG_R;
      if (flag)
         putchar (' ');
   }
   if (flag & FLAG_V)
   {
      fputs (u.version, stdout);
      flag &= ~FLAG_V;
      if (flag)
         putchar (' ');
   }
   if (flag & FLAG_M)
      fputs (u.machine, stdout);
   putchar ('\n');
   
   return (EXIT_SUCCESS);
}
