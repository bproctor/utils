
/*
 *  wc -- word, line and byte or character count
 *  Copyright (c) 2001-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

#define FLAG_BYTES      0x01
#define FLAG_WORDS      0x02
#define FLAG_LINES      0x04

static unsigned flags;
static unsigned total_lines, lines;
static unsigned total_words, words;
static unsigned total_bytes, bytes;
static char number[9];
static char buffer[4096];
static void (*get_totals)(FILE *);

static const char help_text[] =
{
   "Usage: wc long-option\n"
   "       wc [-c | -m] [-lw] file ...\n"
   "\n"
   "  -c         Print the character count\n"
   "  -m         Print the byte count\n"
   "  -l         Print the line count\n"
   "  -w         Print the word count\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "https://github.com/bproctor/utils"
};

static void itoa (unsigned i)
{
   char *ptr;
   strcpy (number, "         ");
   ptr = &number[6];   
   while (i > 0 && ptr >= &number[0])
      *ptr-- = i % 10 + '0', i /= 10;
   number[8] = '\0';
}

static void get_chars (FILE *in)
{
   fseek (in, 0, SEEK_END);
   bytes = ftell (in);
}

static void get_lines (FILE *in)
{
   unsigned i, count;
   
   lines = 0;
   while (1)
   {
      count = fread (buffer, 1, 4096, in);
      if (count == 0)
         break;
      for (i = 0; i < count; ++i)
         if (buffer[i] == '\n')
            lines++;
   }
}

static void get_normal (FILE *in)
{
   unsigned i, count, word;
   
   bytes = lines = words = 0;
   while (1)
   {
      word = 0;
      count = fread (buffer, 1, 4096, in);
      if (count == 0)
         break;
      bytes += count;
      for (i = 0; i < count; ++i)
      {
         if (isspace (buffer[i]))
         {
            if (buffer[i] == '\n')
               lines++;
            if (word == 1)
               words++, word = 0;
         }
         else if (word == 0)
            word = 1;         
      }
   }
}

/*
 * Main
 */

int command_wc (int argc, char **argv)
{
   int i, files;
   char *ptr;
   FILE *in;

   flags = 0;
   i = files = 0;
   
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
                  puts ("wc: version "VERSION);
               else
                  terror ("invalid option `%s'", argv[i]);
               return (EXIT_SUCCESS);
            case 'c':
               flags |= FLAG_BYTES;
               continue;
            case 'l':
               flags |= FLAG_LINES;
               continue;
            case 'm':
               flags |= FLAG_BYTES;
               continue;
            case 'w':
               flags |= FLAG_WORDS;
               continue;
         }
      }
      else
         break;
   }
   if (flags == 0)
   {
      flags |= FLAG_BYTES|FLAG_WORDS|FLAG_LINES;
      get_totals = get_normal;
   }
   else if (flags == FLAG_LINES)
      get_totals = get_lines;   
   else if (flags == FLAG_BYTES)
      get_totals = get_chars;
   else
      get_totals = get_normal;

   for (; i < argc; ++i)
   {
      char *ptr, display[100];
      if ((in = fopen (argv[i], "r")) == 0)
         warning ("cannot open `%s' for reading: %s", strerror (errno));
      get_totals (in);
      fclose (in);
      ptr = display;
      if (flags & FLAG_LINES)
         itoa (lines), strcpy (ptr, number), ptr += 8;
      if (flags & FLAG_WORDS)
         itoa (words), strcpy (ptr, number), ptr += 8;
      if (flags & FLAG_BYTES)
         itoa (bytes), strcpy (ptr, number), ptr += 8;
      strcpy (ptr, argv[i]);
      puts (display);
      total_lines += lines;
      total_words += words;
      total_bytes += bytes;
      ++files;
   }
   if (files == 0)
      get_totals (stdin);
   else if (files > 1)
   {
      char *ptr, display[100];
      ptr = display;
      if (flags & FLAG_LINES)
         itoa (total_lines), strcpy (ptr, number), ptr += 8;
      if (flags & FLAG_WORDS)
         itoa (total_words), strcpy (ptr, number), ptr += 8;
      if (flags & FLAG_BYTES)
         itoa (total_bytes), strcpy (ptr, number), ptr += 8;
      strcpy (ptr, "total");
      puts (display);
   }
   return (EXIT_SUCCESS);
}
