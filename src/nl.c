
/*
 *  nl -- line numbering filter
 *  Copyright (c) 2003-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <string.h>
#include "utils.h"

static const char help_text[] =
{
   "Usage: nl long-option\n"
   "       nl [-p][-b type][-d delim][-f type][-h type][-i incr][-l num]\n"
   "          [-n format][-s sep][-v start][-w width][file]\n"
   "Line numbering filter.\n"
   "\n"
   "  -b type    Format the text body with style 'type'\n"
   "  -d delim   Pages are seperated with 'delim'\n"
   "  -f type    Format footers with style 'type'\n"
   "  -h type    Format headers with style 'type'\n"
   "  -i incr    Increment the line number by 'incr'\n"
   "  -l num     'num' empty lines count as one\n"
   "  -n format  Numbering format (ln, rn, rz)\n"
   "  -p         Do not reset lines at page breaks\n"
   "  -s sep     Add 'sep' after line number, default=tab\n"
   "  -v start   Set first line number on each page to 'start'\n"
   "  -w width   Set column width of line numbers to 'width'\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "'format' can be one of the following:\n"
   "  ln         Left justify, no leading zeros\n"
   "  rn         Right justify, no leading zeros\n"
   "  rz         Right justify, leading zeros\n"
   "'type' can be one of the following:\n"
   "  a          Number all lines\n"
   "  n          Do not number any lines\n"
   "  t          Do not number empty lines\n"
   "\n"
   "https://github.com/bproctor/utils"
};

#define FLAG_ALL 1
#define FLAG_NONE 2
#define FLAG_EMPTY 3

static char *page_sep = 0;
static char flag_page = 0;
static char *seperator = "\t";
static char *format = "%6lu";
static char btype = FLAG_EMPTY;
static char ftype = FLAG_EMPTY;
static char htype = FLAG_EMPTY;
static FILE *in = 0;
static int width = 6;
static unsigned long increment = 1;
static unsigned long lineno = 0;
static char buf[1024];

static void do_nl (void)
{
   int i;
   char fmt1[40], fmt2[40];
   
   strcpy (fmt1, format);
   strcat (fmt1, "%s%s");

   for (i = 0; i < width; ++i)
      fmt2[i] = ' ';
   fmt2[i+1] = '\0';
   strcat (fmt2, "%s%s");

   while (fgets (buf, 1024, in))
   {
      if (buf[0] == '\n')
      {
         if (btype == FLAG_EMPTY)
            putchar ('\n');
         else if (btype == FLAG_NONE)
            printf (fmt2, lineno += increment, seperator, buf);
         else
            printf (fmt1, lineno += increment, seperator, buf);
      }
      else
      {
         printf (fmt1, lineno += increment, seperator, buf);
         if (flag_page)
         {
            if (!strstr (buf, page_sep))
               lineno = 1;
         }
      }
   } 
}

/*
 * Main
 */
 
int command_nl (int argc, char **argv)
{
   int i;
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
                  puts ("nl: version "VERSION);
               else
                  terror ("invalid option `%s'", argv[i]);
               return (EXIT_SUCCESS);
            case 'b':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-%c' option", *(ptr-1));
               if (!strcmp (ptr, "a"))
                  btype = FLAG_ALL;
               else if (!strcmp (ptr, "n"))
                  btype = FLAG_NONE;
               else if (!strcmp (ptr, "t"))
                  btype = FLAG_EMPTY;
               else
                  terror ("invalid body type specified `%s'", ptr);
               break;
            case 'd':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-%c' option", *(ptr-1));
               page_sep = ptr;
               break;
            case 'f':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-%c' option", *(ptr-1));
               if (!strcmp (ptr, "a"))
                  ftype = FLAG_ALL;
               else if (!strcmp (ptr, "n"))
                  ftype = FLAG_NONE;
               else if (!strcmp (ptr, "t"))
                  ftype = FLAG_EMPTY;
               else
                  terror ("invalid footer type specified `%s'", ptr);
               break;
            case 'h':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-%c' option", *(ptr-1));
               if (!strcmp (ptr, "a"))
                  htype = FLAG_ALL;
               else if (!strcmp (ptr, "n"))
                  htype = FLAG_NONE;
               else if (!strcmp (ptr, "t"))
                  htype = FLAG_EMPTY;
               else
                  terror ("invalid header type specified `%s'", ptr);
               break;
            case 'i':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-%c' option", *(ptr-1));
               increment = getnum_ul (ptr);
               break;
            case 'l':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-%c' option", *(ptr-1));
               break;
            case 'n':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-%c' option", *(ptr-1));
               if (!strcmp (ptr, "ln"))
                  format = "%lu";
               else if (!strcmp (ptr, "rn"))
                  format = "%6lu";
               else if (!strcmp (ptr, "rz"))
                  format = "%06lu";
               else
                  terror ("invalid numbering format `%s'", ptr);
               break;
            case 'p':
               flag_page = 1;
               break;
            case 's':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-%c' option", *(ptr-1));
               seperator = ptr;
               break;
            case 'v':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-%c' option", *(ptr-1));
               break;
            case 'w':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-%c' option", *(ptr-1));
               width = getnum_ul (ptr);
               break;
         }
      }
      else if (!in)
         in = xfopen (argv[i], "r");
   }

   if (!in)
      in = stdin;
         
   do_nl ();
   
   return (EXIT_SUCCESS);
}
