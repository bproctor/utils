
/*
 *  prezip.c -- process multimedia files for better compression
 *  Copyright (c) 2001-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: prezip long-option\n"
   "       prezip [-cd] infile [outfile]\n"
   "Process multimedia files for better compression"
   "\n"
   "  -c         Output to standard output\n"
   "  -d         Decode\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the version\n"
   "\n"
   "https://github.com/bproctor/utils"
};

static char flag_decode = 0;
static char *inname;
static char *outname;
static FILE *infile;
static FILE *outfile;
static char *inbuf, *inptr;
static char *outbuf, *outptr;
static unsigned int incount;

static void encode (void)
{
   incount = fread (inbuf, 1, 65535, infile);
   inptr = inbuf;
   if (!incount)
      error ("cannot encode empty file");
   *outptr++ = *inptr++;
   while (1)
   {
      while (--incount)
         *outptr = *inptr - *(inptr-1), ++inptr, ++outptr;
      fwrite (outbuf, 1, outptr - outbuf, outfile);
      outptr = outbuf;
      incount = fread (inbuf, 1, 65535, infile);
      inptr = inbuf;
      if (!incount)
         break;
   }
}

static void decode (void)
{
   incount = fread (inbuf, 1, 65535, infile);
   inptr = inbuf;
   if (!incount)
      error ("cannot decode empty file");
   *outptr++ = *inptr++;
   while (1)
   {
      while (--incount)
         *outptr = *(outptr-1) + *inptr, ++inptr, ++outptr;
      fwrite (outbuf, 1, outptr - outbuf, outfile);
      outptr = outbuf;
      incount = fread (inbuf, 1, 65535, infile);
      inptr = inbuf;
      if (!incount)
         break;
   }
}

/*
 * Main
 */
 
int command_prezip (int argc, char **argv)
{
   int i;
   char *ptr;
   
   if (argc < 2)
      error ("no input file specified");
   
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
                  puts ("prezip: version "VERSION);
               else
                  terror ("invalid option `%s'", argv[i]);
               return (EXIT_SUCCESS);
            case 'c':
               continue;
            case 'd':
               flag_decode = 1;
               continue;
         }
      }
      else if (!infile)
      {
         inname = xmalloc (strlen (argv[i]));
         strcpy (inname, argv[i]);
         infile = xfopen (inname, "r");
      }
      else if (!outfile)
      {
         outname = xmalloc (strlen (argv[i]));
         strcpy (outname, argv[i]);
         outfile = xfopen (outname, "w");
      }
   }   
   if (!infile)
      terror ("no input file specified");
   if (!outfile)
   {
      if (!strcmp (strrchr (inname, '.'), ".pz"))
      {
         if (flag_decode)
         {
            outname = xmalloc (strlen (inname));
            strcpy (outname, inname);
            strcat (strrchr (outname, '.'), "");
            outfile = xfopen (outname, "w");
         }
         else
            error ("file already has .pz extention");
      }
      else
      {   
         outname = xmalloc (strlen (inname)+3);
         strcpy (outname, inname);
         strcat (outname, ".pz");
         outfile = xfopen (outname, "w");
      }
   }
   inbuf = xmalloc (65536);
   outbuf = xmalloc (65536);
   inptr = inbuf;
   outptr = outbuf;   

   if (flag_decode)
      decode ();
   else
      encode ();   

   free (inbuf);
   free (outbuf);
   fclose (infile);
   fclose (outfile);
   return (EXIT_SUCCESS);
}
