
/*
 *  getnum_d.c
 *  Copyright (c) 2003-2012 Brad Proctor
 *
 *  Licensed under The MIT License
 *  Redistributions of files must retain the above copyright notice.
 */

#include <ctype.h>
#include "utils.h"

double (getnum_d)(const char *s)
{
   char sign = '+';
   char found_decimal = 0;
   char found_exponent = 0;
   char exponent_sign = '+';

   int digits = 0;
   double num = 0, num1 = 0, num2 = 0, exp = 0;

   if (*s == '-' || *s == '+')
      sign = *s++;

   while (*s)
   {
      switch (*s)
      {
         case '\0':
            break;
         case '.':
            if (found_decimal)
               terror ("malformed floating-point number `%s'", s);
            found_decimal = 1;
            break;
         case '0': case '1': case '2':
         case '3': case '4': case '5':
         case '6': case '7': case '8':
         case '9':
            if (found_decimal)
               num2 = num2 * 10 + *s - '0', digits++;
            else if (found_exponent)
               exp = exp * 10 + *s - '0';
            else
               num1 = num1 * 10 + *s - '0';
            break;
         case 'e':
         case 'E':
            if (*++s == '-')
               exponent_sign = '-';
            else if (*s == '+')
               exponent_sign = '+';
            found_exponent = 1;
            break;
         default:
            break;
      }
      ++s;
   }      

   while (digits--)
      num2 /= 10;

   num = num1 + num2;

   if (exponent_sign == '-')
      while (exp--)
         num /= 10;
   else
      while (exp--)
         num *= 10;

   if (sign == '-')
      num = -num;

   return (num);
}
