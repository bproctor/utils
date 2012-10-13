
/*
 *  getnum_ll.c
 *  Copyright (c) 2003-2012 Brad Proctor
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <ctype.h>
#include "utils.h"

long long (getnum_ll)(const char *s)
{
   char sign = '+';
   long long i = 0;

   if (*s == '-' || *s == '+')
      sign = *s++;

   if (!isdigit (*s))
      terror ("malformed number `%s'", s);

   if (*s == '0')
   {
      if (*++s == 'x' || *s == 'X')
      {
         while (1)
         {
            if (isdigit (*++s))
               i = (i << 4) + *s - '0';
            else if (*s >= 'a' && *s <= 'f')
               i = (i << 4) + *s - 'a' + 10;
            else if (*s >= 'A' && *s <= 'F')
               i = (i << 4) + *s - 'A' + 10;
            else
               break;
         }
      }
      else
      {
         while (*s >= '0' && *s <= '7')
            i = (i << 3) + *s - '0';
      }
   }
   else
   {
      while (isdigit (*s))
         i = (i * 10) + *s++ - '0';
   }
   
   switch (*s)
   {
      case '\0':
         break;
      case 'k':
      case 'K':
         if (*++s == 'b')
            i *= 1000;
         else if (*s == 'B' || *s == '\0')
            i <<= 10;
         else
            terror ("malformed number `%s'", s);
         break;
      case 'm':
      case 'M':
         if (*++s == 'b')
            i *= 1000000;
         else if (*s == 'B' || *s == '\0')
            i <<= 20;
         else
            terror ("malformed number `%s'", s);
         break;
      case 'g':
      case 'G':
         if (*++s == 'b')
            i *= 1000000000;
         else if (*s == 'B' || *s == '\0')
            i <<= 30;
         else
            terror ("malformed number `%s'", s);
      default:
         terror ("malformed number `%s'", s);
   }

   if (sign == '-')
      i = -i;
      
   return (i);
}
