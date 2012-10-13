
/*
 *  sort -- sort, merge, or sequence check text files
 *  Copyright (c) 1999-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "utils.h"

struct tree
{
   char *string;
   struct tree *left;
   struct tree *right;
};

struct file
{
   FILE *file;
   struct file *next;
};

static const char help_text[] =
{
   "Usage: sort long-option\n"
   "       sort [-m][-bcdfimnr][-o name][-k key][-t char] file ...\n"
   "       sort -c [-bcdfimnr][-o name][-k key][-t char] file ...\n"
   "Sort, merge, or sequence check text files.\n"
   "\n"
   "  -b         Ignore leading blanks\n"
   "  -c         No output, only exit code\n"
   "  -d         Sort on only blanks and alpha-numeric characters\n"
   "  -f         Case insensitive\n"
   "  -i         Ignore non-printable characters\n"
   "  -k key     Specify sort key\n"
   "  -m         Merge only\n"
   "  -n         Numerical sort\n"
   "  -o file    Specify output file\n"
   "  -r         Reverse sort\n"
   "  -t char    Use this field separator\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "https://github.com/bproctor/utils"
};

static char flag_alpha = 0;
static char flag_print = 0;
static char flag_seperator = 0;
static char flag_case = 0;
static char flag_reverse = 0;
static char flag_blanks = 0;
static char buf[4096];
static struct tree *head = 0;
static struct file *fhead = 0;
static struct file *fnext;

static int compare_other (const char *a, const char *b)
{
   const char *p1 = a, *p2 = b;
   
   while (*p1 && *p2)
   {
      if (flag_print)
      {
         while (!isprint (*p1))
            ++p1;
         while (!isprint (*p2))
            ++p2;
      }
      else
      {
         while (!isalnum (*p1))
            ++p1;
         while (!isalnum (*p2))
            ++p2;
      }      
      if (flag_case)
      {
         if (tolower (*p1) < tolower (*p2))
            return (-1);
         else if (tolower (*p1) > tolower (*p2))
            return (1);
      }
      else
      {
         if (*p1 < *p2)
            return (-1);
         else if (*p1 > *p2)
            return (1);
      }
      ++p1, ++p2;
   }
   if (!*p1 && *p2)
      return (-1);
   else if (*p1 && !*p2)
      return (1);
   return (0);
}

static int compare_strings (const char *a, const char *b)
{
   const char *p1 = a, *p2 = b;
   
   if (flag_blanks)
   {
      while (isspace (*p1) && *p1 != '\0')
         ++p1;
      while (isspace (*p2) && *p2 != '\0')
         ++p2;
   }
   
   if (flag_alpha || flag_print)
      return (compare_other (p1, p2));

   if (flag_case)
   {
      while (*p1 && *p2)
      {
         if (tolower (*p1) < tolower (*p2))
            return (-1);
         else if (tolower (*p1) > tolower (*p2))
            return (1);
         ++p1, ++p2;
      }
   }
   else
   {
      while (*p1 && *p2)
      {
         if (*p1 < *p2)
            return (-1);
         else if (*p1 > *p2)
            return (1);
         ++p1, ++p2;
      }
   }
   if (*p1 && !*p2)
      return (-1);
   else if (!*p1 && *p2)
      return (1);
   return (0);
}

static int read_line (void)
{
   int c;
   char *ptr;
   
   ptr = buf;
   while (1)
   {
      c = getc (fnext->file);

      if (flag_seperator)
      {
         if (flag_seperator == c)
         {
            *ptr++ = c, *ptr = '\0';
            return (0);
         }
      }
      else if (c == '\n')
      {
         *ptr++ = c, *ptr = '\0';
         return (0);
      }

      if (c == EOF)
      {
         *ptr = '\0';
         if (fnext->next)
         {
            fnext = fnext->next;
            return (0);
         }
         return (1);
      }
            
      *ptr++ = c;
   }
}

static struct tree *insert_node (void)
{
   int i = 0;
   char *ptr = buf;
   struct tree *last, *t;
   
   last = t = head;
   
   while (t)
   {
      last = t;
      i = compare_strings (t->string, ptr);
      t = (i <= 0) ? t->left : t->right;
   }
   t = xmalloc (sizeof (struct tree));
   if (last)
   {
      if (i <= 0)
         last->left = t;
      else
         last->right = t;
   }
   else
      head = t;         
   t->string = xmalloc (strlen (ptr));
   strcpy (t->string, ptr);
   t->right = t->left = NULL;
   return (t);
}

static void traverse_tree (struct tree *t)
{
   if (t == NULL)
      return;
   else
   {
      if (flag_reverse)
      {
         traverse_tree (t->left);
         fputs (t->string, stdout);
         traverse_tree (t->right);
      }
      else
      {
         traverse_tree (t->right);
         fputs (t->string, stdout);
         traverse_tree (t->left);
      }
   }
   t = NULL;
}

static void do_sort (void)
{
   while (read_line () == 0)
      insert_node ();
   insert_node ();
   traverse_tree (head);
}

static void add_file (const char *name)
{
   struct file *prev, *f;

   prev = f = fhead;
   
   while (f)
   {
      prev = f;
      f = f->next;
   }

   f = xmalloc (sizeof (struct file));
   f->file = xfopen (name, "r");
   f->next = NULL;   

   if (fhead == NULL)
      fhead = f;
   else
      prev->next = f;
}

/*
 * Main
 */

int command_sort (int argc, char **argv)
{
   int i;
   char *ptr;

   progname = argv[0];
   
   for (i = 1; i < argc; ++i)
   {
      ptr = argv[i];
      if (*ptr == '-')
      {
         while (*++ptr)
         switch (*ptr)
         {
            case '\0':
               break;
            case '-':
               if (!strcmp (ptr, "-help"))
                  puts (help_text);
               else if (!strcmp (ptr, "-version"))
                  puts ("sort: version "VERSION);
               else
                  terror ("invalid option `%s'", argv[i]);
               return (EXIT_SUCCESS);
            case 'b':
               flag_blanks = 1;
               continue;
            case 'd':
               flag_alpha = 1;
               continue;
            case 'f':
               flag_case = 1;
               continue;
            case 'i':
               flag_print = 1;
               continue;
            case 'o':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-o' option");
               add_file (ptr);
               break;
            case 'r':
               flag_reverse = 1;
               break;
            case 't':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-t' option");
               flag_seperator = *ptr;
               break;
            case 'c':
               continue;
            case 'k':
               continue;
            case 'm':
               continue;
            case 'n':
               continue;
         }
      }
      else
         add_file (argv[i]);
   }

   fnext = fhead;
   do_sort ();
   
   return (EXIT_SUCCESS);
}
