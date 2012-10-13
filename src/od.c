
/*
 *  od -- dump files in various formats
 *  Copyright (c) 1999-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: od long-options\n"
   "       od [-ANbcdjostvx] file ...\n"
   "Dump files in different formats.\n"
   "\n"
   "  -A addr_base  Radix that the address is printed in\n"
   "     d, o, x    d = decimal, o = octal, x = hex. o is default\n"
   "  -N count      \n"
   "  -b            Interpret bytes in octal\n"
   "  -c            Print bytes as characters\n"
   "  -d            Print words (2 bytes) in unsigned decimal\n"
   "  -j skip       Skip the first `skip' bytes\n"
   "  -o            Print words in octal\n"
   "  -s            Print words as signed decimal\n"
   "  -t string     \n"
   "  -v            Write all output\n"
   "  -x            Print words in hexidecimal\n"
   "\n"
   "Long Options:\n"
   "  --help        Print this help message\n"
   "  --version     Print the program version\n"
   "\n"
   "https://github.com/bproctor/utils"
};

static const char str_table[][5] =
{
   " nul", " soh", " stx", " etx", " eot", " enq", " ack", " bel",
   "  bs", "  ht", "  nl", "  vt", "  ff", "  cr", "  so", "  si",
   " dle", " dc1", " dc2", " dc3", " dc4", " nak", " syn", " etb",
   " can", "  em", " sub", " esc", "  fs", "  gs", "  rs", "  us",
   "  sp",
};

static FILE *in;

static char flag_named = 0;
static char flag_chars = 0;
static char flag_dec = 0;
static unsigned int count = 0;
static char addr_base[] = "%07o";
static char type[] = "%06o";

static void print_default (void)
{
   int b, c, i = 0;

   while (1)
   {
      c = getc (in);
      if (c == EOF)
         break;
      b = getc (in);
      if (b == EOF)
      {
         putchar (' ');
         printf (type, c);
         break;
      }
      c = (b << 8) + c;
      putchar (' ');
      printf (type, c);
      if (++i == 8)
      {
         putchar ('\n');
         printf (addr_base, (int)ftell (in));
         i = 0;
      }
   }
}

static void print_named (void)
{
   int c, i = 0;
   while (1)
   {
      c = getc (in);
      if (c == EOF)
         break;
      if (c <= 040)
         fputs (str_table[c], stdout);
      else if (c == 0177)
         fputs (" del", stdout);
      else
         printf ("   %c", c);
      if (++i == 16)
      {
         putchar ('\n');
         printf (addr_base, (int)ftell (in));
         i = 0;
      }
   }
}

static void print_chars (void)
{
   int c, i = 0;

   while (1)
   {
      c = getc (in);
      if (c == EOF)
         break;
      switch (c)
      {
         case '\0': fputs ("  \\0", stdout); break;
         case '\a': fputs ("  \\a", stdout); break;
         case '\b': fputs ("  \\b", stdout); break;
         case '\f': fputs ("  \\f", stdout); break;
         case '\n': fputs ("  \\n", stdout); break;
         case '\r': fputs ("  \\r", stdout); break;
         case '\t': fputs ("  \\t", stdout); break;
         case '\v': fputs ("  \\v", stdout); break;
         default:
            if (isprint (c))
            {
               fputs ("   ", stdout);
               fputc (c, stdout);
            }
            else
               printf (" %03o", c);
      }
      if (++i == 16)
      {
         putchar ('\n');
         printf (addr_base, (int)ftell (in));
         i = 0;
      }
   }
}

static void print_u2 (void)
{
   int b, c, i = 0;

   while (1)
   {
      c = getc (in);
      if (c == EOF)
         break;
      b = getc (in);
      if (b == EOF)
      {
         printf (" %5d", (unsigned short)c);
         break;
      } 
      c = (b << 8) + c;
      printf (" %5d", (unsigned short)c);
      if (++i == 8)
      {
         putchar ('\n');
         printf (addr_base, (int)ftell (in));
         i = 0;
      }
   }
}

static void do_od (void)
{
   printf (addr_base, 0);
   if (flag_named)
      print_named ();
   else if (flag_chars)
      print_chars ();
   else if (flag_dec)
      print_u2 ();
   else
      print_default ();
   putchar ('\n');
   printf (addr_base, (int)ftell (in));
   putchar ('\n');
}

/*
 * Main
 */

int command_od (int argc, char **argv)
{
   int i;
   unsigned int j = 0;
   char *ptr;

   progname = argv[0];   
   in = stdin;
   
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
                  puts ("od: version "VERSION);
               else
                  terror ("invalid option `%s'", argv[i]);
               return (EXIT_SUCCESS);
            case 'b':
               continue;
            case 'c':
               flag_chars = 1;
               continue;
            case 'd':
               flag_dec = 1;
               continue;
            case 'j':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-j' option");
               
               j = getnum_ul (ptr);
               break;
            case 'o':
               continue;
            case 's':
               continue;
            case 't':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-t' option"); 
              
               switch (*ptr)
               {
                  case 'a':   flag_named = 1; break;
                  default:
                     terror ("invalid typestring");
               }
               break;                     
            case 'A':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-A' option");

               if (*(ptr+1) != '\0')
                  terror ("invalid address base `%s'", ptr);
               switch (*ptr)
               {
                  case 'd': addr_base[3] = 'd'; break;
                  case 'o': addr_base[3] = 'o'; break;
                  case 'x': addr_base[3] = 'x'; break;
                  default:
                     terror ("invalid address base `%s'", ptr);
               }
               break;
            case 'N':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-N' option");
               count = getnum_ull (ptr);
               break;
            default:
               terror ("invalid option `%s'", argv[i]);
         }
         break;
      }
      else
      {
         in = xfopen (argv[i], "r");
         if (j)
         {
            fseek (in, j, SEEK_SET);
            j = 0;
         }
         do_od ();
         fclose (in);
      }
   }
   if (in == stdin)
      do_od ();

   return (EXIT_SUCCESS);
}
