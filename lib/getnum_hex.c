
/*
 *  getnum_hex.c
 *  Copyright (c) 2003-2012 Brad Proctor
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <ctype.h>
#include "utils.h"

long long (getnum_hex)(const char *s)
{
   char sign = '+';
   long long i = 0;

   if (*s == '-' || *s == '+')
      sign = *s++;

   if (!isdigit (*s))
      terror ("malformed number `%s'", s);
   
   while (isxdigit (*s))
      i = (i << 4) + *s++ - '0';
   
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
