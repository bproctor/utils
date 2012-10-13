
/*
 *  error.c
 *  Copyright (c) 2002-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <libgen.h>
#include "utils.h"

void (error)(const char *msg, ...)
{
   va_list ap;
   fprintf (stderr, "%s: ", basename (progname));
   va_start (ap, msg);
   vfprintf (stderr, msg, ap);
   va_end (ap);
   fputc ('\n', stderr);
   exit (EXIT_FAILURE); 
}
