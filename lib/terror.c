
/*
 *  terror.c -- ooh scary
 *  Copyright (c) 2002-2012 Brad Proctor
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <libgen.h>
#include "utils.h"

/*
 * This is the same as error except that it
 * prints an extra message.
 */

void (terror)(const char *msg, ...)
{
   char *p = basename (progname);
   va_list ap;
   fprintf (stderr, "%s: ", p);
   va_start (ap, msg);
   vfprintf (stderr, msg, ap);
   va_end (ap);
   fprintf (stderr, "\nTry `%s --help' for more information\n", p);
   exit (EXIT_FAILURE); 
}
