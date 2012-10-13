
/*
 *  ls -- list directory contents
 *  Copyright (c) 2001-2012 Brad Proctor <brad@bradleyproctor.com>
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "utils.h"

#define FLAG_LONG    0x01

static const char help_text[] =
{
   "Usage: ls long-option\n"
   "       ls [-aAbBcCdfFgGHilLmnopqrRsStuUxX1] [-H | -L] [-w width] [file ...]\n"
   "Print a list of files.\n"
   "\n"
   "  -a         All, Do not hide entries starting with .\n"
   "  -A         Do not list entries . or ..\n"
   "  -b         Print octal escapes for each non-graphic character\n"
   "  -B         Do not list entries ending with ~\n"
   "  -c         Show ctime information\n"
   "  -C         List entries by columns\n"
   "  -d         List directory entries instead of contents\n"
   "  -f         Do not sort\n"
   "  -F         \n"
   "  -g         When -l specified, do not list owner information\n"
   "  -G         \n"
   "  -H         \n"
   "  -i         Print index number of each file\n"
   "  -l         Long listings\n"
   "  -L         Dereference symbolic links\n"
   "  -m         \n"
   "  -n         List numeric user and group ID's instead of names\n"
   "  -o         When -l is specified, do not list group information\n"
   "  -p         Apppend file type indicator to file names\n"
   "  -q         Print ? instead of non-graphic characters\n"
   "  -r         Reverse order when sorting\n"
   "  -R         Recursive, list subdirectories\n"
   "  -s         \n"
   "  -S         Sort listing by size\n"
   "  -t         Sort listing by modification time\n"
   "  -u         \n"
   "  -U         Do not sort, list files in directory order\n"
   "  -w width   Assume screen width instead of default\n"
   "  -x         List entries by lines instead of columns\n"
   "  -X         Sort alphabetically by entry extention\n"
   "  -1         List one entry per line\n"
   "\n"
   "Long Options:\n"
   "  --help     Print this help message\n"
   "  --version  Print the program version\n"
   "\n"
   "https://github.com/bproctor/utils"
};

static void ls (int options)
{
   DIR *dir;
   struct stat *buf;
   struct dirent *next;
   char string[20];

   dir = opendir ("./");
   if (dir != NULL)
   {
      switch (options)
      {
         case 0:
            while ((next = readdir (dir)) != NULL)
               printf ("%s ", next->d_name);
            break;
         case FLAG_LONG:
            buf = (struct stat *)malloc (sizeof (struct stat));
            while ((next = readdir (dir)) != NULL)
            {
               switch (next->d_type)
               {
                  case DT_DIR:      putchar ('d'); break;
                  case DT_FIFO:     putchar ('f'); break;
                  case DT_SOCK:     putchar ('s'); break;
                  case DT_CHR:      putchar ('c'); break;
                  case DT_BLK:      putchar ('b'); break;
                  default:          putchar ('-'); break;
               }
               
               stat (next->d_name, buf);
               putchar (buf->st_mode & S_IRUSR ? 'r' : '-');
               putchar (buf->st_mode & S_IWUSR ? 'w' : '-');
               putchar (buf->st_mode & S_IXUSR ? 'x' : '-');
               putchar (buf->st_mode & S_IRGRP ? 'r' : '-');
               putchar (buf->st_mode & S_IWGRP ? 'w' : '-');
               putchar (buf->st_mode & S_IXGRP ? 'x' : '-');
               putchar (buf->st_mode & S_IROTH ? 'r' : '-');
               putchar (buf->st_mode & S_IWOTH ? 'w' : '-');
               putchar (buf->st_mode & S_IXOTH ? 'x' : '-');
               strftime (string, 20, "%b %2d %k:%M", localtime (&buf->st_mtime));
               printf ("%5d %8d %8d %10d %s %s\n", 
                  (int)buf->st_nlink, 
                  (int)buf->st_uid,
                  (int)buf->st_gid, 
                  (int)buf->st_size, 
                  string, 
                  next->d_name);
            }
      }
      closedir (dir);
   }
}

/*
 * Main
 */
 
int command_ls (int argc, char **argv)
{
   int i, opt = 0;
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
            case '-':
               if (!strcmp (ptr, "-help"))
                  puts (help_text);
               else if (!strcmp (ptr, "-version"))
                  puts ("ls: version "VERSION);
               else
                  terror ("invalid option `%s'", argv[i]);
               return (EXIT_SUCCESS);
            case 'a':
               continue;
            case 'A':
               continue;
            case 'b':
               continue;
            case 'B':
               continue;
            case 'c':
               continue;
            case 'C':
               continue;
            case 'd':
               continue;
            case 'f':
               continue;
            case 'F':
               continue;
            case 'g':
               continue;
            case 'G':
               continue;
            case 'h':
               continue;
            case 'H':
               continue;
            case 'i':
               continue;
            case 'k':
               continue;
            case 'l':
               opt |= FLAG_LONG;
               continue;
            case 'L':
               continue;
            case 'm':
               continue;
            case 'n':
               continue;
            case 'o':
               continue;
            case 'p':
               continue;
            case 'q':
               continue;
            case 'r':
               continue;
            case 'R':
               continue;
            case 's':
               continue;
            case 'S':
               continue;
            case 't':
               continue;
            case 'T':
               continue;
            case 'u':
               continue;
            case 'U':
               continue;
            case 'v':
               continue;
            case 'w':
               if (*++ptr != '\0')
                  ;
               else if (++i < argc)
                  ptr = argv[i];
               else
                  terror ("missing operand to `-w' option");
               continue;
            case 'x':
               continue;
            case 'X':
               continue;
            case '1':
               continue;
         }
      }
   }
   ls (opt);
   return (EXIT_SUCCESS);
}
