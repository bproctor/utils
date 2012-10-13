
/*
 *  tabs -- set terminal tabs
 *  Copyright (c) 2003-2012 Brad Proctor <brad@bradleyproctor.com>
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
   "Usage: tabs long-option\n"
   "       tabs [-n|-a|-a1|-a2|-c|-c2|-c3|-C2|-C3|-C4|-f|-p|-s|-u][+m[n]][-T type]\n"
   "       tabs [-T type][+[n]] n1[,n2,...]\n"
   "Set terminal tabs.\n"
   "\n"
   "  -a         Assembler      1,10,16,36,72\n"
   "  -a1        Assembler      1,9,17,25,33,41,49,57,65,73\n"
   "  -a2        Assembler      1,10,16,40,72\n"
   "  -c         Cobol          1,8,12,16,20,55\n"
   "  -c1        Cobol          ...\n"
   "  -c2        Cobol Compact  1,6,10,14,49\n"
   "  -c3        Cobol Compact  1,6,10,14,18,22,26,30,34,38,42,46,50,54,58,62,67\n"
   "  -C2        C, C++         1,3,5,7,9,11,13,15,17,19,21,23,25,27,29,31,33,35,\n"
   "                            37,39,41,43,45,47,49,51,53,55,57,59,61,63,65,67,\n"
   "                            69,71,73,75,77,79\n"
   "  -C3        C, C++         1,4,7,10,13,16,19,22,25,28,31,34,37,40,43,47,50,\n"
   "                            53,56,59,62,65,68,71,74,77\n"
   "  -C4        C, C++         1,5,9,13,17,21,25,29,33,37,41,45,49,53,57,61,65,\n"
   "                            69,73,77\n"
   "  -f         Fortran        1,7,11,15,19,23\n"
   "  -p         PL/1           1,5,9,13,17,21,25,29,33,37,41,45,49,53,57,61\n"
   "  -s         Snobol         1,10,55\n"
   "  -u         Assembler      1,12,20,44\n"
   "\n"
   "  -n         \n"
   "  -T type    Indicate the type of terminal\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "https://github.com/bproctor/utils"
};

#define FLAG_A    1
#define FLAG_A1   2
#define FLAG_A2   3
#define FLAG_C    4
#define FLAG_C1   5
#define FLAG_C2   6
#define FLAG_C3   7
#define FLAG_CC   8
#define FLAG_CC2  9
#define FLAG_CC3  10
#define FLAG_CC4  11
#define FLAG_F    12
#define FLAG_P    13
#define FLAG_S    14
#define FLAG_U    15

static int flag = 0;
static int tabs[80], num_tabs = 0;


static char *flag_name[12] =
{
    "", "a", "a1", "a2", "c", "c1",
    "c2", "c3", "f", "p", "s", "u"
};

static char tab_list[][40] =
{
   { 1, 10, 16, 36, 72},         /* -a  Assembler     */

   { 1,  9, 17, 25, 33, 41,      /* -a1 Assembler     */
    49, 57, 65, 73},

   { 1, 10, 16, 40, 72},         /* -a2 Assembler     */

   { 1,  8, 12, 16, 20, 55},     /* -c  Cobol         */

   {0},                          /* -c1 Cobol (incomplete) */

   { 1,  6, 10, 14, 49},         /* -c2 Cobol Compact */

   { 1,  6, 10, 14, 18, 22,      /* -c3 Cobol Compact */
    26, 30, 34, 38, 42, 46, 
    50, 54, 58, 62, 67},

   {0},                          /* -C C, C++ (incomplete) */
   
   { 1,  3,  5,  7,  9, 11,      /* -C2 C, C++        */
    13, 15, 17, 19, 21, 23,
    25, 27, 29, 31, 33, 35,
    37, 39, 41, 43, 45, 47,
    49, 51, 53, 55, 57, 59,
    61, 63, 65, 67, 69, 71,
    73, 75, 77, 79},

   { 1,  4,  7, 10, 13, 16,      /* -C3 C, C++        */
    19, 22, 25, 28, 31, 34,
    37, 40, 43, 47, 50, 53,
    56, 59, 62, 65, 68, 71,
    74, 77},
    
   { 1,  5,  9, 13, 17, 21,      /* -C4 C, C++        */
    25, 29, 33, 37, 41, 45,
    49, 53, 57, 61, 65, 69,
    73, 77},
   
   { 1,  7, 11, 15, 19, 23},     /* -f  Fortran       */

   { 1,  5,  9, 13, 17, 21,      /* -p  PL/1          */
    25, 29, 33, 37, 41, 45,
    49, 53, 57, 61},

   { 1, 10, 55},                 /* -s  Snobol        */

   { 1, 12, 20, 44},             /* -u  Assembler     */
};


static void do_tabs (void)
{
   if (num_tabs)
   {
   }
   else
   {
   }
}

/*
 * Main
 */
 
int command_tabs (int argc, char **argv)
{
   int i;
   char *ptr;
   
   progname = argv[0];
   
   for (i = 1; i < argc; ++i)
   {
      ptr = argv[i];
      if (*ptr == '-')
      {
         switch (*ptr)
         {
            case '-':   
               if (!strcmp (ptr, "-help"))
                  puts (help_text);
               else if (!strcmp (ptr, "-version"))
                  puts ("tabs: version "VERSION);
               else
                  terror ("invalid option `%s'", argv[i]);
               return (EXIT_SUCCESS);
            case 'a':
               if (*++ptr == '\0')
               {
                  if (flag == 0)
                     flag = FLAG_A;
                  else
                     terror ("cannot use `-a' with `-%s'", flag_name[flag]);
               }
               else if (*ptr == '1' && *(ptr+1) == '\0')
               {
                  if (flag == 0)
                     flag = FLAG_A1;
                  else
                     terror ("cannot use `-a1' with `-%s'", flag_name[flag]);
               }
               else if (*ptr == '2' && *(ptr+1) == '\0')
               {
                  if (flag == 0)
                     flag = FLAG_A2;
                  else
                     terror ("cannot use `-a2' with `-%s'", flag_name[flag]);
               }
               else
                  terror ("invalid option `%s'", argv[i]);
               break;
            case 'c':
               if (*++ptr == '\0')
               {
                  if (flag == 0)
                     flag = FLAG_C;
                  else
                     terror ("cannot use `-c' with `-%s'", flag_name[flag]);
               }
               else if (*ptr == '1' && *(ptr+1) == '\0')
               {
                  if (flag == 0)
                     flag = FLAG_C1;
                  else
                     terror ("cannot use `-c1' with `-%s'", flag_name[flag]);
               }
               else if (*ptr == '2' && *(ptr+1) == '\0')
               {
                  if (flag == 0)
                     flag = FLAG_C2;
                  else
                     terror ("cannot use `-c2' with `-%s'", flag_name[flag]);
               }
               else if (*ptr == '3' && *(ptr+1) == '\0')
               {
                  if (flag == 0)
                     flag = FLAG_C3;
                  else
                     terror ("cannot use `-c3' with `-%s'", flag_name[flag]);
               }
               else
                  terror ("invalid option `%s'", argv[i]);
               break;
            case 'C':
               if (*++ptr == '\0')
               {
                  if (flag == 0)
                     flag = FLAG_CC;
                  else
                     terror ("cannot use `-C' with `-%s'", flag_name[flag]);
               }
               else if (*ptr == '2' && *(ptr+1) == '\0')
               {
                  if (flag == 0)
                     flag = FLAG_CC2;
                  else
                     terror ("cannot use `-C2' with `-%s'", flag_name[flag]);
               }
               else if (*ptr == '3' && *(ptr+1) == '\0')
               {
                  if (flag == 0)
                     flag = FLAG_CC3;
                  else
                     terror ("cannot use `-C3' with `-%s'", flag_name[flag]);
               }
               else if (*ptr == '4' && *(ptr+1) == '\0')
               {
                  if (flag == 0)
                     flag = FLAG_CC4;
                  else
                     terror ("cannot use `-C4' with `-%s'", flag_name[flag]);
               }
               else
                  terror ("invalid option `%s'", argv[i]);
               break;
            case 'f':
               if (*++ptr == '\0')
               {
                  if (flag == 0)
                     flag = FLAG_F;
                  else
                     terror ("cannot use `-f' with `-%s'", flag_name[flag]);
               }
               else
                  terror ("invalid option `%s'", argv[i]);
               break;
            case 'p':
               if (*++ptr == '\0')
               {
                  if (flag == 0)
                     flag = FLAG_P;
                  else
                     terror ("cannot use `-p' with `-%s'", flag_name[flag]);
               }
               else
                  terror ("invalid option `%s'", argv[i]);
               break;
            case 's':
               if (*++ptr == '\0')
               {
                  if (flag == 0)
                     flag = FLAG_S;
                  else
                     terror ("cannot use `-s' with `-%s'", flag_name[flag]);
               }
               else
                  terror ("invalid option `%s'", argv[i]);
               break;
            case 'u':
               if (*++ptr == '\0')
               {
                  if (flag == 0)
                     flag = FLAG_U;
                  else
                     terror ("cannot use `-u' with `-%s'", flag_name[flag]);
               }
               else
                  terror ("invalid option `%s'", argv[i]);
               break;
            case 'n':
               /* What's -n do again? */
               break;
            case 'T':
               if (++i < argc)
               {
                  /* Get the terminal type */
               }
               else
                  terror ("missing operand to `-T' option");
               break;
            default:
               terror ("invalid option `%s'", argv[i]);
         }
      }
      else if (isdigit (*ptr))
      {
         int i = *ptr - '0';
         
         while (1)
         {
            while (isdigit (*++ptr))
               i = i * 10 + *ptr - '0';
            tabs[num_tabs++] = i;
            i = 0;
            if (*ptr == ',')
               continue;
            else
               break;
         }
      }
      else if (*ptr == '+')
      {
      }
   }
   do_tabs ();
   return (EXIT_SUCCESS);
}
